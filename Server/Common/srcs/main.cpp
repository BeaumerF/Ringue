#include <iostream>
//#include "BException.h"

#ifdef WIN32
# include "WServer.h"
#else
# include "UServer.h"
#endif

int main(int argc, char **argv)
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: ./server <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    ./server 127.0.0.1 5001\n";
      return 1;
    }
#ifdef WIN32
#else
    UServer server(argv[1], argv[2]);
    server.run();
#endif
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return (0);
}
