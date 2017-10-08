#ifndef WSERVER_H
# define WSERVER_H

#include "IServer.h"

class WServer : public IServer
{
public:
  WServer();
  virtual ~WServer();
  void startAccept();
  void handleClient();
  void sendAdresses();
  void stockAdresses();
};

#endif
