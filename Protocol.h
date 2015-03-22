#if !defined(PROTOCOL_H)
#define PROTOCOL_H

/*
 * Simple implementation of binary memcached
 * https://code.google.com/p/memcached/wiki/MemcacheBinaryProtocol
 * 
 * Aki Colovic
 */

#include <string>
#include <boost/array.hpp>

static int REQUEST_MAGIC_BYTE = 0x80;
static int RESPONSE_MAGIC_BYTE = 0x81;
        
typedef boost::array<char, 8192> Buffer;

struct Header
{
    uint8_t magic_byte;
    uint8_t opcode;
    uint16_t key_length;
    uint8_t extras_length;
    uint8_t data_type;
    uint16_t status;
    uint32_t body_length;
    uint32_t opaque;
    uint64_t cas;
};

struct Payload
{
    Buffer extras;
    Buffer key;
    Buffer value;
};

class Request 
{
    public:
        bool Parse( const Buffer& buffer );    
        bool Format( Buffer&  buffer );    
        bool IsValid() const;

    private:
        Header header_;
        Payload payload_;
};

class Response 
{
    public:
        bool IsValid() const;

    private:
        Header header_;
        Payload payload_;
};

#endif
