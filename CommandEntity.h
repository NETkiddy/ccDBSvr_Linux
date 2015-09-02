#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

#include "DataStruct.h"

class FirstCommand : public BaseCommand
{
public:
	virtual void preProcess();
	virtual void execute();
	virtual void postProcess();
};

class SecondCommand : public BaseCommand
{
public:
	virtual void preProcess();
	virtual void execute();
	virtual void postProcess();

};

class ThirdCommand : public BaseCommand
{
public:
	virtual void preProcess();
	virtual void execute();
	virtual void postProcess();

};

#endif