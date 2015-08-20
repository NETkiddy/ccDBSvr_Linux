#include <iostream>  
#include "TcpSvr.h"

//using namespace boost;  
//using namespace boost::asio; 

#define max_len 1024


TcpSession::TcpSession(ioservice &ios)
: _socket(ios)
{
}

TcpSession::~TcpSession()
{
}

void TcpSession::start()
{
	static tcp::no_delay option(true);
	_socket.set_option(option);

	boost::asio::async_read_until(
		_socket, 
		_sbuf, 
		"\n", 
		boost_bind(
			&TcpSession::reader_handler, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

tcp::socket TcpSession::getSocket()
{
	return _socket;
}

void TcpSession::reader_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
	//max_len可以换成较小的数字，就会发现async_read_some可以连续接收未收完的数据
	_socket.async_read_some(
		boost::asio::buffer(data_,max_len),
		boost::bind(
			&TcpSvr::reader_handler, 
			shared_from_this(), 
			boost::asio::placeholders::error)
	);
	
	//assemble
	MsgData msgData = assembleMessage();
	//enqueue
	pthread_mutex_lock(owner->mutQuick);
	owner->pushMessage();
	pthread_mutex_unlock(owner->mutQuick);
}

void TcpSession::writer_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
	
}


MsgData TcpSession::assembleMessage(std::string sMsg)
{
	MsgData msgData;
	size_t ipos = sMsg.find_first_of("@");
	if(ipos == string::npos)
	{
		std::cout<<"@ not found, error message type"<<std::endl;
		return msgData;
	}
	std::string sType = sMsg.substr(0, iPos + 1);
	std::string sContent = sMsg.substr(iPos);

	msgData[sType] = sContent;
}


TcpSvr::TcpSvr(ServiceModule *owner)
: acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 6688))
{
}
		
TcpSvr::~TcpSvr()
{
}

void TcpSvr::open()
{
	std::cout<<"Open Tcpsvr success, Port 6688"<<std::endl;
	start();
	ios.run();//start run callback
}
void TcpSvr::start()
{	
	session_ptr new_session(new TcpSession(ios));

	acceptor.async_accept(
		new_session->getSocket(), 
		bind(&TcpSvr::acceptor_handler, this, new_session, boost::asio::placeholders::error)
	);
	
}

void TcpSvr::acceptor_handler(session_ptr new_session, const system::error_code& err)
{
	if(err)	
		std::cout<<"acceptor_handler error: "<<err<<std::endl;
		return;
	}

	std::cout<<"Client Got"<<std::endl;
	//tcp session queue
	std::string sKey = sock->remote_endpoint().address() + "@" + sock->remote_endpoint().port();
	if(m_mapSession.find(sKey) != m_mapSock.end())
	{
		std::cout<<sKey + "Already connected"<<std::endl;
		return nullptr;
	}
	m_mapSession[sKey] = new_session;

	new_session->start();




	std::cout<<"Clinet: "<<sock->remote_endpoint().address()<<", Port: "<<sock->remote_endpoint().port()<<std::endl;
	sock->async_write_some(buffer("hello asio"),bind(&TcpSvr::writer_handler,this,boost::asio::placeholders::error));
	start();//
}


/*
void TcpSvr::writer_handler(boost::shared_ptr<std::string> pstr, system::error_code ec, size_t bytes_transferred)
{
	if(ec)
		std::cout<< "发送失败!" << std::endl;
	else
		std::cout<< *pstr << " 已发送" << std::endl;
}
*/

void TcpSvr::writer_handler(const boost::system::error_code& error)
{
	std::cout<<"Send Msg Complete..."<<std::endl;
}

void TcpSvr::close()
{
	
}
