#include <stdio.h>
#include "ServiceModule.h"
#include "ConfigSvr.cpp"
#include <exception>

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

void ServiceModule::pushMessage(MsgData msgData)
{
	pthread_mutex_lock(&mutQuick);
	m_vecQuickPipe.push_back(msgData);
	pthread_mutex_unlock(&mutQuick);
}
