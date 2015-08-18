#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <iostream>
#include <string> 




class SQLDB
{
public:
	std::string				m_sMode;
	std::string				m_sIP;
	int						m_iPort;
	std::string 			m_sUsername;
	std::string				m_sPassword;

public:
	SQLDB();
	~SQLDB();
	virtual bool open() = 0;
	virtual bool close() = 0;

};



#endif


