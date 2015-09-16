#include "Reflector.h"
#include <iostream>

std::map<std::string, EntityFactory*>& entityFactoryMap()
{
    static std::map<std::string, EntityFactory*>* factoryMap = new std::map<std::string, EntityFactory*>;
    return *factoryMap;
}

boost::any GetInstanceByName(const std::string& name) 
{
    if (entityFactoryMap().find(name) != entityFactoryMap().end()) 
    {
		std::cout<<"entityFactoryMap size: "<<entityFactoryMap().size()<<std::endl;
    	return entityFactoryMap()[name]->NewInstance();
    }
    return nullptr;
}
