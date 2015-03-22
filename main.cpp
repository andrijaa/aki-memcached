#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "AkiServer.h"
#include "Utils.h"

static std::string SERVER_ADDRESS="127.0.0.1";
static std::string SERVER_PORT="11211"; // default memcached port
static size_t NUM_THREADS=8;

int main(int argc, char* argv[])
{
  try
  {
      std::cerr <<"AkiServer starting..." << std::endl;
      std::cerr <<"Main thread ID -> " << getThreadId() << std::endl;
      
      AkiServer aki_server(SERVER_ADDRESS, SERVER_PORT, NUM_THREADS);
      aki_server.Run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Aki Server Error -> Exception: " << e.what() << std::endl;
  }

  return 0;
}

