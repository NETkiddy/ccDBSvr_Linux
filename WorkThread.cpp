#include <pthread.h>
#include "WorkThread.h"
#include "ServiceModule.h"
#include "ConfigSvr.h"
#include <string>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include "define.h"

using namespace std;


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
	if(0 == create())
	{
		std::cout<<"Thread Create Success, ID: "<<ConfigSvr::intToStr(m_tid)<<std::endl;
		return true;
	}

	std::cout<<"Thread Create Failed"<<std::endl;
	return false;
}

void WorkThread::close()
{
	if(pthread_cancel(m_tid) == 0)
	{
		if(pthread_join(m_tid, NULL) != 0)
		{
			std::cout<<"Thread Join Error, ID: " + ConfigSvr::intToStr(m_tid)<<std::endl;
		}
		else
		{
			std::cout<<"Thread Join Success, ID: " + ConfigSvr::intToStr(m_tid)<<std::endl;
		}
	}
	else
	{
		std::cout<<"Send Cancel Failed"<<std::endl;
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
		return -1;
	}

	return 0;

}

void* WorkThread::threadCB()
{
	Config *cfg = &m_cfg;
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); // 设置其他线程可以cancel掉此线程
	
	int iType = T_QUICK_QUEUE;

	if(!mySqlDB.open())
  	{
		pthread_exit(NULL);
	}

	m_events = (epoll_event*)calloc(EPOLL_MAX_EVENTS, sizeof(m_events[0]));
	while(1)
	{
		std::cout<<"thread running"<<std::endl;

		MsgData msgData;
		int iRet = -1;
		int iWorkType;
		bool bDBState = isDBConnected();
		if(false == iDBState)
		{
			reconnectDB();
			continue;
		}
		else if(true == iDBState)
		{
			int iEventCount = epoll_wait(owner->m_fdEpoll, m_events, EPOLL_MAX_EVENTS, std::stoi(m_cfg[THREADPOOL_TIMEOUT]));
			if(0 == iEventCount)
			{
				//Timeout
				std::cout<<"SIGNAL_HEARTBEAT"<<std::endl;
				iWorkType = SIGNAL_HEARTBEAT;

			}
			else
			{
				std::cout<<"epoll got"<<std::endl;
				for(int i = 0; i < iEventCount; ++i)
				{//TODO, for'S BUG ??
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

	mySqlDB.close();
	pthread_exit(NULL); //退出线程a
}

bool WorkThread::isDBConnected()
{
	return mysqlDB.isConnected();
}

void WorkThread::reconnectDB()
{
	mysqlDB.reconnect();
}

bool WorkThread::constructCommand(BaseCommand *pCommand, int iType)
{
	bool bRet = false;
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
		std::string queryStr = pCommand->sContent;//"select * from siccdb.UserInfo";
		if(mySqlDB.execute(queryStr))
		{
			std::cout<<"execute finish success"<<std::endl;
		}
		else
		{
			std::cout<<"execute finish failed"<<std::endl;
		}
	}
	if('W' == pCommand->cType)
	{
		if(writeMQ(pCommand, T_NORMAL_QUEUE))
		{
			std::cout<<"WriteMQ to Normal Success, CmdID "<<pCommand->cCmdID<<std::endl;
			owner->signalQueue(T_NORMAL_QUEUE);
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
			owner->signalQueue(T_RETRY_QUEUE);
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
	bool bRet = false;
	do
	{
		if(!pCommand)
			break;

		std::string sCmdStr = EMPTY_STRING;
		if(!owner->deserializeCommand(sCmdStr, pCommand))
			break;

		std::string sRabbitQueueName = m_cfg[RABBITMQ_QUEUENAME_PRIFIX] + ConfigSvr::intToStr(iType);
		wRabbitMQ.write(sRabbitQueueName, sCmdStr);
		bRet = true;
	}while(0);

	return bRet;
}

std::string WorkThread::readMQ(int iType)
{
	std::string sRabbitQueueName = m_cfg[RABBITMQ_QUEUENAME_PRIFIX] + ConfigSvr::intToStr(iType);
	std::string sCmdStr = "";//rRabbitMQ.read(sRabbitQueueName);

	return sCmdStr;
}

