#if !defined(CONNECTION_H)
#define CONNECTION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <vector>
#include <ctime>
#include <iostream>
#include <string>

#include "Protocol.h"

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>, private boost::noncopyable
{
    public:

        typedef boost::shared_ptr<TcpConnection> ConnectionPointer;

        /// Construct a connection with the given io_service.
        explicit TcpConnection(boost::asio::io_service& io_service);

        boost::asio::ip::tcp::socket& socket();

        void Start();

    private:

        void HandleRead(const boost::system::error_code& e, std::size_t bytes_read);

        void HandleWrite(const boost::system::error_code& /*error*/, size_t bytes_transferred/*bytes_transferred*/);

    private:

        boost::asio::ip::tcp::socket socket_;
        std::string message_;
        
        // Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::io_service::strand strand_;

        // Buffer for incoming data.
        Buffer buffer_;
};

#endif
