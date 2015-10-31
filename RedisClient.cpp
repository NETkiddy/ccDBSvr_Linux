#include "RedisClient.h"

RedisClient::RedisClient(std::string ip, int port)
: m_sIP(ip)
, m_iPort(port)
, pContext(nullptr)
{

}

RedisClient::~RedisClient()
{

}

bool RedisClient::connect()
{
	pContext = redisConnect(m_sIP.c_str(), m_iPort);
	if(pContext->err)
	{
		std::cout<<"Connect redis failed, error: "<<pContext->err<<std::endl;
		return false;
	}
	std::cout<<"Connect redis success!"<<std::endl;
	return true;
}
/*
RedisClientReply RedisClient::executeCommand(std::string commStr)
{
	reply = static_cast<redisReply*>(redisCommand(pContext, commStr.c_str()));
	
	if(REDIS_REPLY_INTEGER == reply->type || REDIS_REPLY_STRING == reply->type)
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

RedisClientReply RedisClient::executeCommand(std::string commandStr, std::string param1)
{	
	reply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str(), param1.c_str()));
	
	redisClientReply.type = reply->type;

	if(REDIS_REPLY_INTEGER == reply->type)
	{
		redisClientReply.iValue = std::stoi(reply->str);
		std::cout<<reply->str<<std::endl;
	}

	if(REDIS_REPLY_STRING == reply->type)
	{
		redisClientReply.sValue = reply->str;
		std::cout<<reply->str<<std::endl;
	}

	if(REDIS_REPLY_ARRAY == reply->type)
	{
		for(int i = 0; i < reply->elements; ++i)
		{
			vecValue.push_back(reply->element[i]->str);
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
*/
RedisClientReply RedisClient::executeCommand(std::string commandStr, int iCount, std::string param1, std::string param2, std::string param3, std::string param4)
{
	switch(iCount)
	{
		case 0:
		{
			pReply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str()));
			break;
		}
		case 1:
		{
			pReply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str(), param1.c_str()));
			break;
		}
		case 2:
		{
			pReply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str(), param1.c_str(), param2.c_str()));
			break;
		}
		case 3:
		{
			pReply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str(), param1.c_str(), param2.c_str(), param3.c_str()));
			break;
		}
		case 4:
		{
			pReply = static_cast<redisReply*>(redisCommand(pContext, commandStr.c_str(), param1.c_str(), param2.c_str(), param2.c_str(), param4.c_str()));
			break;
		}
	}
	
	redisClientReply.iType = pReply->type;

	if(REDIS_REPLY_INTEGER == pReply->type)
	{
		redisClientReply.iValue = pReply->integer;
		std::cout<<pReply->integer<<std::endl;
	}

	if(REDIS_REPLY_STRING == pReply->type)
	{
		redisClientReply.sValue = pReply->str;
		std::cout<<pReply->str<<std::endl;
	}

	if(REDIS_REPLY_ARRAY == pReply->type)
	{
		for(int i = 0; i < pReply->elements; ++i)
		{
			redisClientReply.vecValue.push_back(pReply->element[i]->str);
			std::cout<<pReply->element[i]->str<<std::endl;
		}
	}
	if(REDIS_REPLY_NIL == pReply->type)
	{
		std::cout<<"not a answer"<<std::endl;
	}
	if(REDIS_REPLY_ERROR == pReply->type)
	{
		std::cout<<"error"<<std::endl;
	}
 
	freeReplyObject(pReply);

	return redisClientReply;
}
