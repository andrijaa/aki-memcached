/*
 * Simple implementation of binary memcached
 * https://code.google.com/p/memcached/wiki/MemcacheBinaryProtocol
 * 
 * Aki Colovic
 */
#include <netinet/in.h>
#include <netinet/tcp.h>   

#include "Protocol.h"
#include "Connection.h"

bool Request::Format(Buffer& buffer)
{

   return false; 
}

bool Request::Parse( const Buffer& buffer )
{
    if (buffer.size() >= 24 )
    {
        header_ = *(reinterpret_cast<const Header*>(buffer.data()));
        header_.key_length = ntohs( header_.key_length ); 
        header_.status = ntohs( header_.status ); 
        header_.body_length= ntohl( header_.body_length ); 
        header_.opaque= ntohl( header_.opaque); 
        header_.cas= ntohll( header_.cas ); 

        std::cerr << " magic_byte = " << header_.magic_byte;
        std::cerr << " key_length = " << header_.key_length;

        return true;
    }

    return false;
}

bool Request::IsValid() const
{
    // Get
    return (payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
    
    // Set
    return (!payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
    
}

bool Response::IsValid() const
{
    // Get
    return (!payload_.extras.empty() && !payload_.key.empty());    
    // Set
    return (!payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
}
