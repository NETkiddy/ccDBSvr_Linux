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
void BaseCommand::preProcess()
{

}
void BaseCommand::execute()
{

}
void BaseCommand::postProcess()
{

}


FirstCommand::FirstCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
FirstCommand::~FirstCommand()
{

}
void FirstCommand::preProcess()
{

}
void FirstCommand::execute()
{

}
void FirstCommand::postProcess()
{

}


SecondCommand::SecondCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
SecondCommand::~SecondCommand()
{

}
void SecondCommand::preProcess()
{

}
void SecondCommand::execute()
{

}
void SecondCommand::postProcess()
{

}


ThirdCommand::ThirdCommand(char cCmdID, char cType, std::string sContent)
: BaseCommand(cCmdID, cType, sContent)
{

}
ThirdCommand::~ThirdCommand()
{

}
void ThirdCommand::preProcess()
{

}
void ThirdCommand::execute()
{

}
void ThirdCommand::postProcess()
{

}