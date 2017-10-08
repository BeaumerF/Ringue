#ifndef USERVER_H
# define USERVER_H

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <list>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "UConnectionManager.h"
#include "UConnectionTCP.h"
#include "IServer.h"

class UServer : public IServer, private boost::noncopyable
{
public:
  explicit UServer(const std::string& address, const std::string& port);
  void run();
private:
  void start_accept();
  void handle_accept();
  void handle_accept(const boost::system::error_code& e);
  void handle_stop();

  boost::asio::io_service             io_service_;
  boost::asio::signal_set             signals_;
  boost::asio::ip::tcp::acceptor      acceptor_;
  UConnectionManager                  connection_manager_;
  UConnectionTCP::connection_ptr      new_connection_;
};

#endif
