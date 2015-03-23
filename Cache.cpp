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

    // TODO: implement CAS
    cache_[key] = value_info;
    return true;
}

bool Cache::ProcessCommand(const Request& request, Response& response)
{
    if (request.IsGetCommand())
    {
        std::string value = Get( request.getKey() );
        response.InitFrom( request );
        response.setValue( value );       
        return true;
    }

    if (request.IsSetCommand())
    {
        // TODO: add support for expiration
        Set( request.getKey(), request.getValue(), UINT64_MAX ); 
        response.InitFrom( request );
        return true;
    } 

    // TODO: support more commands

    return false;   
}
