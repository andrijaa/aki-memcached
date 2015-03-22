#include "Cache.h"

Cache::Cache()
{

}

Cache::~Cache()
{

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
