#ifndef _COMMAND_Entity_H_
#define _COMMAND_Entity_H_

#include "DataStruct.h"

class FirstCommand : public BaseCommand
{
public:
	FirstCommand(char cCmdID, char cType, std::string sContent);
	~FirstCommand();
	virtual bool preProcess();
	virtual bool execute(BaseDB_sPtr &spBaseDB);
	virtual bool postProcess();
};

class SecondCommand : public BaseCommand
{
public:
	SecondCommand(char cCmdID, char cType, std::string sContent);
	~SecondCommand();
	virtual bool preProcess();
	virtual bool execute(BaseDB_sPtr &spBaseDB);
	virtual bool postProcess();

};

class ThirdCommand : public BaseCommand
{
public:
	ThirdCommand(char cCmdID, char cType, std::string sContent);
	~ThirdCommand();
	virtual bool preProcess();
	virtual bool execute(BaseDB_sPtr &spBaseDB);
	virtual bool postProcess();

};

#endif
