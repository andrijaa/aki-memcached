#include <vector>
#include <boost/bind.hpp>

#include <Connection.h>
#include <Protocol.h>

TcpConnection::TcpConnection(boost::asio::io_service& io_service)
: socket_(io_service), strand_(io_service)
{
}

boost::asio::ip::tcp::socket& TcpConnection::socket()
{
    return socket_;
}

void TcpConnection::Start()
{
    socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
        boost::bind(&TcpConnection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred))
    );
}


void TcpConnection::HandleRead(const boost::system::error_code& e, std::size_t bytes_read)
{
    // 1. Read until a complete request is read
    // 2. Read until a complete request is read

    if (!e)
    {
        /*        std::stringstream ss;
        for(int i=0; i<bytes_read; ++i)
            ss << std::hex << (int)buffer_[i];
        std::string hex_version = ss.str();
        std::cerr << hex_version << std::endl; 
*/
        Request request;
        if (request.Parse(buffer_))
        {
            // TODO: parse one by one
            // TODO: process command
            // TODO: respond
        }
        else 
        {       
            socket_.async_read_some(boost::asio::buffer(buffer_),strand_.wrap(
                        boost::bind(&TcpConnection::HandleRead, shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred))
                    );
        }
    }
}


void TcpConnection::HandleWrite(const boost::system::error_code& /*error*/, size_t bytes_transferred/*bytes_transferred*/)
{
    std::cerr << "Finished writing! " << bytes_transferred << std::endl;
}
