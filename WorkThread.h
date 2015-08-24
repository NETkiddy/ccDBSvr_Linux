#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include <iostream>
#include <pthread.h>
//#include "ServiceModule.h"
#include "MySqlDB.h"
#include "ConfigSvr.h"


typedef void *(*ThreadFunc)(void *arg);
class ServiceModule;

class WorkThread
{
public:
	WorkThread(ServiceModule *owner);
	~WorkThread();

	bool open();
	void close();
	pthread_t gettid();
	int create(WorkThread *wt, ThreadFunc func);
	static void* threadCB(void*);
public:
	pthread_t m_tid;
	MySqlDB	mySqlDB;
	Config m_cfg;
	
private:
	


};

#endif
