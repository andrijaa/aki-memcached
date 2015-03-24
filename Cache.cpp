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


const packet_t* Cache::Get( const std::string key, packet_t& extras ) const
{
    CacheSet::const_iterator it = cache_.find( key );
    if (it != cache_.end())
    {
        if ( it->second.extras.empty())
        {
            extras.resize(4);
            std::fill (extras.begin(),extras.begin()+4,0x00);
        }
        else
        {
            extras = it->second.extras;
        }
        return &(it->second.value); 
    }
    return NULL;
}

bool Cache::Set( const std::string key, const packet_t* value, const packet_t* extras, const uint64_t expiration ) 
{
    ValueType value_info;
    value_info.value = *value;
    value_info.extras = *extras;
    value_info.extras = *extras;
    value_info.extras.resize(4); // we only care about flags, TODO: handle expiry field 

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
        
        boost::mutex::scoped_lock lock( cache_mutex_ );
        packet_t extras;
        const packet_t* value = Get( request.getKey(), extras );
        //std::cerr << " Value = " << std::string(value[ << std::endl;
        response.setCommand( GET_COMMAND );

        response.setExtras( extras);       
        response.setValue( value );
        return true;
    }

    if (request.IsSetCommand())
    {
        std::cerr << std::endl;
        std::cerr << " Recv SET Command ! " << std::endl;
        std::cerr << " Key = " << request.getKey() << std::endl;
        //std::cerr << " Value = " << request.getValue() << std::endl;
        {
            boost::mutex::scoped_lock lock( cache_mutex_ );
            packet_t extras = request.getPayload().extras;
            Set( request.getKey(), request.getValue(), &extras, UINT64_MAX ); 
        }
        response.setCommand( SET_COMMAND );
        return true;
    } 

    return false;   
}
