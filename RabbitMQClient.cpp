#include "RabbitMQClient.h"
#include "define.h"
#include "ConfigSvr.h"

using namespace AmqpClient;

RabbitMQWriter::RabbitMQWriter(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame)
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
	
	initial();
}

RabbitMQWriter::~RabbitMQWriter()
{
	//m_channel->close();
}

void RabbitMQWriter::initial()
{
	m_sExchangeName = "amq.direct";
	m_sRoutingKey = "key_zx";

}

void RabbitMQWriter::write(std::string sQueueName, std::string sContent)
{
	bool bRet = false;
	
	//delcare here in case of that: queue/exchange is deleted during running
	m_channel->DeclareQueue(sQueueName, /*passive*/false, /*durable*/true, /*exclusive*/false, /*auto_delete*/false);
    m_channel->DeclareExchange(m_sExchangeName, /*exchange_type*/Channel::EXCHANGE_TYPE_DIRECT, /*passive*/false, /*durable*/true, /*auto_delete*/false);
	m_channel->BindQueue(sQueueName, m_sExchangeName, m_sRoutingKey);
	
	BasicMessage::ptr_t msg_in = BasicMessage::Create();
	msg_in->Body(sContent);
	//设置消息的delivery_mode属性为2，因为队列和交换机都设置为durable的，这个设置将保证消息能够持久化，
	//也就是说，当它还没有送达消费者之前如果RabbitMQ重启则它能够被恢复
	msg_in->DeliveryMode(BasicMessage::dm_persistent);
    //Publishes a Basic message to an exchange
	m_channel->BasicPublish(m_sExchangeName, m_sRoutingKey, msg_in);
 
    std::cout<<"Message Write: "<<msg_in->Body()<<std::endl;

}

int RabbitMQWriter::getMQLength(std::string sQueueName)
{
	return 0;
}

RabbitMQReader::RabbitMQReader(std::string host, int port, std::string username, std::string password, std::string vhost, int maxFrame)
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

	initial();
}

RabbitMQReader::~RabbitMQReader()
{	
	close();
	//m_channel->close();
}

void RabbitMQReader::initial()
{
	m_sExchangeName = "amq.direct";
	m_sRoutingKey = "key_zx";

}

void RabbitMQReader::doConsume(std::string sQueueName, int iTag)
{
	m_iConsumerTag = iTag;
    //Starts consuming Basic messages on a queue
	m_channel->BasicConsume(sQueueName , CONSUMER_PREFIX + ConfigSvr::intToStr(m_iConsumerTag), /*no_local*/true, /*no_ack*/true, /*exclusive*/false);
	std::cout<<"Create Consumer success: "<<CONSUMER_PREFIX + ConfigSvr::intToStr(m_iConsumerTag)<<std::endl;
}

void RabbitMQReader::close()
{
	std::map<std::string, bool>::iterator iterConsumed = m_mapConsumed.begin();
	for(;iterConsumed != m_mapConsumed.end(); ++iterConsumed)
	{
		//cancel creadted consumer
		//TODO, m_channel->BasicCancel(CONSUMER_PREFIX + ConfigSvr::intToStr(m_iConsumerTag));
		//m_mapConsumed[sQueueName] = false;

	}

}

std::string RabbitMQReader::read(std::string sQueueName, int iTag)
{
	//first, do consume
	if(m_mapConsumed.find(sQueueName) != m_mapConsumed.end())
	{
		if(false == m_mapConsumed[sQueueName])
		{
			doConsume(sQueueName, iTag);
			m_mapConsumed[sQueueName] = true;
		}
	}
	else
	{
		doConsume(sQueueName, iTag);
		m_mapConsumed[sQueueName] = true;
	}
	
	//second, delcare here in case of that: queue/exchange is deleted during running
	m_channel->DeclareQueue(sQueueName, /*passive*/false, /*durable*/true, /*exclusive*/false, /*auto_delete*/false);
    m_channel->DeclareExchange(m_sExchangeName, /*exchange_type*/Channel::EXCHANGE_TYPE_DIRECT, /*passive*/false, /*durable*/true, /*auto_delete*/false);
	m_channel->BindQueue(sQueueName, m_sExchangeName, m_sRoutingKey);

    //third, Consumes a single message, Waits for a single Basic message to be Delivered. 
	//This function only works after BasicConsume has successfully been called
    BasicMessage::ptr_t msg_out = m_channel->BasicConsumeMessage(CONSUMER_PREFIX + ConfigSvr::intToStr(iTag))->Message();
	std::string sMsg = msg_out->Body();
    
	std::cout<<sQueueName<<", Message Received: "<<sMsg<<std::endl;
	return sMsg;
}


int RabbitMQReader::getMQLength(std::string sQueueName)
{
	return 0;
}
