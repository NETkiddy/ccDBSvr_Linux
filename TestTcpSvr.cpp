#include <iostream>
#include "TcpSvr.h"
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <unistd.h>
#include "ServiceModule.h"

int main()
{
//	io_service ios;

//	TcpSvr tcp(ios);
//	tcp.open();	


	ServiceModule sm;


	while(1)
	{
		sleep(1000);
	}
	return 1;
}
