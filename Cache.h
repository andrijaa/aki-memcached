#if !defined(AKI_CACHE)
#define AKI_CACHE

#include <unordered_map>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>

#include "Protocol.h"

class Cache
{
    public:
        Cache();
        ~Cache(); 

    public:
        static Cache* Instance();

    public:
        const packet_t* Get( const std::string key ) const;
        bool Set( const std::string key, const packet_t* value, const uint64_t expiration);

        bool ProcessCommand(const Request& request, Response& response);

    private:

        struct ValueType
        {
            packet_t value;
            boost::asio::deadline_timer* expiration_timer;
        };

        typedef std::unordered_map< std::string, ValueType> CacheSet;
        CacheSet cache_;

        boost::mutex cache_mutex_;

};

#endif
