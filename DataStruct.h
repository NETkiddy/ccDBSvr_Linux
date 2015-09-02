#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <iostream>
#include <string> 

struct MsgData
{
	char cCmdID;
	char cType;
	std::string sContent;
};


class SQLDB
{
public:
	SQLDB();
	~SQLDB();
	virtual bool open() = 0;
	virtual bool close() = 0;
public:
	std::string				m_sMode;
	std::string				m_sIP;
	int						m_iPort;
	std::string 			m_sUsername;
	std::string				m_sPassword;


};

class BaseCommand
{
public:
	BaseCommand(char cCmdID, char cType, std::string sContent);
	~BaseCommand();

	virtual void preProcess();
	virtual void execute();
	virtual void postProcess();
public:
	char	m_cCmdID;
	char	m_cType;
	std::string m_sContent;


};

#endif


