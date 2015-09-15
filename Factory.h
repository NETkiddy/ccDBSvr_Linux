#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "DataStruct.h"
#include <string>
#include "ConfigSvr.h"

class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();

	static BaseCommand* build(char cCmdID, char cType, std::string sContent);
};


class SQLFactory
{
public:
	SQLFactory();
	~SQLFactory();

	static void build(int iDBType, BaseDB_sPtr& ptr, Config cfg);
};
#endif
