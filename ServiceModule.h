#ifndef _SERVICE_MODULE_H_
#define _SERVICE_MODULE_H_

#include <stdio.h>
#include <vector>
#include <deque>
#include "WorkThread.h"
#include "DataStruct.h"
#include "TcpSvr.h"
#include "PipeClient.h"
#include "CommandEntity.h"
#include "Factory.h"
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>



class ServiceModule
{
public:
	ServiceModule();
	~ServiceModule();
	
	bool open();
	void close();
	bool pushMessage(MsgData msgData);
	bool popMessage(MsgData &msgData);
	void signalQueue(int cType);
	bool serializeCommand(BaseCommand **pCommand, std::string sCmdStr);
	bool serializeCommand(BaseCommand **pCommand, MsgData msgData);
	bool deserializeCommand(std::string sCmdStr, BaseCommand *pCommand);
public:
	std::vector<WorkThread*> m_vecWT;
	TcpSvr *m_tcpSvr;
	std::deque<MsgData>	m_deQuickQueue;
	pthread_mutex_t mutQuick = PTHREAD_MUTEX_INITIALIZER;
	Config m_cfg;
	//	io_service ios;
	int m_fdEpoll;
  	int m_fdQuickPipe;
  	int m_fdNormalPipe;
  	int m_fdRetryPipe;
	PipeWriter m_QuickPipeWriter;
	PipeWriter m_NormalPipeWriter;
	PipeWriter m_RetryPipeWriter;
	std::string m_sPipeName;
  	struct epoll_event m_eventQuick;  
  	struct epoll_event m_eventNormal;  
  	struct epoll_event m_eventRetry;  
private:
	CommandFactory commandFactory;
};

#endif
