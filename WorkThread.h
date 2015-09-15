#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include <iostream>
#include <pthread.h>
#include "DataStruct.h"
//#include "ServiceModule.h"
#include "MySqlDB.h"
#include "ConfigSvr.h"
#include "RabbitMQClient.h"
#include "PipeClient.h"

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
	bool constructCommand(BaseCommand **ppCommand, int iType);
	void executeCommand(BaseCommand *pCommand, int iType);
	void finalExecuteCommand(BaseCommand *pCommand, int iType);
public:
	ServiceModule *owner;
	pthread_t m_tid;
	//MySqlDB	mySqlDB;
	BaseDB_sPtr m_spBaseDB;
	Config m_cfg;
	
private:
	bool isDBConnected();
	void reconnectDB();
	bool handleQueue(int iType);
	bool handleQuickQueue(BaseCommand **ppCommand);
	bool handleNormalQueue(BaseCommand **ppCommand);
	bool handleRetryQueue(BaseCommand **ppCommand);
	void deserializeCommand(std::string sCmdStr, BaseCommand *pCommand);
	void executeQuickCommand(BaseCommand *pCommand);
	void executeNormalCommand(BaseCommand *pCommand);
	void executeRetryCommand(BaseCommand *pCommand);
	std::string readMQ(int iType);
	bool writeMQ(BaseCommand *pCommand, int iType);
	bool openDB();
	
private:
	RabbitMQWriter wRabbitMQ;
	RabbitMQReader rRabbitMQ;
  	struct epoll_event *m_events; 
	PipeReader m_QuickPipeReader;

};

#endif
