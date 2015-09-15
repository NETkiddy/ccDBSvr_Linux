#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>

using namespace sql;

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "123"

int main() 
{
   Driver *driver = NULL;
   Connection *conn = NULL;
   driver = get_driver_instance();
   std::cout<<DBHOST<<std::endl;
   std::string sDBHOST = DBHOST;
   std::string sUSER = USER;
   std::string sPASSWORD = PASSWORD;
   conn = driver->connect(sDBHOST, sUSER, sPASSWORD);
 	if(conn)	
		std::cout<<"Connectted!!"<<std::endl;
	else
		std::cout<<"Failed, unConnectted"<<std::endl;
		
	conn->setAutoCommit(0);
   cout<<"DataBase connection autocommit mode = "<<conn->getAutoCommit()<<endl;
   delete conn;
   driver = NULL;
   conn = NULL;
   return 0;
}
