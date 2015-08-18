//#include <conio.h>  
#include <iostream>  
using namespace std;  
//#include <boost/asio.hpp>  
//#include <boost/shared_ptr.hpp>  
//#include <boost/bind.hpp> 
#include "TcpSvr.h"

using namespace boost;  
using namespace boost::asio; 

TcpSvr::TcpSvr()
: acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 6688))
{
//	open();
}
		
TcpSvr::~TcpSvr()
{
}

void TcpSvr::open()
{
	sock_pt sock(new ip::tcp::socket(ios));
	acceptor.async_accept(*sock, bind(&TcpSvr::acceptor_handler, this, boost::asio::placeholders::error, sock));
}

void TcpSvr::acceptor_handler(const system::error_code& err, sock_pt sock)
{
	if(err)
	{
		return;
	}
	std::cout<<"Clinet: "<<sock->remote_endpoint().address()<<", Port: "<<sock->remote_endpoint().port()<<std::endl;
	sock->async_write_some(buffer("hello asio"),bind(&TcpSvr::writer_handler,this,boost::asio::placeholders::error));
	open();//
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
















//using boost::asio::ip::tcp;
//using namespace std;





/*
void process_client(shared_ptr<tcp::socket> client)
{
        time_t now = time(0);
        shared_ptr<string> message(new string(ctime(&now)));

        auto callback = [=](const boost::system::error_code& err ,size_t size)
        {
            if ((int)size == message->length())
                cout << "write completed" << endl;
     	   };

        client->async_send(boost::asio::buffer(*message), callback);
}

typedef function<void (const boost::system::error_code&)> accept_callback;
    
void start_accept(tcp::acceptor& server)
{
		shared_ptr<tcp::socket> client(new tcp::socket(server.get_io_service()));
		accept_callback callback = [&server, client](const boost::system::error_code& error)
		{
				if (!error)
						process_client(client);

				start_accept(server);
		};

		server.async_accept(*client, callback);
}


void open()
{
		try
		{
				boost::asio::io_service io_service;
				tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3200));
				start_accept(acceptor);
				io_service.run();
		}
		catch (std::exception& e)
		{
				std::cerr << e.what() << std::endl;
		}
		return 0;
}
*/







