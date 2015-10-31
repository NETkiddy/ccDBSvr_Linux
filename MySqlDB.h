#ifndef _MYSQL_DB_H_
#define _MYSQL_DB_H_

#include "DataStruct.h"
#include "ConfigSvr.h"
#include "RedisClient.h"

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <vector>
#include <string>

class MySqlDB : public BaseDB
{
public:
	sql::Driver         *m_driver;
	sql::Connection     *m_conn;
	sql::Statement 		*m_stmt;
	sql::ResultSet 		*m_resSet;
	RedisClient			redisClient;
public:
	MySqlDB(Config cfg);
	~MySqlDB();
	bool open();
	bool close();
	bool isConnected();
	void reconnect();
	bool execute(std::string sQueryStr);
	bool getCacheFromRedis(std::string sRedisRowSetKey, std::vector<std::string>& redisRowKeyVector, int iType);
	void parseRedisRowKeyVector(std::vector<std::string> rowKeyVector);
	std::string cache2Hash(sql::ResultSet *resSet, std::string sSHA1QueryStr, int iTTL);
	
};





#endif
