#include <vector>
#include <boost/bind.hpp>

#include <Connection.h>
#include <Cache.h>
#include <Protocol.h>
#include <Utils.h>

TcpConnection::TcpConnection(boost::asio::io_service& io_service)
: socket_(io_service), strand_(io_service)
{
}

TcpConnection::~TcpConnection()
{
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

boost::asio::ip::tcp::socket& TcpConnection::socket()
{
    return socket_;
}

void TcpConnection::Start()
{
    socket_.async_read_some(boost::asio::buffer(read_buffer_), strand_.wrap(
        boost::bind(&TcpConnection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred))
    );
}

void TcpConnection::HandleRead(const boost::system::error_code& e, std::size_t bytes_read)
{
    if (!e)
    {
        Request request;
        if (request.Parse(read_buffer_))
        {
            Response response;
            if( Cache::Instance()->ProcessCommand(request, response) )
            {
                response.Format( write_buffer_ ); 
                boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_), 
                    strand_.wrap(boost::bind(&TcpConnection::HandleWrite, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred))
                );
            }
        }
        else 
        {       
            socket_.async_read_some(boost::asio::buffer(read_buffer_),
                strand_.wrap( boost::bind(&TcpConnection::HandleRead, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred))
            );
        }
    }
    else
    {
        //std::cerr << "Handle read error !" << std::endl;
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }
}


void TcpConnection::HandleWrite(const boost::system::error_code& e /*error*/, size_t bytes_transferred/*bytes_transferred*/)
{
    if (!e)
    {
        socket_.async_read_some(boost::asio::buffer(read_buffer_),
           strand_.wrap( boost::bind(&TcpConnection::HandleRead, shared_from_this(),
           boost::asio::placeholders::error,
           boost::asio::placeholders::bytes_transferred))
        );
    }
    else
    {
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        //std::cerr << "HandleWrite Error" << std::endl;
    }
}
