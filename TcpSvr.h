#ifndef _TCPSVR_H_
#define _TCPSVR_H_

//#include <conio.h>  
//#include "ServiceModule.h"
#include <iostream>  
#include <string>
#include <map>
using namespace std;  
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/bind.hpp> 
#include <boost/enable_shared_from_this.hpp>
using namespace boost;  
using namespace boost::asio;  

//typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
typedef std::map<string, string> MsgData;
class ServiceModule;
class TcpSvr;

class TcpSession : public boost::enable_shared_from_this<TcpSession>
{
public:
	TcpSession(io_service &ios, TcpSvr *owner);
	~TcpSession();
	void initData();
	void start();
	ip::tcp::socket &getSocket();
public:
	std::string _sIP;
	int _iPort;

private:
	void reader_handler(const boost::system::error_code &error, size_t bytes_transferred);
	void writer_handler(const boost::system::error_code &error, size_t bytes_transferred);
	MsgData assembleMessage(std::string sMsg);
	
private:
	TcpSvr	*owner;
	ip::tcp::socket _socket;
	boost::asio::streambuf _sbuf;
	char _cdata[1024];
	char _cbuf[1024];
};



typedef boost::shared_ptr<TcpSession> session_ptr;
class TcpSvr
{
public:
	std::map<std::string, session_ptr> m_mapSession;
	pthread_mutex_t mutSock = PTHREAD_MUTEX_INITIALIZER;
	ServiceModule *owner;

private:
	io_service ios;
	ip::tcp::acceptor acceptor;

public:
	TcpSvr(ServiceModule *owner);
	~TcpSvr();
	void open();
	void start();
	void close();
	void closeSession(std::string sKey);
	
	void acceptor_handler(session_ptr new_session, const system::error_code& err);
//	void writer_handler(boost::shared_ptr<std::string> pstr, system::error_code ec, size_t bytes_transferred);
	//void acceptor_handler(const system::error_code& err, sock_pt sock);
	//void acceptor_handler();
	//void writer_handler(const boost::system::error_code& error);

private:
};

#endif
