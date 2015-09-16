#include <string>
#include <iostream>
#include "Reflector.h"

class TestClass
{
public:
    void Out()
    {
    	std::cout<<"i am TestClass"<<std::endl;
    }
};

class TestClass2
{
public:
    void Out()
    {
    	std::cout<<"i am TestClass 2"<<std::endl;
    }
};
REFLECTOR(TestClass);
REFLECTOR(TestClass2);

int main()
{
	try
	{
    boost::any instance = GetInstanceByName("TestClass2");
    TestClass2 *testClass = boost::any_cast<TestClass2*>(instance);
	testClass->Out();
	}
	catch(const boost::bad_any_cast& badcast_e)
	{  
		std::cout<<"bad_any_cast: "<<badcast_e.what()<<std::endl;
		return -1;  
	}  
    return 0;
}
