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
   Driver *driver;
   Connection *conn;
   driver = get_driver_instance();
   conn = driver->connect(DBHOST, USER, PASSWORD);
   conn->setAutoCommit(0);
   cout<<"DataBase connection autocommit mode = "<<conn->getAutoCommit()<<endl;
   delete conn;
   driver = NULL;
   conn = NULL;
   return 0;
}
