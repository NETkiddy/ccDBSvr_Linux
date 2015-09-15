#include "Factory.h"
#include "CommandEntity.h"
#include "MySqlDB.h"

CommandFactory::CommandFactory()
{

}

CommandFactory::~CommandFactory()
{

}

BaseCommand* CommandFactory::build(char cCmdID, char cType, std::string sContent)
{
	BaseCommand *pCommand = nullptr;
	switch(cCmdID)
	{
		case '1':
			pCommand = new FirstCommand(cCmdID, cType, sContent);
			break;
		case '2':
			pCommand = new SecondCommand(cCmdID, cType, sContent);
			break;
		case '3':
			pCommand = new ThirdCommand(cCmdID, cType, sContent);
			break;
		default:
			pCommand = new BaseCommand(cCmdID, cType, sContent);
			break;
	}

	return pCommand;
}



SQLFactory::SQLFactory()
{
}

SQLFactory::~SQLFactory()
{
}

void SQLFactory::build(int iDBType, BaseDB_sPtr &ptr, Config cfg)
{
	switch(iDBType)
	{
		case 0:
			ptr.reset(new MySqlDB(cfg));			
			break;

		case 1:
			//TODO
			break;

		case 2:
			//TODO
			break;
	}
}
