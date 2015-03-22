#if !defined(AKI_CACHE)
#define AKI_CACHE

#include <unordered_map>
#include <boost/asio/deadline_timer.hpp>
#include <string>

class Cache
{
    public:
        Cache();
        ~Cache(); 

    public:
        std::string Get( const std::string key ) const;
        bool Set( const std::string key, const std::string value, const uint64_t expiration);

    private:

    struct ValueType
    {
        std::string value;
        boost::asio::deadline_timer* expiration_timer;
    };

    typedef std::unordered_map< std::string, ValueType> CacheSet;
    CacheSet cache_;

};

#endif