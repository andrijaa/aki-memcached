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
#include <vector>

static uint8_t REQUEST_MAGIC_BYTE = 0x80;
static uint8_t RESPONSE_MAGIC_BYTE = 0x81;

static uint8_t  GET_COMMAND = 0x00;
static uint8_t  SET_COMMAND = 0x01;
        
typedef boost::array<char, 8192> Buffer;
typedef std::vector<uint8_t> packet_t;

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
    packet_t extras;
    packet_t key;
    packet_t value;
};

class Packet
{
    public:
        Packet();
        virtual ~Packet();

    public:
        virtual bool IsValid() const = 0; 
        bool Parse( const Buffer& buffer );    
        void Format( packet_t&  buffer );    

    public:
        bool IsGetCommand() const;
        bool IsSetCommand() const;
        
        void setCommand( const uint8_t command);

        std::string getKey() const;
        const packet_t* getValue() const;

        Header getHeader() const;
        Payload getPayload() const;

        void setKey(const std::string key);
        void setValue(const packet_t* value);

        void setExtras( const packet_t& extras);

    protected:
        Header header_;
        Payload payload_;
};

class Request : public Packet 
{
    public:
        Request();
        ~Request();

    public:
        bool IsValid() const;
};

class Response : public Packet 
{
    public:
        Response();
        ~Response();

    public:
        bool IsValid() const;
};

#endif
