#ifndef _TCPSVR_H_
#define _TCPSVR_H_

//#include <conio.h>  
#include "ServiceModule.h"
#include <iostream>  
using namespace std;  
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/bind.hpp>  
using namespace boost;  
using namespace boost::asio;  

typedef boost::shared_ptr<ip::tcp::socket> sock_pt;

class TcpSvr
{
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

};

#endif
