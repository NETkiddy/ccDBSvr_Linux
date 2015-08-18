#include "ConfigSvr.h"
#include "define.h"


ConfigSvr::ConfigSvr(Config &cfg)
{
}

ConfigSvr::~ConfigSvr()
{
}

void ConfigSvr::loadCoreOption(Config &cfg)
{
}

void ConfigSvr::loadServiceOption(Config &cfg)
{

	cfg[IP] = "127.0.0.1";
	cfg[PORT] = "3306";
	cfg[MODE] = "tcp";
	cfg[USERNAME] = "root";
	cfg[PASSWORD] = "123";
}


