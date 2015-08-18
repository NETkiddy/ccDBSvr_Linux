#ifndef _CORE_APP_H_
#define _CORE_APP_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
//#include "TcpSvr.h"
#include "ServiceModule.h"

using namespace std;

class CoreApp
{
public:
	//TcpSvr tcpSvr;
	std::vector<ServiceModule*> m_vecSM;

public:
	CoreApp();
	~CoreApp();

	void start();
	void stop();

};

#endif
