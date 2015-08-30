#include <stdio.h>
#include "ServiceModule.h"
#include "ConfigSvr.cpp"
#include <exception>
#include "define.h"

ServiceModule::ServiceModule()
: m_tcpSvr(this)
{
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
}


bool ServiceModule::open()
{
    try
	{
		ConfigSvr::loadServiceOption(m_cfg);
		//start thread
		int threadCount = 3;
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
		m_tcpSvr.open();
	
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
	m_tcpSvr.close();

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

void ServiceModule::signalQueue(int cType)
{
	//TODO

}

bool ServiceModule::serializeCommand(BaseCommand *pCommand, std::string sCmdStr)
{

}


bool ServiceModule::serializeCommand(BaseCommand *pCommand, MsgData msgData)
{

}
