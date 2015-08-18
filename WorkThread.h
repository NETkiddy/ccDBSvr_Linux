#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include <iostream>
#include <pthread.h>
#include "MySqlDB.h"
#include "ConfigSvr.h"


typedef void *(*ThreadFunc)(void *arg);

class WorkThread
{
public:
	WorkThread(Config cfg);
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
