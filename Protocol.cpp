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
    if (buffer.size() >= sizeof(Header) )
    {
        header_ = *(reinterpret_cast<const Header*>(buffer.data()));
        // Perform necessary byte reordering 
        header_.key_length = ntohs( header_.key_length ); 
        header_.status = ntohs( header_.status ); 
        header_.body_length= ntohl( header_.body_length ); 
        header_.opaque= ntohl( header_.opaque); 
        header_.cas= ntohll( header_.cas ); 

        if ( buffer.size() >= (sizeof(Header) + header_.extras_length + header_.key_length + header_.body_length ) )
        {
            if ( header_.extras_length > 0 )
            {
                payload_.key.resize( header_.key_length );
                uint8_t* extras_start = (uint8_t*)(buffer.data() + sizeof(Header));
                memcpy( &payload_.extras[0], extras_start , header_.extras_length);
            }

            if ( header_.key_length > 0 )
            {
                payload_.extras.resize( header_.extras_length );
                uint8_t* key_start = (uint8_t*)(buffer.data() + sizeof(Header) + header_.extras_length);
                memcpy( &payload_.key[0], key_start, header_.key_length);
                std::cerr<< " Key = " << std::string( (char*)&payload_.key[0], header_.key_length) << std::endl;
            }       

            if ( header_.body_length > 0 )
            {
                uint8_t* body_start = (uint8_t*)(buffer.data() + sizeof(Header) + header_.extras_length + header_.key_length);
                size_t value_size = header_.body_length - (header_.extras_length + header_.key_length);
                payload_.value.resize( value_size);
                memcpy( &payload_.value[0], body_start, value_size);
                std::cerr<< " Value = " << std::string( (char*)&payload_.value[0], value_size) << std::endl;
            }       

            return true;
        }
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
