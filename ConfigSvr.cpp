#include "ConfigSvr.h"
#include "define.h"
#include <unistd.h>



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
        log_error("LoadSystem: " + sFilePath + " Loads Failed, " + IntToStr(xmlerr));
    }
    else
    {
        try
        {
            tinyxml2::XMLElement* e_Root = doc.RootElement();
            if(NULL == e_Root)
            {
                log_error("LoadSystem: " + sFilepath + " Root Element Created Failed");
                return;
            }

            tinyxml2::XMLElement* e_service = e_Root->FirstChildElement("service");
            if(e_service == NULL)
            {
                log_error("LoadSystem: " + sFilepath + " <service> Created Failed");
                return;
            }

            //----
            tinyxml2::XMLElement* e_appid = e_service->FirstChildElement("appid");
            if(NULL == e_appid)
            {
                log_error("LoadSystem: " + sFilepath + " <appid> Created Failed");
                return;
            }                                          
            String sAppID = e_appid->GetText();
            cfg[APP_ID] = sAppID;

            //----
            tinyxml2::XMLElement* e_servicecount = e_service->FirstChildElement("count");
            if(NULL == e_servicecount)
            {
                log_error("LoadSystem: " + sFilepath + " <count> Created Failed");
                return;
            }
            String sServiceCount = e_servicecount->GetText();
            cfg[SERVICE_COUNT] = sServiceCount;


            //----
            tinyxml2::XMLElement* e_config = e_Root->FirstChildElement("config");
            if(e_config == NULL)
            {
                log_error("LoadSystem: " + sFilepath + " <config> Created Failed");
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
                    log_error("LoadSystem: " + sFilepath + " <module> Created Failed");
                    return;
                }
                //
                tinyxml2::XMLElement* e_moduleid = e_module->FirstChildElement("id");
                if(NULL == e_moduleid)
                {
                    log_error("LoadSystem: " + sFilepath + " <id> Created Failed");
                    return;
                }
	            String sModuleID = e_moduleid->GetText();
	            cfg[MODULE_ID] = sModuleID;

                //
                tinyxml2::XMLElement* e_moduleport = e_module->FirstChildElement("port");
                if(NULL == e_moduleport)
                {
                    log_error("LoadSystem: " + sFilepath + " <port> Created Failed");
                    return;
                }
	            String sModulePort = e_moduleport->GetText();
	            cfg[MODULE_PORT] = sModulePort;

                //
                tinyxml2::XMLElement* e_updateinfohour = e_module->FirstChildElement("updateinfohour");
                if(NULL == e_updateinfohour)
                {
                    log_error("LoadSystem: " + sFilepath + " <updateinfohour> Created Failed");
                    return;
                }
	            String sUpdateinfoHour = e_updateinfohour->GetText();
	            cfg[UPDATE_INFO_HOUR] = sUpdateinfoHour;

                //
                tinyxml2::XMLElement* e_mqcount = e_module->FirstChildElement("mqcount");
                if(NULL == e_mqcount)
                {
                    log_error("LoadSystem: " + sFilepath + " <mqcount> Created Failed");
                    return;
                }
	            String sMQCount = e_mqcount->GetText();
	            cfg[MQ_COUNT] = sMQCount;


	            //--------------------------------------------------------------------------------------
                tinyxml2::XMLElement* e_db = e_param->FirstChildElement("db");
                if(NULL == e_db)
                {
                    log_error("LoadSystem: " + sFilepath + " <db> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_dbtype = e_db->FirstChildElement("type");
                if(NULL == e_dbtype)
                {
                    log_error("LoadSystem: " + sFilepath + " <type> Created Failed");
                    return;
                }
	            String sDBType = e_dbype->GetText();
	            cfg[DB_TYPE] = sDBType;

                //
                tinyxml2::XMLElement* e_dbsource = e_db->FirstChildElement("source");
                if(NULL == e_dbsource)
                {
                    log_error("LoadSystem: " + sFilepath + " <source> Created Failed");
                    return;
                }
	            String sDBSource = e_dbsource->GetText();
	            cfg[DB_SOURCE] = sDBSource;

                //
                tinyxml2::XMLElement* e_dbport = e_db->FirstChildElement("port");
                if(NULL == e_dbport)
                {
                    log_error("LoadSystem: " + sFilepath + " <port> Created Failed");
                    return;
                }
	            String sDBPort = e_dbport->GetText();
	            cfg[DB_PORT] = sDBPort;

                //
                tinyxml2::XMLElement* e_dbmode = e_db->FirstChildElement("mode");
                if(NULL == e_dbmode)
                {
                    log_error("LoadSystem: " + sFilepath + " <mode> Created Failed");
                    return;
                }
	            String sDBMode = e_dbmode->GetText();
	            cfg[DB_MODE] = sDBMode;

                //
                tinyxml2::XMLElement* e_dbname = e_db->FirstChildElement("name");
                if(NULL == e_dbname)
                {
                    log_error("LoadSystem: " + sFilepath + " <name> Created Failed");
                    return;
                }
	            String sDBName = e_dbname->GetText();
	            cfg[DB_NAME] = sDBName;

                //
                tinyxml2::XMLElement* e_dbusername = e_db->FirstChildElement("username");
                if(NULL == e_dbusername)
                {
                    log_error("LoadSystem: " + sFilepath + " <username> Created Failed");
                    return;
                }
	            String sDBUsername = e_dbusername->GetText();
	            cfg[DB_USERNAME] = sDBUsername;

                //
                tinyxml2::XMLElement* e_dbpassword = e_db->FirstChildElement("password");
                if(NULL == e_dbpassword)
                {
                    log_error("LoadSystem: " + sFilepath + " <password> Created Failed");
                    return;
                }
	            String sDBPassword = e_dbpassword->GetText();
	            cfg[DB_PASSWORD] = sType;

                //
                tinyxml2::XMLElement* e_dbtimeout = e_db->FirstChildElement("timeout");
                if(NULL == e_dbtimeout)
                {
                    log_error("LoadSystem: " + sFilepath + " <timeout> Created Failed");
                    return;
                }
	            String sDBTimeout = e_dbtimeout->GetText();
	            cfg[DB_TIMEOUT] = sDBTimeout;

                //
                tinyxml2::XMLElement* e_spfile = e_db->FirstChildElement("spfile");
                if(NULL == e_spfile)
                {
                    log_error("LoadSystem: " + sFilepath + " <spfile> Created Failed");
                    return;
                }
	            String sDBSPFile = e_spfile->GetText();
	            cfg[DB_SP_FILE] = sDBSPFile;

                //
                tinyxml2::XMLElement* e_wsfile = e_db->FirstChildElement("wsfile");
                if(NULL == e_wsfile)
                {
                    log_error("LoadSystem: " + sFilepath + " <wsfile> Created Failed");
                    return;
                }
	            String sDBWSFILE = e_wsfile->GetText();
	            cfg[DB_SP_FILE] = sDBWSFILE;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_threadpool = e_param->FirstChildElement("threadpool");
                if(NULL == e_threadpool)
                {
                    log_error("LoadSystem: " + sFilepath + " <threadpool> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_threadpoolcount = e_threadpool->FirstChildElement("count");
                if(NULL == e_threadpoolcount)
                {
                    log_error("LoadSystem: " + sFilepath + " <count> Created Failed");
                    return;
                }
	            String sThreadpoolCount = e_threadpoolcount->GetText();
	            cfg[THREADPOOL_COUNT] = sThreadpoolCount;

                //
                tinyxml2::XMLElement* e_threadpooltimeout = e_threadpool->FirstChildElement("timeout");
                if(NULL == e_threadpooltimeout)
                {
                    log_error("LoadSystem: " + sFilepath + " <timeout> Created Failed");
                    return;
                }
	            String sThreadpoolTimeout = e_threadpooltimeout->GetText();
	            cfg[THREADPOOL_TIMEOUT] = sThreadpoolTimeout;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_rabbitmq = e_param->FirstChildElement("rabbitmq");
                if(NULL == e_rabbitmq)
                {
                    log_error("LoadSystem: " + sFilepath + " <rabbitmq> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_rabbitmqhost = e_rabbitmq->FirstChildElement("host");
                if(NULL == e_rabbitmqhost)
                {
                    log_error("LoadSystem: " + sFilepath + " <host> Created Failed");
                    return;
                }
	            String sRabbitMQHost = e_rabbitmqhost->GetText();
	            cfg[RABBITMQ_HOST] = sRabbitMQHost;

                //
                tinyxml2::XMLElement* e_rabbitmqport = e_rabbitmq->FirstChildElement("port");
                if(NULL == e_rabbitmqport)
                {
                    log_error("LoadSystem: " + sFilepath + " <port> Created Failed");
                    return;
                }
	            String sRabbitMQPort = e_rabbitmqport->GetText();
	            cfg[RABBITMQ_PORT] = sRabbitMQPort;

                //
                tinyxml2::XMLElement* e_rabbitmqusername = e_rabbitmq->FirstChildElement("username");
                if(NULL == e_rabbitmqusername)
                {
                    log_error("LoadSystem: " + sFilepath + " <username> Created Failed");
                    return;
                }
	            String sRabbitMQUsername = e_rabbitmqusername->GetText();
	            cfg[RABBITMQ_USERNAME] = sRabbitMQUsername;

                //
                tinyxml2::XMLElement* e_rabbitmqpassword = e_rabbitmq->FirstChildElement("password");
                if(NULL == e_rabbitmqpassword)
                {
                    log_error("LoadSystem: " + sFilepath + " <password> Created Failed");
                    return;
                }
	            String sRabbitMQPassword = e_rabbitmqpassword->GetText();
	            cfg[RABBITMQ_PASSWORD] = sRabbitMQPassword;

                //
                tinyxml2::XMLElement* e_rabbitmqvhost = e_rabbitmq->FirstChildElement("vhost");
                if(NULL == e_rabbitmqvhost)
                {
                    log_error("LoadSystem: " + sFilepath + " <vhost> Created Failed");
                    return;
                }
	            String sRabbitMQVhost= e_rabbitmqvhost->GetText();
	            cfg[RABBITMQ_VHOST] = sRabbitMQVhost;

                //
                tinyxml2::XMLElement* e_rabbitmqmaxframe = e_rabbitmq->FirstChildElement("maxframe");
                if(NULL == e_rabbitmqmaxframe)
                {
                    log_error("LoadSystem: " + sFilepath + " <maxframe> Created Failed");
                    return;
                }
	            String sRabbitMQMaxframe = e_rabbitmqmaxframe->GetText();
	            cfg[RABBITMQ_MAXFRAME] = sRabbitMQMaxframe;

                //
                tinyxml2::XMLElement* e_rabbitmqqueuenameprefixe = e_rabbitmq->FirstChildElement("queuenameprefix");
                if(NULL == e_rabbitmqqueuenameprefix)
                {
                    log_error("LoadSystem: " + sFilepath + " <queuenameprefix> Created Failed");
                    return;
                }
	            String sRabbitMQQueueNamePrefix = e_rabbitmqqueuenameprefix->GetText();
	            cfg[RABBITMQ_QUEUENAME_PRIFIX] = sRabbitMQQueueNamePrefix;


	            //--------------------------------------------------------------------------------------
	            tinyxml2::XMLElement* e_pipe = e_param->FirstChildElement("pipe");
                if(NULL == e_pipe)
                {
                    log_error("LoadSystem: " + sFilepath + " <pipe> Created Failed");
                    return;
                }

                //
                tinyxml2::XMLElement* e_pipenameprefix = e_pipe->FirstChildElement("nameprefix");
                if(NULL == e_pipenameprefix)
                {
                    log_error("LoadSystem: " + sFilepath + " <nameprefix> Created Failed");
                    return;
                }
	            String sPipeNamePrefix = e_pipenameprefix->GetText();
	            cfg[PIPE_NAMEPREFIX] = sPipeNamePrefix;

                //
                tinyxml2::XMLElement* e_pipemode = e_pipe->FirstChildElement("mode");
                if(NULL == e_pipemode)
                {
                    log_error("LoadSystem: " + sFilepath + " <mode> Created Failed");
                    return;
                }
	            String sPipeMode = e_pipemode->GetText();
	            cfg[PIPE_MODE] = sPipeMode;



                //当前SP中下一个Param节点
                e_param = e_param->NextSiblingElement();
            }
        }
        catch(std::exception &e)
        {
            log_error("Exception-LoadSPOption: " + sFilepath + " Convert Error, " + e.what());
        }
    }
}


