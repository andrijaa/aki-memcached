#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "AkiServer.h"
#include "Utils.h"


AkiServer::AkiServer(const std::string& address, const std::string& port, std::size_t thread_pool_size)
: thread_pool_size_(thread_pool_size),
    signals_(io_service_),
    acceptor_(io_service_),
    new_connection_()
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&AkiServer::OnStop, this));

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.bind(endpoint);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.listen(); 

    StartAccept();
}

void AkiServer::Run()
{
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
            boost::bind(&boost::asio::io_service::run, &io_service_)));
        threads.push_back(thread);
    }
    std::cerr << "Aki Server started!" << std::endl;

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
    {
        threads[i]->join();
    }
}

void AkiServer::StartAccept()
{
    new_connection_.reset(new TcpConnection(io_service_));  
    acceptor_.async_accept(new_connection_->socket(), 
            boost::bind(&AkiServer::OnConnectionAccepted, this, new_connection_, boost::asio::placeholders::error));
}

void AkiServer::OnConnectionAccepted(TcpConnection::ConnectionPointer new_connection, const boost::system::error_code& error)
{
    std::cerr <<" OnConnectionAccepted!" << " Thread id= " << getThreadId() <<  std::endl;
    if (!error)
    {
        new_connection_->Start();
    }
    StartAccept();
}

void AkiServer::OnStop()
{
    std::cerr <<" AkiService::OnStop" << std::endl;
    io_service_.stop();
}

