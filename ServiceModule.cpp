#include <stdio.h>
#include "ServiceModule.h"
#include "ConfigSvr.cpp"
#include <exception>
#include "define.h"

ServiceModule::ServiceModule()
{
	m_fdEpoll = epoll_create1(0);
	if(-1 == m_fdEpoll)
	{
		perror("epoll_create1");
		abort();
	}

}


ServiceModule::~ServiceModule()
{
	std::vector<WorkThread*>::iterator iterWT = m_vecWT.begin();
	for(;iterWT != m_vecWT.end(); ++iterWT)
	{
		delete (*iterWT);
		m_vecWT.erase(iterWT);
		std::cout<<"Service Module DeConstructed!"<<std::endl; 
	}

	if(m_tcpSvr)
		delete m_tcpSvr;
	m_tcpSvr = nullptr;
}


bool ServiceModule::open()
{
    try
	{
		ConfigSvr::loadServiceOption(m_cfg);

		//start pipe
		string sNormalPipeName = m_cfg[PIPE_NAMEPREFIX] + "0";
		string sRetryPipeName = m_cfg[PIPE_NAMEPREFIX] + "1";
		if( 0 != m_NormalPipeWriter.open(sNormalPipeName, m_cfg[PIPE_MODE])
			|| 0 != m_RetryPipeWriter.open(sRetryPipeName, m_cfg[PIPE_MODE]) )
			return false;
		int m_fdNormalPipe = m_NormalPipeWriter.getFd();
		int m_fdRetryPipe = m_RetryPipeWriter.getFd();
		eventNormal.data.fd = m_fdNormalPipe;
		eventRetry.data.fd = m_fdRetryPipe;
		eventNormal.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
		eventRetry.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
		if (-1 == epoll_ctl(m_fdEpoll, EPOLL_CTL_ADD, m_fdNormalPipe, &eventNormal)
			|| -1 == epoll_ctl(m_fdEpoll, EPOLL_CTL_ADD, m_fdRetryPipe, &eventRetry))  
	    {  
	      perror ("epoll_ctl");  
	      abort ();  
	    }  

		//start thread
		int threadCount = std::stoi(m_cfg[THREADPOOL_COUNT]);
		for(int i = 0; i < threadCount; ++i)
		{
			WorkThread *wt = new WorkThread(this);
			if(wt->open())
			{
				m_vecWT.push_back(wt);		
				std::cout<<"WorhThread Open Success"<<std::endl;
			}
		}	
		
		//start TcpSvr
		m_tcpSvr = new TcpSvr(this);
		m_tcpSvr->open();

		return true;
	}
	catch(std::exception &e)
	{
		std::cout<<"Service Module Open Failed, "<<e.what()<<std::endl;	
	}
	return false;
}

void ServiceModule::close()
{
	//close thread
	std::vector<WorkThread*>::iterator iterWT = m_vecWT.begin();
	for(;iterWT != m_vecWT.end(); ++iterWT)
	{
		(*iterWT)->close();
	}
	
	//close TcpSvr
	m_tcpSvr->close();

	std::cout<<"Service Module Closed"<<std::endl;
}

bool ServiceModule::pushMessage(MsgData msgData)
{
	bool bRet = false;

	pthread_mutex_lock(&mutQuick);
	//check queue length
	if(m_deQuickQueue.size() <= MAX_QUICK_SIZE)
	{
		m_deQuickQueue.push_back(msgData);
		signalQueue(T_QUICK_QUEUE);
		bRet = true;
	}
	else
	{
		std::cout<<"Quick Pipe Overflow..."<<std::endl;
	}

	pthread_mutex_unlock(&mutQuick);
	return bRet;
}

bool ServiceModule::popMessage(MsgData &msgData)
{
	bool bRet = false;

	pthread_mutex_lock(&mutQuick);
	if(m_deQuickQueue.size() > 0)
	{
		msgData = m_deQuickQueue.front();
		m_deQuickQueue.pop_front();	
		bRet = true;
	}
	pthread_mutex_unlock(&mutQuick);

	return bRet;
}

void ServiceModule::signalQueue(int iType)
{
	PipeData ppdata;
	ppdata.iTag = 1;

	if(T_NORMAL_QUEUE == iType)
	{
		m_NormalPipeWriter.write(&ppdata, 1);
	}
	else if(T_RETRY_QUEUE == iType)
	{
		m_RetryPipeWriter.write(&ppdata, 1);
	}
}

bool ServiceModule::serializeCommand(BaseCommand *pCommand, std::string sCmdStr)
{
	return true;
}


bool ServiceModule::serializeCommand(BaseCommand *pCommand, MsgData msgData)
{
	pCommand = new CommandFactory();
	return true;
}
