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
#include "Utils.h"

Packet::Packet()
{
    header_.key_length = 0;
    header_.extras_length = 0;
    header_.body_length = 0;
    header_.data_type = 0x00;
    header_.status = 0x0000;
    header_.opaque = 0x00000000;
    header_.cas = 0;
}

Packet::~Packet()
{

}

void Packet::setExtras( const char* extras, size_t size)
{
    payload_.extras.resize(size);
    memcpy( &payload_.extras[0], extras, size );
    header_.extras_length = size;
}

void Packet::setCommand( const uint8_t command )
{
    header_.opcode = command;
}

bool Packet::IsGetCommand() const
{
    return header_.opcode == GET_COMMAND;
}

bool Packet::IsSetCommand() const
{
    return header_.opcode == SET_COMMAND;
}

std::string Packet::getKey() const
{
    if ( !payload_.key.empty() )
    {
        return std::string( (char*)&payload_.key[0], payload_.key.size() );
    }
    return "";
}

std::string Packet::getValue() const
{
    if ( !payload_.value.empty() )
    {
        return std::string( (char*)&payload_.value[0], payload_.value.size() );
    }
    return "";
}

Header Packet::getHeader() const
{
    return header_;
}

Payload Packet::getPayload() const
{
    return payload_;
}

void Packet::setKey( const std::string key )
{
    if ( !key.empty() )
    {
        payload_.key.resize( key.size() );
        memcpy( &payload_.key[0], key.data(), key.size() );
        
        header_.key_length = key.size();
    }
}

void Packet::setValue( const std::string value)
{
    if ( !value.empty() )
    {
        payload_.value.resize( value.size() );
        memcpy( &payload_.value[0], value.data(), value.size() );
        header_.body_length = header_.key_length + header_.extras_length + value.size();
    }
}

bool Packet::Parse( const Buffer& buffer )
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
                payload_.extras.resize( header_.extras_length );
                uint8_t* extras_start = (uint8_t*)(buffer.data() + sizeof(Header));
                memcpy( &payload_.extras[0], extras_start , header_.extras_length);
            }

            if ( header_.key_length > 0 )
            {
                payload_.key.resize( header_.key_length );
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

void Packet::Format( packet_t& buffer )
{
    // Copy header
    Header header = header_;
    // Perform necessary byte reordering 
    header.key_length = htons( header.key_length ); 
    header.status = htons( header.status ); 
    header.body_length= htonl( header.body_length ); 
    header.opaque= htonl( header.opaque); 
    header.cas= htonll( header.cas ); 
    buffer.resize(sizeof(Header));
    memcpy(&buffer[0], &header, sizeof(Header));

    // Copy extras
    uint8_t* extras_start = (uint8_t*)(&buffer[0] + sizeof(Header));
    if (!payload_.extras.empty())
    {
        buffer.resize(buffer.size() + payload_.extras.size());
        memcpy( extras_start, &payload_.extras[0], payload_.extras.size() );
    }

    // Copy key
    uint8_t* key_start = (uint8_t*)(&buffer[0] + (sizeof(Header) + payload_.extras.size()));
    if (!payload_.key.empty())
    {
        buffer.resize(buffer.size() + payload_.key.size());
        memcpy( key_start, &payload_.key[0], payload_.key.size() );
    }

    // Copy value
    if (!payload_.value.empty())
    {
        buffer.resize(buffer.size() + payload_.value.size());
        uint8_t* value_start = (uint8_t*)(&buffer[0] + (sizeof(Header) + payload_.key.size() + payload_.extras.size()));
        memcpy( value_start, &payload_.value[0], payload_.value.size() );
    }
 }

Request::Request()
{
    header_.magic_byte = REQUEST_MAGIC_BYTE;
}

Request::~Request()
{

}

bool Request::IsValid() const
{
    if ( IsGetCommand() )
        return (payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
    else if ( IsSetCommand() ) 
        return (!payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
    
    return false;
}

Response::Response()
{
    header_.magic_byte = RESPONSE_MAGIC_BYTE;
}

Response::~Response()
{

}

void Response::InitFrom( const Request& request )
{
    header_ = request.getHeader();
    header_.magic_byte = RESPONSE_MAGIC_BYTE; // Response

    if ( request.IsGetCommand() )
    {
         //response 
    }
}

bool Response::IsValid() const
{
    if ( IsGetCommand() )
        return (!payload_.extras.empty() && !payload_.key.empty());    
    else if ( IsSetCommand() )
        return (!payload_.extras.empty() && !payload_.key.empty() && !payload_.value.empty());    
    return false;
}
