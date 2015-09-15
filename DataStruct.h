#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <iostream>
#include <string> 
#include <memory>

struct MsgData
{
	char cCmdID;
	char cType;
	std::string sContent;
};


class BaseDB
{
public:
	BaseDB();
	~BaseDB();
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool isConnected() = 0;
	virtual void reconnect() = 0;
	virtual bool execute(std::string sQueryStr) = 0;

public:
	std::string				m_sMode;
	std::string				m_sIP;
	int						m_iPort;
	std::string 			m_sUsername;
	std::string				m_sPassword;


};
typedef std::shared_ptr<BaseDB> BaseDB_sPtr;

class BaseCommand
{
public:
	BaseCommand(char cCmdID, char cType, std::string sContent);
	~BaseCommand();

	virtual bool preProcess();
	virtual bool execute(BaseDB_sPtr &spBaseDB);
	virtual bool postProcess();
public:
	char	m_cCmdID;
	char	m_cType;
	std::string m_sContent;


};

#endif


