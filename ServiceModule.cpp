#include <stdio.h>
#include "ServiceModule.h"
#include "ConfigSvr.cpp"


ServiceModule::ServiceModule()
: m_tcpSvr()
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
	Config cfg;
	ConfigSvr::loadServiceOption(cfg);

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
	m_tcpSvr.open(this);
	
	return true;
	}
	catch(...)
	{
		std::cout<<"Service Module Open Failed"<<std::endl;	
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
