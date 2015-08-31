#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include <iostream>
#include <pthread.h>
#include "DataStruct.h"
//#include "ServiceModule.h"
#include "MySqlDB.h"
#include "ConfigSvr.h"
#include "RabbitMQClient.h"

typedef void*(*ThreadFunc)(void *arg);
class ServiceModule;

class WorkThread
{
typedef void*(*MemberThreadFunc)(WorkThread*);

public:
	WorkThread(ServiceModule *owner);
	~WorkThread();

	bool open();
	void close();
	pthread_t gettid();
	int create();
	void* threadCB();
	bool constructCommand(BaseCommand *pCommand, int iType);
	void executeCommand(BaseCommand *pCommand, int iType);
	void finalExecuteCommand(BaseCommand *pCommand, int iType);
public:
	ServiceModule *owner;
	pthread_t m_tid;
	MySqlDB	mySqlDB;
	Config m_cfg;
	
private:
	int isDBConnected();
	void reconnectDB();
	bool handleQueue(int iType);
	bool handleQuickQueue(BaseCommand *pCommand);
	bool handleNormalQueue(BaseCommand *pCommand);
	bool handleRetryQueue(BaseCommand *pCommand);
	void deserializeCommand(std::string sCmdStr, BaseCommand *pCommand);
	void executeQuickCommand(BaseCommand *pCommand);
	void executeNormalCommand(BaseCommand *pCommand);
	void executeRetryCommand(BaseCommand *pCommand);
	std::string readMQ(int iType);
	bool writeMQ(BaseCommand *pCommand, int iType);

	
private:
	RabbitMQWriter wRabbitMQ;
	RabbitMQReader rRabbitMQ;
  	struct epoll_event *events; 

};

#endif
