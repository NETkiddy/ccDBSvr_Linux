#include <stdio.h>
#include "ServiceModule.h"
#include "ConfigSvr.cpp"
#include <exception>
#include "define.h"
#include <unistd.h>
#include <stdlib.h>

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
		string sQuickPipeName = m_cfg[PIPE_NAMEPREFIX] + "0";
		string sNormalPipeName = m_cfg[PIPE_NAMEPREFIX] + "1";
		string sRetryPipeName = m_cfg[PIPE_NAMEPREFIX] + "2";
		if( 0 != m_QuickPipeWriter.open(sQuickPipeName, m_cfg[PIPE_MODE])
			|| 0 != m_NormalPipeWriter.open(sNormalPipeName, m_cfg[PIPE_MODE])
			|| 0 != m_RetryPipeWriter.open(sRetryPipeName, m_cfg[PIPE_MODE]) )
			return false;
		m_fdQuickPipe = m_QuickPipeWriter.getFd();
		m_fdNormalPipe = m_NormalPipeWriter.getFd();
		m_fdRetryPipe = m_RetryPipeWriter.getFd();
		m_eventQuick.data.fd = m_fdQuickPipe;
		m_eventNormal.data.fd = m_fdNormalPipe;
		m_eventRetry.data.fd = m_fdRetryPipe;
		m_eventQuick.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
		m_eventNormal.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
		m_eventRetry.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
		if (-1 == epoll_ctl(m_fdEpoll, EPOLL_CTL_ADD, m_fdQuickPipe, &m_eventQuick)  
			|| -1 == epoll_ctl(m_fdEpoll, EPOLL_CTL_ADD, m_fdNormalPipe, &m_eventNormal)
			|| -1 == epoll_ctl(m_fdEpoll, EPOLL_CTL_ADD, m_fdRetryPipe, &m_eventRetry))  
	    {  
	      std::cout<<"epoll_ctl error"<<std::endl;  
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
				//std::cout<<"WorhThread Open Success"<<std::endl;
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
		bRet = true;
	}
	else
	{
		std::cout<<"Quick Pipe Overflow..."<<std::endl;
	}

	pthread_mutex_unlock(&mutQuick);
	
	if(bRet)
		signalQueue(T_QUICK_QUEUE);
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
	int iRet = -1;
	//char iTag = 3;
	char iTag[1] = {'3'};

	if(T_QUICK_QUEUE == iType)
	{
		std::cout<<"Try Signal Quick"<<std::endl;
		iRet = m_QuickPipeWriter.write(iTag, 1);
	}
	else if(T_NORMAL_QUEUE == iType)
	{
		std::cout<<"Try Singnal Normal"<<std::endl;
		iRet = m_NormalPipeWriter.write(iTag, 1);
	}
	else if(T_RETRY_QUEUE == iType)
	{
		std::cout<<"Try Singnal Retry"<<std::endl;
		iRet = m_RetryPipeWriter.write(iTag, 1);
	}

	//std::cout<<"Pipe-"<<iType<<" write "<<iRet<<std::endl;
}

bool ServiceModule::serializeCommand(BaseCommand **ppCommand, std::string sCmdStr)
{
	std::cout<<"Serialize Message: "<<sCmdStr<<std::endl;
	size_t iPos = sCmdStr.find_first_of("@");
	if(iPos == string::npos)
	{
	    std::cout<<"@ not found, error message type"<<std::endl;
	}
	std::string sCmdID = sCmdStr.substr(0, iPos);
	std::string sContent = sCmdStr.substr(iPos + 1);
	
	iPos = sContent.find_first_of("@");
	if(iPos == string::npos)
	{
	    std::cout<<"@ not found, error message type"<<std::endl;
	}
	std::string sType = sContent.substr(0, iPos);
	sContent = sContent.substr(iPos + 1);
	
	MsgData msgData;
	char *tmp = const_cast<char*>(sCmdID.c_str());
	msgData.cCmdID = tmp[0];
	tmp = const_cast<char*>(sType.c_str());
	msgData.cType = tmp[0];
	msgData.sContent = sContent;

	return serializeCommand(ppCommand, msgData);
}


bool ServiceModule::serializeCommand(BaseCommand **ppCommand, MsgData msgData)
{
	*ppCommand = commandFactory.build(msgData.cCmdID, msgData.cType, msgData.sContent);
	if(nullptr == ppCommand)
	{
		ConfigSvr::log_error("serializeCommand, build error");
		return false;
	}

	return true;
}

bool ServiceModule::deserializeCommand(std::string &sCmdStr, BaseCommand *pCommand)
{
	bool bRet = false;
	do
	{
		if(!pCommand)
			break;
		std::string queryStr = ConfigSvr::charToStr(pCommand->m_cCmdID) + "@" + ConfigSvr::charToStr(pCommand->m_cType) + "@" + pCommand->m_sContent;
		sCmdStr = queryStr;
		bRet = true;
	}while(0);

	return bRet;
}
