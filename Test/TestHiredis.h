#ifndef _TEST_HIREDIS_H_
#define _TEST_HIREDIS_H_
#include <string>
#include <hiredis/hiredis.h>

class TestHiredis
{
public:
	TestHiredis();
	~TestHiredis();
	
	void excuteCommand(std::string commStr, std::string param1);


	redisContext *conn;
};


#endif
