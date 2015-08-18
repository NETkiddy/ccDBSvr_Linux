#include <stdio.h>
#include <stdlib.h>
#include "CoreApp.h"
#include <vector>

using namespace std;

CoreApp::CoreApp()
{
	
}

CoreApp::~CoreApp()
{
	stop();

	std::vector<ServiceModule*>::iterator iterSM = m_vecSM.begin();
	for(;iterSM != m_vecSM.end(); ++iterSM)
	{
		delete (*iterSM);
		m_vecSM.erase(iterSM++);
	}
}
void CoreApp::start()
{
	//open all sm
	int iCountSM = 1;
	for(int i = 0; i < iCountSM; ++i)
	{
	    ServiceModule *sm = new ServiceModule();
		if(sm->open())
			m_vecSM.push_back(sm);
	}

	//
	
}

void CoreApp::stop()
{
	std:vector<ServiceModule*>::iterator iterSM = m_vecSM.begin();
	for(;iterSM != m_vecSM.end(); ++iterSM)
	{
		(*iterSM)->close();
	}
	std::cout<<"CoreApp Stopped"<<std::endl;
}

