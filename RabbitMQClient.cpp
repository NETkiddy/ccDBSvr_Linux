#include "RabbitMQClient.h"
#include "define.h"

using namespace AmqpClient;

WriteRabbitMQ::WriteRabbitMQ(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame)
:	m_sHost(host), 
	m_iPort(port), 
	m_sUsername(username), 
	m_sPassword(password), 
	m_sVhost(vhost), 
	m_iMaxFrame(maxFrame) 
{
	if(EMPTY_STRING == m_sHost)
	{
		m_channel = Channel::Create();
	}
	else
	{
		char *localRabbit = const_cast<char*>(m_sHost.c_str());
		m_channel = Channel::Create(localRabbit);
	}

}

WriteRabbitMQ::~WriteRabbitMQ()
{
	//m_channel->close();
}

void WriteRabbitMQ::write(std::string sQueueName, std::string sContent)
{
	bool bRet = false;

	std::string sExchangeName = "amq.direct";
	std::string sRoutingKey = "key_zx";
	m_channel->DeclareQueue(sQueueName, /*passive*/false, /*durable*/true, /*exclusive*/false, /*auto_delete*/false);
    m_channel->DeclareExchange(sExchangeName, /*exchange_type*/Channel::EXCHANGE_TYPE_DIRECT, /*passive*/false, /*durable*/true, /*auto_delete*/false);
	m_channel->BindQueue(sQueueName, sExchangeName, sRoutingKey);
	BasicMessage::ptr_t msg_in = BasicMessage::Create();
	msg_in->Body(sContent);
	//设置消息的delivery_mode属性为2，因为队列和交换机都设置为durable的，这个设置将保证消息能够持久化，
	//也就是说，当它还没有送达消费者之前如果RabbitMQ重启则它能够被恢复
	msg_in->DeliveryMode(BasicMessage::dm_persistent);
    //Publishes a Basic message to an exchange
	m_channel->BasicPublish(sExchangeName, sRoutingKey, msg_in);
 
    std::cout<<"Message Write: "<<msg_in->Body()<<std::endl;

}

int WriteRabbitMQ::getMQLength(std::string sQueueName)
{
	return 0;
}

ReadRabbitMQ::ReadRabbitMQ(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame)
:	m_sHost(host), 
	m_iPort(port), 
	m_sUsername(username), 
	m_sPassword(password), 
	m_sVhost(vhost), 
	m_iMaxFrame(maxFrame) 
{
	if(EMPTY_STRING == m_sHost)
	{
		m_channel = Channel::Create();
	}
	else
	{
		char *localRabbit = const_cast<char*>(m_sHost.c_str());
		m_channel = Channel::Create(localRabbit);
	}

}

ReadRabbitMQ::~ReadRabbitMQ()
{
	//m_channel->close();
}

std::string ReadRabbitMQ::read(std::string sQueueName)
{
    //Starts consuming Basic messages on a queue
	m_channel->BasicConsume(sQueueName, "consumer_tag");
    //Consumes a single message, Waits for a single Basic message to be Delivered. 
	//This function only works after BasicConsume has successfully been called
    BasicMessage::ptr_t msg_out = m_channel->BasicConsumeMessage("consumer_tag")->Message();
 	
	std::string sMsg = msg_out->Body();
    std::cout<<"Message Received: "<<sMsg<<std::endl;
	return sMsg;
}


int ReadRabbitMQ::getMQLength(std::string sQueueName)
{
	return 0;
}
