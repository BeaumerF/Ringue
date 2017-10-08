#include "CmdHandler.h"

CmdHandler::CmdHandler()
{

}

std::vector<std::string> CmdHandler::splitRequest(std::string const &request)
{
    std::istringstream ss(request);
    std::string token;
    std::vector<std::string> requestVector;

    while(std::getline(ss, token, '\n'))
        requestVector.push_back(token);

    return (requestVector);
}
