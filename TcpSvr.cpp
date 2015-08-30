#include <iostream>  
#include "TcpSvr.h"
#include "ServiceModule.h"

using namespace boost;  
using namespace boost::asio; 

#define MAX_LEN 1024

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

void TcpSession::initData()
{
	static ip::tcp::no_delay option(true);
	_socket.set_option(option);
	_sIP = _socket.remote_endpoint().address().to_string();
	_iPort = _socket.remote_endpoint().port();

}

void TcpSession::start()
{	

	memset(_cdata, 0, sizeof(_cdata));
	//max_len可以换成较小的数字，就会发现async_read_some可以连续接收未收完的数据
	_socket.async_read_some(
		boost::asio::buffer(_cdata, MAX_LEN),
		boost::bind(
			&TcpSession::reader_handler, 
			shared_from_this(), 
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	/*
	boost::asio::async_read_until(
		_socket, 
		_sbuf, 
		"0a0b",
		boost::bind(
			&TcpSession::reader_handler, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	*/
}


void TcpSession::reader_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
	if(error)
	{
		stringstream ss;
		ss<<_iPort;
		owner->closeSession(_sIP+"@"+ss.str());
		return;
	}

	std::cout<<"Handling read..."<<std::endl;
	std::string sRecv(_cdata);
	
	/*
	std::istream is(&_sbuf);
	is >> sRecv;
	*/

	std::cout<<"Receive: "<<sRecv<<std::endl;
	//assemble
	MsgData msgData;
	assembleMessage(msgData, sRecv);
	//enqueue
	std::cout<<"lock"<<std::endl;
	owner->owner->pushMessage(msgData);
	std::cout<<"unlock"<<std::endl;
	
	memset(_cbuf, 0, 1024);
	strncpy(_cbuf, "200", 3);
	_cbuf[3] = '\0';
	boost::asio::async_write(
		_socket,
		boost::asio::buffer(_cbuf, strlen(_cbuf)),
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
	if(error)
	{
		stringstream ss;
		ss<<_iPort;
		owner->closeSession(_sIP+"@"+ss.str());
		return;
	}
	std::cout<<"Handling write..."<<std::endl;
	std::cout<<"Send: "<<_cbuf<<std::endl;

	//max_len可以换成较小的数字，就会发现async_read_some可以连续接收未收完的数据
	_socket.async_read_some(
		boost::asio::buffer(_cdata, MAX_LEN),
		boost::bind(
			&TcpSession::reader_handler, 
			shared_from_this(), 
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

	/*
	boost::asio::async_read_until(
		_socket,
		_sbuf,
		"0a0b",
		boost::bind(
			&TcpSession::reader_handler,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	*/		
}


void TcpSession::assembleMessage(MsgData &msgData, std::string sMsg)
{
	
	std::cout<<"Assemble Message: "<<sMsg<<std::endl;
	size_t iPos = sMsg.find_first_of("@");
	if(iPos == string::npos)
	{
		std::cout<<"@ not found, error message type"<<std::endl;
	}
	std::string sCmdID = sMsg.substr(0, iPos + 1);
	std::string sContent = sMsg.substr(iPos);
	
	iPos = sContent.find_first_of("@");
	if(iPos == string::npos)
	{
		std::cout<<"@ not found, error message type"<<std::endl;
	}
	std::string sType = sContent.substr(0, iPos + 1);
	sContent = sContent.substr(iPos);

	char *tmp = const_cast<char*>(sCmdID.c_str());
	msgData.cCmdID = tmp[0];
	tmp = const_cast<char*>(sType.c_str());
	msgData.cType = tmp[0];
	msgData.sContent = sContent;
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
	new_session.get()->initData();
	
	//tcp session queue
	stringstream ss;
	ss<<(new_session.get()->_iPort);

	std::string sKey = new_session.get()->_sIP + "@" + ss.str();
	if(m_mapSession.find(sKey) != m_mapSession.end())
	{
		std::cout<<sKey + "Already connected"<<std::endl;
		return;
	}
	m_mapSession[sKey] = new_session;
	std::cout<<"Client "<<sKey<<" Connected"<<std::endl;

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

void TcpSvr::closeSession(std::string sKey)
{
	std::map<std::string, session_ptr>::iterator iter = m_mapSession.find(sKey);
	if(iter != m_mapSession.end())
	{
		iter->second.get()->getSocket().close();
		std::cout<<"Client "<<sKey<<" Closed"<<std::endl;
		m_mapSession.erase(iter++);//?delete?
	}
}
