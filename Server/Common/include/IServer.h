#ifndef ISERVER_H
# define ISERVER_H

class IServer
{
public:
  IServer() {};
  virtual ~IServer() {};
  virtual void run() = 0;
  virtual void start_accept() = 0;
  virtual void handle_accept() = 0;
  virtual void handle_stop() = 0;
};

#endif
