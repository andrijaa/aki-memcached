#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "Utils.h"

unsigned long getThreadId()
{
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}

std::string DecToHex( char ch )
{
    std::string str;

    std::string hex = "0123456789ABCDEF";

    for (int i=2*sizeof( char ) - 1; i>=0; i--) 
    {
        str += hex[((ch >> i*4) & 0xF)];
    }
    return str;
}

std::string PrintHex( void* buffer, size_t size, unsigned columns )
{
    std::ostringstream str;            
    for (unsigned k =0; k < size; k++ )
    {
        if ( (k % columns) == 0 )
            str << std::endl << "  ";
        str << " " << DecToHex( ((char*)buffer)[k] ) << " ";
    }

    return str.str();
}




