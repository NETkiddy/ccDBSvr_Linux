#include <pthread.h>
#include "WorkThread.h"
#include "ServiceModule.h"
#include <string>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include "define.h"

using namespace std;

void* threadCb(void *arg);


WorkThread::WorkThread(ServiceModule *owner)
: owner(owner),
  mySqlDB(owner->m_cfg),
  m_cfg(owner->m_cfg),
  rRabbitMQ(m_cfg[RABBITMQ_HOST], std::stoi(m_cfg[RABBITMQ_PORT]), m_cfg[RABBITMQ_USERNAME], m_cfg[RABBITMQ_PASSWORD], m_cfg[RABBITMQ_VHOST],std::stoi(m_cfg[RABBITMQ_MAXFRAME])),
  wRabbitMQ(m_cfg[RABBITMQ_HOST], std::stoi(m_cfg[RABBITMQ_PORT]), m_cfg[RABBITMQ_USERNAME], m_cfg[RABBITMQ_PASSWORD], m_cfg[RABBITMQ_VHOST], std::stoi(m_cfg[RABBITMQ_MAXFRAME]))
{
}


WorkThread::~WorkThread()
{
}


bool WorkThread::open()
{
	int iRet = create();
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

int WorkThread::create()
{
	union {// 联合类，用于转换类成员方法指针到普通函数指针（试过编译器不允许在这两种函数之间强制转换），不知道有没有更好的方法。
		void* (*aliasCB)(void *);
		void* (WorkThread::*memberCB)();
	} SwitchProc;                                // 尽管联合里的两种函数类型现在看起来有很大不同，但它们的最终形式是相同的。
	SwitchProc.memberCB = &WorkThread::threadCB;   // 转换，SwitchProc.aliasCB就是对应的普通函数指针了
	
	if(pthread_create(&(this->m_tid), NULL, SwitchProc.aliasCB, this) != 0)
	{
	//	std::cout<<"Create Thread Error"<<std::endl;
		return -1;
	}

	//printf("TID in pthread_create function: %u.\n",tid);
	//stringstream ssTmp;
	//ssTmp<<gettid();
	//std::cout<<"Create Thread Success"<<std::endl; 
	
	sleep(1);
	return 0;

}

void* WorkThread::threadCB()
{
	//WorkThread *pThis = static_cast<WorkThread*>(arg);
	Config *cfg = &m_cfg;
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); // 设置其他线程可以cancel掉此线程
	
	int iType = T_QUICK_QUEUE;

	mySqlDB.open();
  	
  	m_events = (epoll_event*)calloc(EPOLL_MAX_EVENTS, sizeof(m_events[0]));
	
	while(1)
	{
		sleep(60);
		std::cout<<"thread running"<<std::endl;

		MsgData msgData;
		int iRet = -1;
		int iWorkType;
		int iDBState = isDBConnected();
		if(-1 == iDBState)
		{
			reconnectDB();
			continue;
		}

		if(0 == iDBState)
		{
			//force heartbeat msgdata
		}
		else if(1 == iDBState)
		{
			int iEventCount = epoll_wait(owner->m_fdEpoll, m_events, EPOLL_MAX_EVENTS, 3000);
			if(0 == iEventCount)
			{
				//Timeout
				std::cout<<"SIGNAL_HEARTBEAT"<<std::endl;
				iWorkType = SIGNAL_HEARTBEAT;

			}
			else
			{
				for(int i = 0; i < iEventCount; ++i)
				{
					if ((m_events[i].events & EPOLLERR) ||  
              			(m_events[i].events & EPOLLHUP) ||  
              			(!(m_events[i].events & EPOLLIN)))  
					{
						std::cout<<"epoll_wait error"<<std::endl;
              			::close(m_events[i].data.fd); 
              			continue;
					}
					else if(owner->m_fdNormalPipe == m_events[i].data.fd)
					{
						std::cout<<"SIGNAL_NORMAL"<<std::endl;
						iWorkType = SIGNAL_NORMAL;
					}
					else if(owner->m_fdRetryPipe == m_events[i].data.fd)
					{
						std::cout<<"SIGNAL_RETRY"<<std::endl;
						iWorkType = SIGNAL_RETRY;
					}
				}
			}
	
		}
		
		BaseCommand *pCommand = NULL;
		constructCommand(pCommand, iType);
		if(!pCommand)
		{
			std::cout<<"Construct Command Failed"<<std::endl;
			continue;
		}

		executeCommand(pCommand, iType);
		finalExecuteCommand(pCommand, iType);
		
		
	}
	pthread_exit(NULL); //退出线程a
}

int WorkThread::isDBConnected()
{
	return 1;
}

void WorkThread::reconnectDB()
{
}

