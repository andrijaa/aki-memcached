#include <iostream>

#include "Cache.h"

Cache::Cache()
{

}

Cache::~Cache()
{

}

Cache* Cache::Instance()
{
    static Cache* cache_instance = new Cache();
    return cache_instance;
}


std::string Cache::Get( const std::string key ) const
{
    CacheSet::const_iterator it = cache_.find( key );
    if (it != cache_.end())
    {
        return it->second.value; 
    }
    // TODO: return other than "" if not found?
    return "";
}

bool Cache::Set( const std::string key, const std::string value, const uint64_t expiration ) 
{
    ValueType value_info;
    value_info.value = value;

    cache_[key] = value_info;
    return true;
}

bool Cache::ProcessCommand(const Request& request, Response& response)
{
    if (request.IsGetCommand())
    {
        std::cerr << std::endl;
        std::cerr << " Recv GET Command ! " << std::endl;
        std::cerr << " Key = " << request.getKey() << std::endl;
        std::string value;
        {
            boost::mutex::scoped_lock lock( cache_mutex_ );
            value = Get( request.getKey() );
        }
        std::cerr << " Value = " << value << std::endl;
        response.setCommand( GET_COMMAND );
        char extras[4] = {0x00, 0x00, 0x00, 0x00};
        response.setExtras( extras, 4);       
        response.setValue( value );       
        return true;
    }

    if (request.IsSetCommand())
    {
        std::cerr << std::endl;
        std::cerr << " Recv SET Command ! " << std::endl;
        std::cerr << " Key = " << request.getKey() << std::endl;
        std::cerr << " Value = " << request.getValue() << std::endl;
        {
            boost::mutex::scoped_lock lock( cache_mutex_ );
            Set( request.getKey(), request.getValue(), UINT64_MAX ); 
        }
        response.setCommand( SET_COMMAND );
        return true;
    } 

    return false;   
}
