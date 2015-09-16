#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include <boost/any.hpp>   
#include <string>
#include <map>

class EntityFactory
{
public:
    virtual boost::any NewInstance()
    {
		return boost::any();
    }
};

std::map<std::string, EntityFactory*>& entityFactoryMap();
boost::any GetInstanceByName(const std::string &name);

#define REFLECTOR(name) \
class EntityFactory##name : public EntityFactory \
{ \
public: \
    boost::any NewInstance() \
    { \
    	return boost::any(new name); \
    } \
}; \
void register_factory_##name() \
{ \
    if(entityFactoryMap().find(#name) == entityFactoryMap().end()) \
    { \
    	entityFactoryMap()[#name] = new EntityFactory##name(); \
    } \
} \
__attribute__((constructor))void register_factory_##name();


#endif
