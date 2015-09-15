#include "ConfigSvr.h"
#include "define.h"
#include <unistd.h>
#include "tinyxml2.h"


ConfigSvr::ConfigSvr(Config &cfg)
{
}

ConfigSvr::~ConfigSvr()
{
}

bool ConfigSvr::isFileExist(const char *filePath)
{
	bool bRet = false;
	if(!filePath)
		return bRet;
	if(0 == access(filePath, F_OK))
		bRet = true;
	return bRet;
}

void ConfigSvr::loadCoreOption(Config &cfg)
{
}

void ConfigSvr::log_error(std::string sErrInfo)
{
	std::cout<<sErrInfo<<endl;
}

void ConfigSvr::loadServiceOption(Config &cfg)
{
	std::string sFilePath = CONFIG_FILE;
	if(!isFileExist(sFilePath.c_str()))
	{
		std::cout<<sFilePath<<" not exist"<<std::endl;
		return;
	}

	using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    XMLError xmlerr = doc.LoadFile(sFilePath.c_str());
    if(xmlerr != XML_SUCCESS)
    {
        log_error("LoadSystem: " + sFilePath + " Loads Failed, " + intToStr(xmlerr));
    }
    else
    {
        try
        {
            tinyxml2::XMLElement* e_Root = doc.RootElement();
            if(NULL == e_Root)
            {
                log_error("LoadSystem: " + sFilePath + " Root Element Created Failed");
                return;
            }

            tinyxml2::XMLElement* e_service = e_Root->FirstChildElement("service");
            if(e_service == NULL)
            {
                log_error("LoadSystem: " + sFilePath + " <service> Created Failed");
                return;
            }

            //----
            tinyxml2::XMLElement* e_appid = e_service->FirstChildElement("appid");
            if(NULL == e_appid)
            {
                log_error("LoadSystem: " + sFilePath + " <appid> Created Failed");
                return;
            }                                          
            std::string sAppID = e_appid->GetText();
            cfg[APP_ID] = sAppID;

            //----
            tinyxml2::XMLElement* e_servicecount = e_service->FirstChildElement("count");
            if(NULL == e_servicecount)
            {
                log_error("LoadSystem: " + sFilePath + " <count> Created Failed");
                return;
            }
            std::string sServiceCount = e_servicecount->GetText();
            cfg[SERVICE_COUNT] = sServiceCount;


            //----
            tinyxml2::XMLElement* e_config = e_Root->FirstChildElement("config");
            if(e_config == NULL)
            {
                log_error("LoadSystem: " + sFilePath + " <config> Created Failed");
                return;
            }
            //----
            tinyxml2::XMLElement* e_param = e_config->FirstChildElement("param");
            while(e_param != NULL)
            {
	            //--------------------------------------------------------------------------------------
                tinyxml2::XMLElement* e_module = e_param->FirstChildElement("module");
                if(NULL == e_module)
                {
                    log_error("LoadSystem: " + sFilePath + " <module> Created Failed");
                    return;
                }
                //
                tinyxml2::XMLElement* e_moduleid = e_module->FirstChildElement("id");
                if(NULL == e_moduleid)
                {
                    log_error("LoadSystem: " + sFilePath + " <id> Created Failed");
                    return;
                }
	            std::string sModuleID = e_moduleid->GetText();
	            cfg[MODULE_ID] = sModuleID;

                //
                tinyxml2::XMLElement* e_moduleport = e_module->FirstChildElement("port");
                if(NULL == e_moduleport)
                {
                    log_error("LoadSystem: " + sFilePath + " <port> Created Failed");
                    return;
                }
	            std::string sModulePort = e_moduleport->GetText();
	            cfg[MODULE_PORT] = sModulePort;

                //
                tinyxml2::XMLElement* e_updateinfohour = e_module->FirstChildElement("updateinfohour");
                if(NULL == e_updateinfohour)
                {
                    log_error("LoadSystem: " + sFilePath + " <updateinfohour> Created Failed");
                    return;
                }
	            std::string sUpdateinfoHour = e_updateinfohour->GetText();
	            cfg[UPDATE_INFO_HOUR] = sUpdateinfoHour;

                //
                tinyxml2::XMLElement* e_mqcount = e_module->FirstChildElement("mqcount");
                if(NULL == e_mqcount)
                {
                    log_error("LoadSystem: " + sFilePath + " <mqcount> Created Failed");
                    return;
                }
	            std::string sMQCount = e_mqcount->GetText();
	            cfg[MQ_COUNT] = sMQCount;


	            //--------------------------------------------------------------------------------------
                tinyxml2::XMLElement* e_db = e_param->FirstChildElement("db");
                if(NULL == e_db)
                {
                    log_error("LoadSystem: " + sFilePath + " <db> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_dbtype = e_db->FirstChildElement("type");
                if(NULL == e_dbtype)
                {
                    log_error("LoadSystem: " + sFilePath + " <type> Created Failed");
                    return;
                }
	            std::string sDBType = e_dbtype->GetText();
	            cfg[DB_TYPE] = sDBType;

                //
                tinyxml2::XMLElement* e_dbsource = e_db->FirstChildElement("source");
                if(NULL == e_dbsource)
                {
                    log_error("LoadSystem: " + sFilePath + " <source> Created Failed");
                    return;
                }
	            std::string sDBSource = e_dbsource->GetText();
	            cfg[DB_SOURCE] = sDBSource;

                //
                tinyxml2::XMLElement* e_dbport = e_db->FirstChildElement("port");
                if(NULL == e_dbport)
                {
                    log_error("LoadSystem: " + sFilePath + " <port> Created Failed");
                    return;
                }
	            std::string sDBPort = e_dbport->GetText();
	            cfg[DB_PORT] = sDBPort;

                //
                tinyxml2::XMLElement* e_dbmode = e_db->FirstChildElement("mode");
                if(NULL == e_dbmode)
                {
                    log_error("LoadSystem: " + sFilePath + " <mode> Created Failed");
                    return;
                }
	            std::string sDBMode = e_dbmode->GetText();
	            cfg[DB_MODE] = sDBMode;

                //
                tinyxml2::XMLElement* e_dbname = e_db->FirstChildElement("name");
                if(NULL == e_dbname)
                {
                    log_error("LoadSystem: " + sFilePath + " <name> Created Failed");
                    return;
                }
	            std::string sDBName = e_dbname->GetText();
	            cfg[DB_NAME] = sDBName;

                //
                tinyxml2::XMLElement* e_dbusername = e_db->FirstChildElement("username");
                if(NULL == e_dbusername)
                {
                    log_error("LoadSystem: " + sFilePath + " <username> Created Failed");
                    return;
                }
	            std::string sDBUsername = e_dbusername->GetText();
	            cfg[DB_USERNAME] = sDBUsername;

                //
                tinyxml2::XMLElement* e_dbpassword = e_db->FirstChildElement("password");
                if(NULL == e_dbpassword)
                {
                    log_error("LoadSystem: " + sFilePath + " <password> Created Failed");
                    return;
                }
	            std::string sDBPassword = e_dbpassword->GetText();
	            cfg[DB_PASSWORD] = sDBPassword;

                //
                tinyxml2::XMLElement* e_dbtimeout = e_db->FirstChildElement("timeout");
                if(NULL == e_dbtimeout)
                {
                    log_error("LoadSystem: " + sFilePath + " <timeout> Created Failed");
                    return;
                }
	            std::string sDBTimeout = e_dbtimeout->GetText();
	            cfg[DB_TIMEOUT] = sDBTimeout;

                //
                tinyxml2::XMLElement* e_spfile = e_db->FirstChildElement("spfile");
                if(NULL == e_spfile)
                {
                    log_error("LoadSystem: " + sFilePath + " <spfile> Created Failed");
                    return;
                }
	            std::string sDBSPFile = e_spfile->GetText();
	            cfg[DB_SP_FILE] = sDBSPFile;

                //
                tinyxml2::XMLElement* e_wsfile = e_db->FirstChildElement("wsfile");
                if(NULL == e_wsfile)
                {
                    log_error("LoadSystem: " + sFilePath + " <wsfile> Created Failed");
                    return;
                }
	            std::string sDBWSFILE = e_wsfile->GetText();
	            cfg[DB_SP_FILE] = sDBWSFILE;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_threadpool = e_param->FirstChildElement("threadpool");
                if(NULL == e_threadpool)
                {
                    log_error("LoadSystem: " + sFilePath + " <threadpool> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_threadpoolcount = e_threadpool->FirstChildElement("count");
                if(NULL == e_threadpoolcount)
                {
                    log_error("LoadSystem: " + sFilePath + " <count> Created Failed");
                    return;
                }
	            std::string sThreadpoolCount = e_threadpoolcount->GetText();
	            cfg[THREADPOOL_COUNT] = sThreadpoolCount;

                //
                tinyxml2::XMLElement* e_threadpooltimeout = e_threadpool->FirstChildElement("timeout");
                if(NULL == e_threadpooltimeout)
                {
                    log_error("LoadSystem: " + sFilePath + " <timeout> Created Failed");
                    return;
                }
	            std::string sThreadpoolTimeout = e_threadpooltimeout->GetText();
	            cfg[THREADPOOL_TIMEOUT] = sThreadpoolTimeout;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_rabbitmq = e_param->FirstChildElement("rabbitmq");
                if(NULL == e_rabbitmq)
                {
                    log_error("LoadSystem: " + sFilePath + " <rabbitmq> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_rabbitmqhost = e_rabbitmq->FirstChildElement("host");
                if(NULL == e_rabbitmqhost)
                {
                    log_error("LoadSystem: " + sFilePath + " <host> Created Failed");
                    return;
                }
	            std::string sRabbitMQHost = e_rabbitmqhost->GetText();
	            cfg[RABBITMQ_HOST] = sRabbitMQHost;

                //
                tinyxml2::XMLElement* e_rabbitmqport = e_rabbitmq->FirstChildElement("port");
                if(NULL == e_rabbitmqport)
                {
                    log_error("LoadSystem: " + sFilePath + " <port> Created Failed");
                    return;
                }
	            std::string sRabbitMQPort = e_rabbitmqport->GetText();
	            cfg[RABBITMQ_PORT] = sRabbitMQPort;

                //
                tinyxml2::XMLElement* e_rabbitmqusername = e_rabbitmq->FirstChildElement("username");
                if(NULL == e_rabbitmqusername)
                {
                    log_error("LoadSystem: " + sFilePath + " <username> Created Failed");
                    return;
                }
	            std::string sRabbitMQUsername = e_rabbitmqusername->GetText();
	            cfg[RABBITMQ_USERNAME] = sRabbitMQUsername;

                //
                tinyxml2::XMLElement* e_rabbitmqpassword = e_rabbitmq->FirstChildElement("password");
                if(NULL == e_rabbitmqpassword)
                {
                    log_error("LoadSystem: " + sFilePath + " <password> Created Failed");
                    return;
                }
	            std::string sRabbitMQPassword = e_rabbitmqpassword->GetText();
	            cfg[RABBITMQ_PASSWORD] = sRabbitMQPassword;

                //
                tinyxml2::XMLElement* e_rabbitmqvhost = e_rabbitmq->FirstChildElement("vhost");
                if(NULL == e_rabbitmqvhost)
                {
                    log_error("LoadSystem: " + sFilePath + " <vhost> Created Failed");
                    return;
                }
	            std::string sRabbitMQVhost= e_rabbitmqvhost->GetText();
	            cfg[RABBITMQ_VHOST] = sRabbitMQVhost;

                //
                tinyxml2::XMLElement* e_rabbitmqmaxframe = e_rabbitmq->FirstChildElement("maxframe");
                if(NULL == e_rabbitmqmaxframe)
                {
                    log_error("LoadSystem: " + sFilePath + " <maxframe> Created Failed");
                    return;
                }
	            std::string sRabbitMQMaxframe = e_rabbitmqmaxframe->GetText();
	            cfg[RABBITMQ_MAXFRAME] = sRabbitMQMaxframe;

                //
                tinyxml2::XMLElement* e_rabbitmqqueuenameprefix = e_rabbitmq->FirstChildElement("queuenameprefix");
                if(NULL == e_rabbitmqqueuenameprefix)
                {
                    log_error("LoadSystem: " + sFilePath + " <queuenameprefix> Created Failed");
                    return;
                }
	            std::string sRabbitMQQueueNamePrefix = e_rabbitmqqueuenameprefix->GetText();
	            cfg[RABBITMQ_QUEUENAME_PRIFIX] = sRabbitMQQueueNamePrefix;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_pipe = e_param->FirstChildElement("pipe");
                if(NULL == e_pipe)
                {
                    log_error("LoadSystem: " + sFilePath + " <pipe> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_pipenameprefix = e_pipe->FirstChildElement("nameprefix");
                if(NULL == e_pipenameprefix)
                {
                    log_error("LoadSystem: " + sFilePath + " <nameprefix> Created Failed");
                    return;
                }
	            std::string sPipeNamePrefix = e_pipenameprefix->GetText();
	            cfg[PIPE_NAMEPREFIX] = sPipeNamePrefix;

                //
                tinyxml2::XMLElement* e_pipemode = e_pipe->FirstChildElement("mode");
                if(NULL == e_pipemode)
                {
                    log_error("LoadSystem: " + sFilePath + " <mode> Created Failed");
                    return;
                }
	            std::string sPipeMode = e_pipemode->GetText();
	            cfg[PIPE_MODE] = sPipeMode;



                //当前SP中下一个Param节点
                e_param = e_param->NextSiblingElement();
            }
        }
        catch(std::exception &e)
        {
            log_error("Exception-LoadSPOption: " + sFilePath + " Convert Error, " + e.what());
        }
    }
}

std::string ConfigSvr::intToStr(int iValue)
{
	stringstream ss;
	ss<<iValue;
	
	return ss.str();
}

std::string ConfigSvr::charToStr(char cValue)
{
	stringstream ss;
	ss<<cValue;

	return ss.str();
}
