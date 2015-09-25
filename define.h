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
	
const 	std::string		EMPTY_STRING				=	"";

const	std::string 	CONFIG_FILE					=	"system.xml";
const	std::string 	APP_ID						=	"appid";
const	std::string 	SERVICE_COUNT				=	"servicecount";
const	std::string 	MODULE_ID					=	"moduleid";
const	std::string 	MODULE_PORT					=	"moduleport";
const	std::string 	UPDATE_INFO_HOUR			=	"updateinfohour";
const	std::string 	MQ_COUNT					=	"mqcount";
const	std::string 	DB_TYPE						=	"dbtype";
const	std::string 	DB_SOURCE					=	"dbsource";
const	std::string 	DB_PORT						=	"dbport";
const	std::string 	DB_NAME						=	"dbname";
const	std::string 	DB_MODE						=	"dbmode";
const	std::string 	DB_USERNAME					=	"dbusername";
const	std::string 	DB_PASSWORD					=	"dbpassword";
const	std::string 	DB_TIMEOUT					=	"dbtimeout";
const	std::string 	DB_SP_FILE					=	"dbspfile";
const	std::string 	DB_WS_FILE					=	"dbwsfile";
const	std::string 	THREADPOOL_COUNT			=	"threadpoolcount";
const	std::string 	THREADPOOL_TIMEOUT			=	"threadpooltimeout";
const	std::string 	RABBITMQ_HOST				=	"rabbitmqhost";
const	std::string 	RABBITMQ_PORT				=	"rabbitmqport";
const	std::string 	RABBITMQ_USERNAME			=	"rabbitmqusername";
const	std::string 	RABBITMQ_PASSWORD			=	"rabbitmqpassword";
const	std::string 	RABBITMQ_VHOST				=	"rabbitmqvhost";
const	std::string 	RABBITMQ_MAXFRAME			=	"rabbitmqmaxframe";
const	std::string 	RABBITMQ_QUEUENAME_PRIFIX	=	"rabbitmqqueuenameprefix";
const	std::string 	PIPE_NAMEPREFIX				=	"normalpipename";
const	std::string 	PIPE_MODE					=	"normalpipemode";

const	int 			EPOLL_MAX_EVENTS			=	3;//Quick, normalMQ, retryMQ

const 	int 			T_QUICK_QUEUE				=	0;
const 	int 			T_NORMAL_QUEUE				=	1;
const 	int 			T_RETRY_QUEUE				=	2;
const 	int 			T_HEARTBEAT					=	3;

const 	int				MAX_QUICK_SIZE				=	500000;

const	std::string		CONSUMER_PREFIX				= 	"consumer_tag_";

const	int				FAILED_INDEX				=	-1;
const	int				SUCCESS_INDEX				=	0;
#endif
