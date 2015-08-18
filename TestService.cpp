#include "CoreApp.h"
#include <unistd.h>

int main()
{
	CoreApp core;
	core.start();
	
	while(1)
	{
		sleep(10000);
		std::cout<<"Server Running..."<<std::endl;
	}
	return 1;
}
