#ifndef _SERVICE_MODULE_H_
#define _SERVICE_MODULE_H_

#include <stdio.h>
#include <vector>
#include "WorkThread.h"
#include "TcpSvr.h"
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

typedef std::map<string, string> MsgData;

class ServiceModule
{
public:
	ServiceModule();
	~ServiceModule();
	
	bool open();
	void close();
	void pushMessage(MsgData msgData);
public:
	std::vector<WorkThread*> m_vecWT;
	TcpSvr m_tcpSvr;
	std::vector<MsgData>	m_vecQuickPipe;
	pthread_mutex_t mutQuick = PTHREAD_MUTEX_INITIALIZER;
	Config m_cfg;
	//	io_service ios;
};

#endif
