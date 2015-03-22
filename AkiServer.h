#if !defined(AKI_SERVER_H)
#define AKI_SERVER_H

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include "Connection.h"

class AkiServer : private boost::noncopyable
{
    public:

        AkiServer(const std::string& address, const std::string& port, std::size_t thread_pool_size);

        void Run();

    private:

        void StartAccept();
        
        void OnStop();

        void OnConnectionAccepted(TcpConnection::ConnectionPointer new_connection, const boost::system::error_code& error);

        /// The number of threads that will call io_service::run().
        std::size_t thread_pool_size_;

        /// The io_service used to perform asynchronous operations.
        boost::asio::io_service io_service_;

        /// Acceptor used to listen for incoming connections.
        boost::asio::ip::tcp::acceptor acceptor_;

        /// The next connection to be accepted.
        TcpConnection::ConnectionPointer new_connection_;

        boost::asio::signal_set signals_;
};

#endif
