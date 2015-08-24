#include <pthread.h>
#include "WorkThread.h"
#include "ServiceModule.h"
#include <string>
#include <sstream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void* threadCb(void *arg);


WorkThread::WorkThread(ServiceModule *owner)
: mySqlDB(owner->m_cfg)
, m_cfg(owner->m_cfg)
{
}


WorkThread::~WorkThread()
{
}


bool WorkThread::open()
{
	int iRet = create(this, threadCB);
	if(iRet == 0)
	{
		//int  itmp = m_tid;
		stringstream ssTmp;
		ssTmp<<m_tid;
		std::cout<<"Thread Create Success, ID: "<<ssTmp.str()<<std::endl;
		return true;
	}

	std::cout<<"Thread Create Failed"<<std::endl;
	return false;
}

void WorkThread::close()
{
	if(pthread_cancel(m_tid) == 0)
	{
		//int itmp = m_tid;
		stringstream ssTmp;
		ssTmp<<m_tid;
		
		if(pthread_join(m_tid, NULL) != 0)
		{
			std::cout<<"Thread Join Error, ID: " + ssTmp.str()<<std::endl;
		}
		else
		{
			std::cout<<"Thread Join Success, ID: " + ssTmp.str()<<std::endl;
		}
	}
	else
	{
		std::cout<<"Send Calcel Failed"<<std::endl;
	}
}

pthread_t WorkThread::gettid()
{
	return m_tid;
}	

int WorkThread::create(WorkThread *wt, ThreadFunc func)
{
	if(pthread_create(&(wt->m_tid), NULL, func, this) != 0)
	{
		//std::cout<<"Create Thread Error!"<<std::endl;
		return -1;
	}

	//printf("TID in pthread_create function: %u.\n",tid);
	//stringstream ssTmp;
	//ssTmp<<gettid();
	std::cout<<"Pthread create success"<<std::endl; 
	
	sleep(1);
	return 0;

}

void* WorkThread::threadCB(void *arg)
{
	WorkThread *pWT = static_cast<WorkThread*>(arg);
	Config *cfg = &(pWT->m_cfg);
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); // 设置其他线程可以cancel掉此线程
	
	std::string queryStr = "select * from siccdb.UserInfo";
	
	while(1)
	{
		sleep(60);
		std::cout<<"thread running"<<std::endl;
		pWT->mySqlDB.open();
		pWT->mySqlDB.execute(queryStr);
		//stringstream ssTmp;
		//ssTmp<<gettid();
		//std::cout<<"TID:c" +  ssTmp.str()<<std::endl;; //why pthread_self		
		//printf("TID: %u.\n", tid); //why pthread_self
	}
	pthread_exit(NULL); //退出线程a
}


