#ifndef _TCPSVR_H_
#define _TCPSVR_H_

//#include <conio.h>  
#include "ServiceModule.h"
#include <iostream>  
#include <map>
using namespace std;  
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/bind.hpp>  
using namespace boost;  
using namespace boost::asio;  

//typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
typedef std::map<string, string> MsgData;


class TcpSession : public boost:enable_shared_from_this<TcpSession>
{
public:
	TcpSession(io_service &ios);
	~TcpSession();
	void start();
	ip::tcp::socket &getSocket();

private:
	void reader_handler(const boost::system::error_code &error, size_t bytes_transferred);
	void writer_handler(const boose::system::error_code &error, size_t bytes_transferred);
	MsgData assembelMessage(std::string sMsg);
	
private:
	std::string _sIP;
	int _iPort;
	ip::tcp::socket _socket;
	boost::asio::streambuf _sbuf;
};



typedef boost::shared_ptr<TcpSession> session_ptr;
class TcpSvr
{
public:
	std::map<std::string, session_ptr> m_mapSession;
	pthread_mutex_t mutSock = PTHREAD_MUTEX_INITIALIZER;

private:
	io_service ios;
	ip::tcp::acceptor acceptor;

public:
	TcpSvr(ServiceModule *owner);
	~TcpSvr();
	void open();
	void start();
	void close();
	
	void acceptor_handler(const system::error_code& err, sock_pt sock);
//	void writer_handler(boost::shared_ptr<std::string> pstr, system::error_code ec, size_t bytes_transferred);
	//void acceptor_handler(const system::error_code& err, sock_pt sock);
	//void acceptor_handler();
	void writer_handler(const boost::system::error_code& error);

private:
};

#endif
