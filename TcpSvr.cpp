#include <iostream>  
#include "TcpSvr.h"
#include "ServiceModule.h"

using namespace boost;  
using namespace boost::asio; 

#define max_len 1024

//class TcpServer;

TcpSession::TcpSession(io_service &ios, TcpSvr *owner)
: _socket(ios),
  owner(owner)	
{
	
}

TcpSession::~TcpSession()
{
}


ip::tcp::socket &TcpSession::getSocket()
{
	return _socket;
}

void TcpSession::start()
{	
	_sIP = _socket.remote_endpoint().address().to_string();
	_iPort = _socket.remote_endpoint().port();
	
	static ip::tcp::no_delay option(true);
	_socket.set_option(option);

	boost::asio::async_read_until(
		_socket, 
		_sbuf, 
		"\n", 
		boost::bind(
			&TcpSession::reader_handler, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}


void TcpSession::reader_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
	/*
	//max_len可以换成较小的数字，就会发现async_read_some可以连续接收未收完的数据
	_socket.async_read_some(
		boost::asio::buffer(data_,max_len),
		boost::bind(
			&TcpSvr::reader_handler, 
			shared_from_this(), 
			boost::asio::placeholders::error)
	);
	*/
	std::istream is(&_sbuf);
	std::string sRecv;
	is >> sRecv;
	//assemble
	MsgData msgData = assembleMessage(sRecv);
	//enqueue
	pthread_mutex_lock(&(owner->owner->mutQuick));
	owner->owner->pushMessage(msgData);
	pthread_mutex_unlock(&(owner->owner->mutQuick));
	
	memset(cbuf, 0, 1024);
	strncpy(cbuf, "200", 3);
	boost::asio::async_write(
		_socket,
		boost::asio::buffer(cbuf, strlen(cbuf)),
		boost::bind(
			&TcpSession::writer_handler,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

}

void TcpSession::writer_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
	boost::asio::async_read_until(
		_socket,
		_sbuf,
		"\n",
		boost::bind(
			&TcpSession::reader_handler,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
			
}


MsgData TcpSession::assembleMessage(std::string sMsg)
{
	MsgData msgData;
	size_t iPos = sMsg.find_first_of("@");
	if(iPos == string::npos)
	{
		std::cout<<"@ not found, error message type"<<std::endl;
		return msgData;
	}
	std::string sType = sMsg.substr(0, iPos + 1);
	std::string sContent = sMsg.substr(iPos);

	msgData[sType] = sContent;
}


TcpSvr::TcpSvr(ServiceModule *owner)
: acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 6688)),
  owner(owner)
{
}
		
TcpSvr::~TcpSvr()
{
}

void TcpSvr::open()
{
	start();
	std::cout<<"Open Tcpsvr success, Port 6688"<<std::endl;
	ios.run();//start run callback
}
void TcpSvr::start()
{	
	session_ptr new_session(new TcpSession(ios, this));

	acceptor.async_accept(
		new_session->getSocket(), 
		bind(&TcpSvr::acceptor_handler, this, new_session, boost::asio::placeholders::error)
	);
	
}

void TcpSvr::acceptor_handler(session_ptr new_session, const system::error_code& err)
{
	if(err)	
	{
		std::cout<<"acceptor_handler error: "<<err<<std::endl;
		return;
	}

	std::cout<<"Client Got"<<std::endl;
	//tcp session queue
	stringstream ss;
	ss<<(new_session->_iPort);

	std::string sKey = new_session->_sIP + "@" + ss.str();
	if(m_mapSession.find(sKey) != m_mapSession.end())
	{
		MsgData assembleMessage(std::string sMsg);

		std::cout<<sKey + "Already connected"<<std::endl;
		return;
	}
	m_mapSession[sKey] = new_session;

	new_session->start();




	//std::cout<<"Clinet: "<<sock->remote_endpoint().address()<<", Port: "<<sock->remote_endpoint().port()<<std::endl;
//	sock->async_write_some(buffer("hello asio"),bind(&TcpSvr::writer_handler,this,boost::asio::placeholders::error));
	start();//keeping accepting
}


/*
void TcpSvr::writer_handler(boost::shared_ptr<std::string> pstr, system::error_code ec, size_t bytes_transferred)
{
	if(ec)
		std::cout<< "发送失败!" << std::endl;
	else
		std::cout<< *pstr << " 已发送" << std::endl;
}

void TcpSvr::writer_handler(const boost::system::error_code& error)
{
	std::cout<<"Send Msg Complete..."<<std::endl;
}
*/


void TcpSvr::close()
{
	
}
