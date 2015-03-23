#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "AkiServer.h"
#include "Utils.h"

static const std::string SERVER_ADDRESS="127.0.0.1";
static const std::string SERVER_PORT="11211"; // default memcached port
static const size_t NUM_THREADS=8;

int main(int argc, char* argv[])
{
  try
  {
      if (argc != 4 && argc != 1)
      {
          std::cerr << "Usage: AkiServer <host> <port> <num-threads> \n";
          return 1;
      }

      
      // Initialise the server.
      std::size_t num_threads = NUM_THREADS;
      std::string server_host = SERVER_ADDRESS;
      std::string server_port = SERVER_PORT; 

      if ( argc == 4 )
      {
          num_threads = boost::lexical_cast<std::size_t>(argv[3]);
          server_host = std::string(argv[1]);
          server_port = std::string(argv[2]);
      }

      std::cerr <<"AkiServer starting @ host = " << server_host << ":" << server_port << " with " << num_threads << " threads." <<  std::endl;
      std::cerr <<"Main thread ID -> " << getThreadId() << std::endl;

      AkiServer aki_server(server_host, server_port, num_threads);
      aki_server.Run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Aki Server Error -> Exception: " << e.what() << std::endl;
  }

  return 0;
}

