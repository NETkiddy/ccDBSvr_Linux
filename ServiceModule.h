#ifndef _SERVICE_MODULE_H_
#define _SERVICE_MODULE_H_

#include <stdio.h>
#include <vector>
#include "WorkThread.h"
#include "TcpSvr.h"
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
public:
	std::vector<WorkThread*> m_vecWT;
	TcpSvr m_tcpSvr;
	std::vector<MsgData>	m_vecQuickPipe;
	pthread_mutex_t mutQuick = PTHREAD_MUTEX_INITIALIZER;

	//	io_service ios;
};

#endif