bool WorkThread::constructCommand(BaseCommand *pCommand, int iType)
{
	bool bRet = false;
	
	if(!pCommand)
	{
		std::cout<<"executeCommand Failed: Null Command"<<std::endl;
		return bRet;
	}

	switch(iType)
	{
		case T_QUICK_QUEUE:
		{
			bRet = handleQuickQueue(pCommand);
			break;
		}
		case T_NORMAL_QUEUE:
		{
			bRet = handleNormalQueue(pCommand);
			break;
		}
		case T_RETRY_QUEUE:
		{
			bRet = handleRetryQueue(pCommand);
			break;
		}
	}

	return bRet;
}

void WorkThread::executeCommand(BaseCommand *pCommand, int iType)
{
	if(!pCommand)
	{
		std::cout<<"executeCommand Failed: Null Command"<<std::endl;
		return;
	}

	switch(iType)
	{
		case T_QUICK_QUEUE:
		{
			executeQuickCommand(pCommand);
			break;
		}
		case T_NORMAL_QUEUE:
		{
			executeNormalCommand(pCommand);
			break;
		}
		case T_RETRY_QUEUE:
		{
			executeRetryCommand(pCommand);
			break;	
		}
	}



}

void WorkThread::finalExecuteCommand(BaseCommand *pCommand, int iType)
{
	
}

bool WorkThread::handleQuickQueue(BaseCommand *pCommand)
{
     bool bRet = false;

	 //read qucik Queue
	 MsgData msgData;
     bRet = owner->popMessage(msgData);
	 if(bRet)
	 {
	 	bRet = owner->serializeCommand(pCommand, msgData);
     }
	 
	 return bRet;
}

bool WorkThread::handleNormalQueue(BaseCommand *pCommand)
{
	bool bRet = false;

	std::string sCmdStr = readMQ(T_NORMAL_QUEUE);
	if(sCmdStr.empty())
	{
		std::cout<<"Read Normal Queue Empty"<<std::endl;
		return bRet;
	}
	
	bRet = owner->serializeCommand(pCommand, sCmdStr);
	return bRet;
}

bool WorkThread::handleRetryQueue(BaseCommand *pCommand)
{
	bool bRet = false;
	std::string sCmdStr = readMQ(T_RETRY_QUEUE);
	if(sCmdStr.empty())
	{
		std::cout<<"Read Retry Queue Empty"<<std::endl;
		return bRet;
	}
	
	bRet = owner->serializeCommand(pCommand, sCmdStr);
	return bRet;
}

void WorkThread::executeQuickCommand(BaseCommand *pCommand)
{
	if('R' == pCommand->cType)
	{
		std::string queryStr = "select * from siccdb.UserInfo";
		if(mySqlDB.execute(queryStr))
		{
			std::cout<<"execute success"<<std::endl;
		}
		else
		{
			std::cout<<"execute failed"<<std::endl;
		}
	}
	if('W' == pCommand->cType)
	{
		if(writeMQ(pCommand, T_NORMAL_QUEUE))
		{
			std::cout<<"WriteMQ to Normal Success, CmdID "<<pCommand->cCmdID<<std::endl;
		}
		else
		{
			std::cout<<"WriteMQ to Normal Failed, CmdID "<<pCommand->cCmdID<<std::endl;
		}
	}

}


void WorkThread::executeNormalCommand(BaseCommand *pCommand)
{
	std::string queryStr = "select * from siccdb.UserInfo";
	if(mySqlDB.execute(queryStr))
	{
		std::cout<<"execute success on normal"<<std::endl;
	}
	else
	{
		std::cout<<"execute failed on normal, writeMQ to Retry"<<std::endl;
		if(writeMQ(pCommand, T_RETRY_QUEUE))
		{
			std::cout<<"WriteMQ to Retry Success, CmdID "<<pCommand->cCmdID<<std::endl;
		}
		else
		{
			std::cout<<"WriteMQ to Retry Failed, CmdID "<<pCommand->cCmdID<<std::endl;
		}
	}

}

void WorkThread::executeRetryCommand(BaseCommand *pCommand)
{
	std::string queryStr = "select * from siccdb.UserInfo";
	if(mySqlDB.execute(queryStr))
	{
		std::cout<<"execute success on retry"<<std::endl;
	}
	else
	{
		//TODO, send mail log
		std::cout<<"execute failed on retry"<<std::endl;
	}

}


bool WorkThread::writeMQ(BaseCommand *pCommand, int iType)
{
	if(!pCommand)
		return false;

	std::string sCmdStr = EMPTY_STRING;
	deserializeCommand(sCmdStr, pCommand);
	
	std::string sRabbitQueueName = "mod-1";
	wRabbitMQ.write(sRabbitQueueName, sCmdStr);

}

std::string WorkThread::readMQ(int iType)
{
	std::string sRabbitQueueName = "mod-1";
	std::string sCmdStr = rRabbitMQ.read(sRabbitQueueName);

	return sCmdStr;
}

void WorkThread::deserializeCommand(std::string sCmdStr, BaseCommand *pCommand)
{
	if(!pCommand)
		return;
	std::string queryStr = "select * from siccdb.UserInfo";
	sCmdStr = queryStr;
}
