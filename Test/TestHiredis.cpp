#include <iostream>
#include "TestHiredis.h"

TestHiredis::TestHiredis()
{
		conn = redisConnect("127.0.0.1", 6379);
		if(conn->err)
		{
			std::cout<<"connection error: "<<conn->errstr<<std::endl;
		}

		std::cout<<"connection succeed"<<std::endl;
}

TestHiredis::~TestHiredis()
{
	redisFree(conn);
}

void TestHiredis::excuteCommand(std::string commStr, std::string param1)
{
	redisReply *reply = static_cast<redisReply*>(redisCommand(conn, commStr.c_str(), param1.c_str()));
	if(REDIS_REPLY_INTEGER == reply->type 
		|| REDIS_REPLY_STRING == reply->type)
	{
		std::cout<<reply->str<<std::endl;
	}
	if(REDIS_REPLY_ARRAY == reply->type)
	{
		for(int i = 0; i < reply->elements; ++i)
		{
			std::cout<<reply->element[i]->str<<std::endl;
		}
	}
	if(REDIS_REPLY_NIL == reply->type)
	{
		std::cout<<"not a answer"<<std::endl;
	}
	if(REDIS_REPLY_ERROR == reply->type)
	{
		std::cout<<"error"<<std::endl;
	}

	freeReplyObject(reply);
}

int main()
{
	TestHiredis hRedis;
	//hRedis.excuteCommand("set test:1 xiangzhou");
	//hRedis.excuteCommand("set test:2 quanzhou");
	//hRedis.excuteCommand("hset hash:1 name xiangzhou");
	//hRedis.excuteCommand("hset hash:1 gentle male");
	//hRedis.excuteCommand("hset hash:1 age 27");

	//hRedis.excuteCommand("hset hash:2 name haomeng");
	//hRedis.excuteCommand("hset hash:2 gentle male");
	//hRedis.excuteCommand("hset hash:2 age 28");
	
	//hRedis.excuteCommand("hset hash:3 name lutang");
	//hRedis.excuteCommand("hset hash:3 gentle female");
	//hRedis.excuteCommand("hset hash:3 age 24");
	
	//hRedis.excuteCommand("get test:2");
	hRedis.excuteCommand("smembers %s", "sqerqd");
	//hRedis.excuteCommand("hgetall hash:3");
	//hRedis.excuteCommand("hgetall hash:2");
	
	return 1;
}
