#ifndef _CONFIG_SVR_H
#define _CONFIG_SVR_H

#include <string>
#include <iostream>
#include <map>

typedef std::map<std::string, std::string> Config;

class ConfigSvr
{
public:
	ConfigSvr(Config &cfg);
	~ConfigSvr();

	static void loadCoreOption(Config &cfg);
	static void loadServiceOption(Config &cfg);



};


#endif
