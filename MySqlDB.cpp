#include "MySqlDB.h"
#include "define.h"
#include "ConfigSvr.h"
#include <sstream>
#include <openssl/sha.h>
#include "ConfigSvr.h"
#include <string.h>
#include <vector>
#include <string>

MySqlDB::MySqlDB(Config cfg)
: redisClient(cfg[REDIS_IP], std::stoi(cfg[REDIS_PORT]))
{
	m_sIP = cfg[DB_SOURCE];
	m_iPort = std::stoi(cfg[DB_PORT]);
	//m_iPort = atoi(cfg[PORT].c_str());
	m_sMode = cfg[DB_MODE];
	m_sUsername = cfg[DB_USERNAME];
	m_sPassword = cfg[DB_PASSWORD];

	m_driver = get_driver_instance();

}

MySqlDB::~MySqlDB()
{
	if(m_conn)
		delete m_conn;
	m_conn = nullptr;
	if(m_stmt)
		delete m_stmt;//You must free sql::Statement and sql::Connection objects explicitly using delete.
	m_stmt = nullptr;
	
	//delete m_driver;//But do not explicitly free driver, the connector object! The connector will take care of freeing that.

}


/*
As of Connector/C++, these methods can be used to check the connection state or reconnect:

sql::Connection::isValid() checks whether the connection is alive

sql::Connection::reconnect() reconnects if the connection has gone down
*/
bool MySqlDB::open()
{
	bool bRet = false;
	std::string connectStr = m_sIP + ":"  + ConfigSvr::intToStr(m_iPort);
	m_conn = m_driver->connect(connectStr, m_sUsername, m_sPassword);
	//stmt = con->createStatement();
	//m_conn->setAutoCommit(0);
	if(m_conn)
	{
		bRet = true;
		ConfigSvr::log_error("Mysql Open Success");
	}
	else
	{
		ConfigSvr::log_error("Mysql Open Failed");
	}
	
	// connect redis server
	redisClient.connect();

	return bRet;
}

bool MySqlDB::close()
{
	if(m_conn)
		delete m_conn;
	m_conn = nullptr;
	if(m_stmt)
		delete m_stmt;//You must free sql::Statement and sql::Connection objects explicitly using delete.
	m_stmt = nullptr;

	ConfigSvr::log_error("Mysql Close Success");
}

bool MySqlDB::isConnected()
{
	return m_conn->isValid();
}

void MySqlDB::reconnect()
{
	m_conn->reconnect();
}
/*
use the methods: 
	sql::Statement::execute(), 
	sql::Statement::executeQuery(),
	sql::Statement::executeUpdate(). 
Use the method sql::Statement::execute() if your query does not return a result set 
or if your query returns more than one result set
*/
bool MySqlDB::execute(std::string queryStr)
{
	bool bRet = false;
	try
	{
		//compute SHA1 of querystr
		unsigned char cSHA1QueryStr[SHA_DIGEST_LENGTH];
		memset(cSHA1QueryStr, 0, SHA_DIGEST_LENGTH);
		SHA1((unsigned char*)(queryStr.c_str()), queryStr.size(), cSHA1QueryStr);
		std::string sSHA1QueryStr((char*)(cSHA1QueryStr));
		
		std::cout<<"sSHA1QueryStr: "<<sSHA1QueryStr<<std::endl;
		//try hit
		int iType = 2;//1 => string
		int iTTL = 10000;
		std::vector<std::string> redisRowKeyVector;
		std::string sCacheType = (1 == iType) ? "string" : "hash";
		//generate redis SET key
		std::string sRedisRowSetKey = "resultset." + sCacheType + ":" + sSHA1QueryStr;
		if(getCacheFromRedis(sRedisRowSetKey, redisRowKeyVector, iType))
		{
			//if hit
			parseRedisRowKeyVector(redisRowKeyVector);
		}
		else//if not hit
		{
			//execute query on DB
			m_conn->setSchema("siccdb");	
			m_stmt = m_conn->createStatement();
			m_resSet = m_stmt->executeQuery(queryStr);
			
			//
			if(1 == iType)
			{
				//sRedisRowSetKey = cache2String(m_resSet, sSHA1QueryStr, iTTL);
			}
			else
			{
				sRedisRowSetKey = cache2Hash(m_resSet, sSHA1QueryStr, iTTL);
			}
			
			//retry hit
			if(getCacheFromRedis(sRedisRowSetKey, redisRowKeyVector, iType))
			{
				parseRedisRowKeyVector(redisRowKeyVector);
			}
		}
	
		bRet = true;
	}
	catch(...)
	{
		std::cout<<"execute error: "<<std::endl;
		bRet = false;
	}
	return bRet;
}

