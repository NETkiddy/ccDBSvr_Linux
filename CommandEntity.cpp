#include "CommandEntity.h"



BaseCommand::BaseCommand(char cCmdID, char cType, std::string sContent)
: m_cCmdID(cCmdID),
  m_cType(cType),
  m_sContent(sContent)
{

}
BaseCommand::~BaseCommand()
{

}
bool BaseCommand::preProcess()
{
	return true;

}
bool BaseCommand::execute(BaseDB_sPtr &spBaseDB)
{
	return spBaseDB->execute(m_sContent);
}
bool BaseCommand::postProcess()
{
	return true;

}


FirstCommand::FirstCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
FirstCommand::~FirstCommand()
{

}
bool FirstCommand::preProcess()
{
	return true;

}
bool FirstCommand::execute(BaseDB_sPtr &spBaseDB)
{
	return spBaseDB->execute(m_sContent);
}
bool FirstCommand::postProcess()
{
	return true;

}


SecondCommand::SecondCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
SecondCommand::~SecondCommand()
{

}
bool SecondCommand::preProcess()
{
	return true;

}
bool SecondCommand::execute(BaseDB_sPtr &spBaseDB)
{
	return spBaseDB->execute(m_sContent);

}
bool SecondCommand::postProcess()
{
	return true;

}


ThirdCommand::ThirdCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
ThirdCommand::~ThirdCommand()
{

}
bool ThirdCommand::preProcess()
{
	return true;

}
bool ThirdCommand::execute(BaseDB_sPtr &spBaseDB)
{
	return spBaseDB->execute(m_sContent);

}
bool ThirdCommand::postProcess()
{
	return true;
}
