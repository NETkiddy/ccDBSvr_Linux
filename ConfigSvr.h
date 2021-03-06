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

	static bool isFileExist(const char *filePath);
	static void loadCoreOption(Config &cfg);
	static void loadServiceOption(Config &cfg);
	static void log_error(std::string sErrInfo);
	static std::string intToStr(int);
	static std::string charToStr(char);
	
	template<class out_type,class in_value>
	out_type convert(const in_value & t);

};


#endif
