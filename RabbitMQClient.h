#ifndef _RABBITMQ_CLIENT_H_
#define RABBITMQ_CLIENT_H_
#include <SimpleAmqpClient/SimpleAmqpClient.h>
 
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>

class RabbitMQWriter
{
public:
	RabbitMQWriter(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame);
	~RabbitMQWriter();

	void initial();
	void write(std::string sQueueName, std::string sContent);
	unsigned int getMQLength(std::string sQueueName);
	void close();

public:
	std::string m_sHost;
	int m_iPort;
	std::string m_sUsername;
	std::string m_sPassword;
	std::string m_sVhost;
	int m_iMaxFrame;
	
	std::string m_sExchangeName;
	std::string m_sRoutingKey;
	AmqpClient::Channel::ptr_t m_channel;
};

class RabbitMQReader
{
public:
	RabbitMQReader(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame);
	~RabbitMQReader();
	
	void initial();
	void doConsume(std::string sQueueName, int iTag);
	std::string read(std::string sQueueName, int iTag);
	unsigned int getMQLength(std::string sQueueName);
	void close();

public:
	std::string m_sHost;
	int m_iPort;
	std::string m_sUsername;
	std::string m_sPassword;
	std::string m_sVhost;
	int m_iMaxFrame;
	int m_iConsumerTag;
	AmqpClient::Channel::ptr_t m_channel;
	std::string m_sExchangeName;
	std::string m_sRoutingKey;
	std::map<std::string, bool> m_mapConsumed;
};



#endif


