#ifndef UCONNECTIONTCP_H
# define UCONNECTIONTCP_H

#include <ctime>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class UConnectionManager;

class UConnectionTCP : public boost::enable_shared_from_this<UConnectionTCP>, private boost::noncopyable
{
public:
  explicit UConnectionTCP(boost::asio::io_service& io_service,
      UConnectionManager& manager);
  boost::asio::ip::tcp::socket& socket();
  void start();
  void stop();
  const std::string &getIp() const;
  int receive_message(const std::string &ip, const std::string &msg);

  typedef boost::shared_ptr<UConnectionTCP> connection_ptr;
private:
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);
  void handle_write(const boost::system::error_code& e);

  bool                          isCalling;
  boost::asio::ip::tcp::socket  socket_;
  UConnectionManager&           connection_manager_;
  boost::asio::streambuf        buffer_;
  std::string                   ip_;
};

#endif
