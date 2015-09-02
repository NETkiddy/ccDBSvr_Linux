#include "MySqlDB.h"
#include "define.h"
#include "ConfigSvr.h"
#include <string>
#include <sstream>


MySqlDB::MySqlDB(Config cfg)
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
	std::string connectStr = m_sMode + "://" + m_sIP + ":"  + ConfigSvr::intToStr(m_iPort);
	m_conn = m_driver->connect(connectStr, m_sUsername, m_sPassword);
	//stmt = con->createStatement();
	//m_conn->setAutoCommit(0);
	if(m_conn)
	{
		bRet = true;
		ConfigSvr::log_error("Mysql Server Open Success");
	}
	else
	{
		ConfigSvr::log_error("Mysql Server Open Failed");
	}

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
	m_conn->setSchema("siccdb");	
	m_stmt = m_conn->createStatement();
	m_resSet = m_stmt->executeQuery(queryStr);
	
	while (m_resSet->next()) 
	{
	    std::cout << "... MySQL replies: "<<std::endl;
		/* Access column data by alias or column name */
		std::cout << "id: ";
		std::cout << m_resSet->getString("id")<<"===";
		std::cout << "name: ";
		/* Access column fata by numeric offset, 1 is the first column */
		std::cout << m_resSet->getString("name") <<"===";
		std::cout << "gentle: ";
		std::cout << m_resSet->getString("gentle") <<"===";
		std::cout << "degree: ";
		std::cout << m_resSet->getString("degree") <<std::endl;
	}
}

BaseCommand::BaseCommand()
{
}

BaseCommand::~BaseCommand()
{
}


