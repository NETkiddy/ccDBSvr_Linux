#ifndef _RABBITMQ_CLIENT_H_
#define RABBITMQ_CLIENT_H_
#include <SimpleAmqpClient/SimpleAmqpClient.h>
 
#include <iostream>
#include <string>
#include <stdlib.h>

class RabbitMQWriter
{
public:
	RabbitMQWriter(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame);
	~RabbitMQWriter();

	void write(std::string sQueueName, std::string sContent);
	int getMQLength(std::string sQueueName);

public:
	std::string m_sHost;
	int m_iPort;
	std::string m_sUsername;
	std::string m_sPassword;
	std::string m_sVhost;
	int m_iMaxFrame;

	AmqpClient::Channel::ptr_t m_channel;
};

class RabbitMQReader
{
public:
	RabbitMQReader(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame);
	~RabbitMQReader();

	void initial(std::string sQueueName, int iTag);
	std::string read(std::string sQueueName, int iTag);
	int getMQLength(std::string sQueueName);

public:
	std::string m_sHost;
	int m_iPort;
	std::string m_sUsername;
	std::string m_sPassword;
	std::string m_sVhost;
	int m_iMaxFrame;

	AmqpClient::Channel::ptr_t m_channel;

};



#endif


