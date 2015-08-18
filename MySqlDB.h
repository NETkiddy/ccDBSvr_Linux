#ifndef _MYSQL_DB_H_
#define _MYSQL_DB_H_

#include "DataStruct.h"
#include "ConfigSvr.h"

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class MySqlDB : public SQLDB
{
public:
	sql::Driver         *m_driver;
	sql::Connection     *m_conn;
	sql::Statement 		*m_stmt;
	sql::ResultSet 		*m_resSet;
public:
	MySqlDB(Config cfg);
	~MySqlDB();
	bool open();
	bool close();
	bool execute(std::string sQueryStr);
};





#endif
