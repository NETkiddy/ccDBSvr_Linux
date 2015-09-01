#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <string>

enum SIGNAL_TYPE
{
	SIGNAL_QUICK = 0,
	SIGNAL_NORMAL,
	SIGNAL_RETRY,
	SIGNAL_HEARTBEAT
};

const 	std::string		EMPTY_STRING		=	"";

const 	std::string		IP					=	"ip";
const 	std::string		PORT				=	"port";
const 	std::string		MODE				=	"mode";
const 	std::string		USERNAME			=	"username";
const 	std::string		PASSWORD			=	"password";

const 	int 			T_QUICK_QUEUE		=	0;
const 	int 			T_NORMAL_QUEUE		=	1;
const 	int 			T_RETRY_QUEUE		=	2;
const 	int				MAX_QUICK_SIZE		=	500000;

const 	std::string		RABBITMQ_HOST		=	"host";
const 	std::string		RABBITMQ_PORT		=	"port";
const 	std::string		RABBITMQ_USERNAME	=	"username";
const	std::string		RABBITMQ_PASSWORD	=	"password";
const 	std::string		RABBITMQ_VHOST		=	"vhost";
const 	std::string 	RABBITMQ_MAXFRAME	=	"maxframe";

const	int 			EPOLL_MAX_EVENTS	=	2;//normalMQ, retryMQ
const	std::string 	NORMAL_PIPE_NAME	=	"normalpipename";
const	std::string 	NORMAL_PIPE_MODE	=	"normalpipemode";
const	std::string 	RETRY_PIPE_NAME		=	"retrypipename";
const	std::string 	RETRY_PIPE_MODE		=	"retrypipemode";

#endif
