#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

#include "DataStruct.h"
#include <string>

class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();

	static BaseCommand* build(char cCmdID, char cType, std::string sContent);
};


#endif