bool MySqlDB::getCacheFromRedis(std::string sRedisRowSetKey, std::vector<std::string>& redisRowKeyVector, int iType)
{
	//try obtain set key from redis SET
	RedisClientReply reply = redisClient.executeCommand(std::string("SMEMBERS %s"), 1, sRedisRowSetKey);
	if(REDIS_REPLY_ARRAY == reply.iType)
	{
		if(reply.vecValue.empty())
		{
			std::cout<<"redis not hit"<<std::endl;
			return false;
		}

		std::cout<<"redis hit!!"<<std::endl;
		//if find SET key in redis, save Row key to redisRowKeyVector
		std::vector<std::string>::iterator iter = reply.vecValue.begin();
		for(; iter != reply.vecValue.end(); ++iter)
		{
			redisRowKeyVector.push_back(*iter);
		}
			
	}
	
	return true;
}

std::string MySqlDB::cache2Hash(sql::ResultSet *resSet, std::string sSHA1QueryStr, int iTTL)
{
	std::string sTTL = ConfigSvr::intToStr(iTTL);

	if(0 == resSet->rowsCount())
	{
		std::cout<<"resultset from sql excution is empty"<<std::endl;
	}
	//generate prefix of hash key, start form 1
	std::string prefix("cache.hash:" + sSHA1QueryStr + ":");
	unsigned int rowNum = 1;//hash num
	sql::ResultSetMetaData *meta = resSet->getMetaData();
	unsigned int colNum = meta->getColumnCount();
	std::string redisRowSetKey("resultset.hash:" + sSHA1QueryStr);
	//
	resSet->beforeFirst();
	while(resSet->next())
	{
		std::string redisRowKey = prefix + ConfigSvr::intToStr(rowNum);
		for(int i = 1; i <= colNum; ++i)
		{
			//obtain value
			std::string colLabel = meta->getColumnLabel(i);
			std::string colValue = resSet->getString(colLabel);
			//save row key to redis HASH
			redisClient.executeCommand(std::string("HSET %s %s %s"), 3, redisRowKey, colLabel, colValue);
			//save hash key to redis SET
			redisClient.executeCommand(std::string("SADD %s %s"), 2, redisRowSetKey, redisRowKey);
			//set HASH expiration
			redisClient.executeCommand(std::string("EXPIRE %s %s"), 2, redisRowKey, sTTL);
		}

		rowNum++;
	}
	
	//set SET expiration
	redisClient.executeCommand(std::string("EXPIRE %s %s"), 2, redisRowSetKey, sTTL);

	return redisRowSetKey;
}

void MySqlDB::parseRedisRowKeyVector(std::vector<std::string> rowKeyVector)
{
	std::vector<std::string>::iterator iter = rowKeyVector.begin();
	for(; iter != rowKeyVector.end(); ++iter)
	{
		
		
		RedisClientReply reply = redisClient.executeCommand(std::string("HGETALL %s"), 1, *iter);
		
		std::vector<std::string>::iterator iter = reply.vecValue.begin();
		for(; iter != reply.vecValue.end(); ++iter)
		{
			std::cout<<*iter<<std::endl;
		}
		/*
		//collect result
		while (m_resSet->next()) 
		{
			std::cout << "============MySQL replies============"<<std::endl;
			// Access column data by alias or column name 
			std::cout << "id: ";
			std::cout << m_resSet->getString("id")<<"+++";
			std::cout << "name: ";
			// Access column fata by numeric offset, 1 is the first column 
			std::cout << m_resSet->getString("name") <<"+++";
			std::cout << "gentle: ";
			std::cout << m_resSet->getString("gentle") <<"+++";
			std::cout << "degree: ";
			std::cout << m_resSet->getString("degree") <<std::endl;
		}
		*/
	}
}
