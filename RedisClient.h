#ifndef _REDIS_CLIENT_H_
#define _REDIS_CLIENT_H_

#include <iostream>
#include <hiredis/hiredis.h>
#include <vector>
#include <string>

struct RedisClientReply
{
	int iType;
	int	iValue;
	std::string sValue;
	std::vector<std::string> vecValue;	
};

class RedisClient
{
public:
	RedisClient(std::string ip, int port);
	~RedisClient();

	bool connect();
	//RedisClientReply executeCommand(std::string commStr);
	//RedisClientReply executeCommand(std::string commStr, std::string param1);
	//RedisClientReply executeCommand(std::string commStr, std::string param1, std::string param2);
	RedisClientReply executeCommand(std::string commStr, int iCount=0, std::string param1="0x00", std::string param2="0x00", std::string param3="0x00", std::string param4="0x00");

public:
	std::string m_sIP;
	int m_iPort;
	RedisClientReply redisClientReply;

private:
	redisContext *pContext;
	redisReply	*pReply;
};


#endif
