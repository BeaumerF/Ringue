#ifndef CMDHANDLER_H
#define CMDHANDLER_H

#include <iostream>
#include <vector>
#include <sstream>

class CmdHandler
{
public:
    CmdHandler();
    std::vector<std::string> splitRequest(std::string const &);
};

#endif // CMDHANDLER_H
