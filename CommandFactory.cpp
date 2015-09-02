#include "CommandFactory.h"


CommandFactory::CommandFactory()
{

}

CommandFactory::~CommandFactory()
{

}

BaseCommand* CommandFactory::build(char cCmdID, char cType, std::string sContent)
{
	BaseCommand *pCommand = nullptr;
	switch(cType)
	{
		case 'a':
			pCommand = new FirstCommand(cCmdID, cType, sContent);
			break;
		case 'b':
			pCommand = new SecondCommand(cCmdID, cType, sContent);
			break;
		case 'c':
			pCommand = new ThirdCommand(cCmdID, cType, sContent);
			break;
		default:
			pCommand = new BaseCommand(cCmdID, cType, sContent);
			break;
	}

	return pCommand;
}