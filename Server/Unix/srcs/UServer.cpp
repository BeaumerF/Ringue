#include "UServer.h"

UServer::UServer(const std::string& address, const std::string& port)
  : io_service_(),
    signals_(io_service_),
    acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port))),
    connection_manager_(),
    new_connection_()
{
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
  signals_.async_wait(boost::bind(&UServer::handle_stop, this));

  (void)address;
//  boost::asio::ip::tcp::resolver resolver(io_service_);
//  boost::asio::ip::tcp::resolver::query query(address, port);
//  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
//  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.listen();
//  acceptor_.bind(endpoint);

  start_accept();
}

void UServer::run()
{
  io_service_.run();
}

void UServer::start_accept()
{
  new_connection_.reset(new UConnectionTCP(io_service_,
        connection_manager_));
  acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&UServer::handle_accept, this,
        boost::asio::placeholders::error));
}

void UServer::handle_accept()
{}

void UServer::handle_accept(const boost::system::error_code& e)
{
  std::cout << "New client connected" << std::endl;
  if (!acceptor_.is_open())
  {
    return;
  }

  if (!e)
  {
    connection_manager_.start(new_connection_);
  }

  start_accept();
}

void UServer::handle_stop()
{
  acceptor_.close();
  connection_manager_.stop_all();
}
