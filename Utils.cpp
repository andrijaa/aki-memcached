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


