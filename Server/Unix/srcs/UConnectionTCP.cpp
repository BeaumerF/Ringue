#include "UConnectionManager.h"
#include "UConnectionTCP.h"

UConnectionTCP::UConnectionTCP(boost::asio::io_service& io_service,
    UConnectionManager& manager)
  : isCalling(false),
    socket_(io_service),
    connection_manager_(manager)
{}

boost::asio::ip::tcp::socket& UConnectionTCP::socket()
{
  return socket_;
}

void UConnectionTCP::start()
{
  ip_ = socket_.remote_endpoint().address().to_string();
  boost::asio::async_read_until(socket_, buffer_, "\r\n",
      boost::bind(&UConnectionTCP::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void UConnectionTCP::stop()
{
  socket_.close();
}

void UConnectionTCP::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  std::istream is(&buffer_);
  std::string  line;
  std::getline(is, line);
  std::cout << "READ > " << line << std::endl;

  std::string first_arg;
  std::string Response;

  if (!e && bytes_transferred)
  {
    first_arg = line.substr(0, line.find_first_of(' '));
    if (first_arg == "CONNECT")
    {
      connection_manager_.addContact(ip_, line.substr(line.find_first_of(' ') + 1));
      Response = "100 connected\r\n";
      boost::asio::async_write(socket_, boost::asio::buffer(Response),
            boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
              boost::asio::placeholders::error));
    }
    else if (line == "GETADDRESSES\r")
    {
      Response = "101 Address IP:\n";
      std::list<Contact> _ip_adresses = connection_manager_.getContacts();
      std::list<Contact>::iterator p = _ip_adresses.begin();
      while (p != _ip_adresses.end())
      {
        if (p->ip == ip_)
        {
          p++;
          continue;
        }
        Response += p->ip;
        Response += " ";
        Response += p->name;
        Response += "\n";
        p++;
      }
      Response += "101 End\r\n";
      boost::asio::async_write(socket_, boost::asio::buffer(Response),
      boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
      boost::asio::placeholders::error));
    }
    else if (first_arg == "CALL")
    {
      if (connection_manager_.send_message(ip_, line.substr(line.find_first_of(' ') + 1,
      line.size() - line.find_first_of(' ') - 2), first_arg) == 0)
      {
        isCalling = true;
        Response = "200 Success\r\n";
        boost::asio::async_write(socket_, boost::asio::buffer(Response),
        boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
      }
      else
      {
        Response = "501 Client not found\r\n";
        boost::asio::async_write(socket_, boost::asio::buffer(Response),
        boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
      }
    }
    else if (first_arg == "STOP")
    {
      if (connection_manager_.send_message(ip_, line.substr(line.find_first_of(' ') + 1,
      line.size() - line.find_first_of(' ') - 2), first_arg) == 0)
      {
        isCalling = false;
        Response = "200 Success\r\n";
        boost::asio::async_write(socket_, boost::asio::buffer(Response),
        boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
      }
      else
      {
        isCalling = false;
        Response = "501 Client not found\r\n";
        boost::asio::async_write(socket_, boost::asio::buffer(Response),
        boost::bind(&UConnectionTCP::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
      }
    }
  }
  else if (e || !bytes_transferred)
  {
    std::cout << "stop read" << std::endl;
    connection_manager_.stop(shared_from_this());
  }
}

void UConnectionTCP::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
    start();
  }
  else if (e)
  {
    std::cout << "stop write" << std::endl;
    connection_manager_.stop(shared_from_this());
  }
}

int UConnectionTCP::receive_message(const std::string &ip, const std::string &msg)
{
  if (msg == "CALL" && isCalling == true)
    return (1);
  std::string Response;

  if (msg == "CALL")
  {
    std::cout << "call" << std::endl;
    Response = "CALL " + ip + "\r\n";
    boost::asio::write(socket_, boost::asio::buffer(Response));
    isCalling = true;
    return (0);
  }
  else if (msg == "STOP")
  {
    std::cout << "stop" << std::endl;
    Response = "STOP\r\n";
    boost::asio::write(socket_, boost::asio::buffer(Response));
    isCalling = false;
    return (0);
  }
  return (1);
}

const std::string &UConnectionTCP::getIp() const
{
  return (ip_);
}


/*
void UUConnectionTCPTCP::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
  //std::cout << "msg receive: " << buf.data();
  std::cout << "msg RCV: " << make_string(_message);
  std::cout << "READ bytes : " << bytes_transferred << " error : " << error.message() << std::endl;

  // Response
  size_t                 pos;
  Fiche                  fiche;
  std::string            msg;
  std::string            arg;

  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  std::ostream msg_stream(&_message);

  msg = make_string(_message);
  pos = msg.find_first_of(' ');
  arg = msg.substr(0, pos);
  if (arg == "CONNECT")
  {
    std::cout << "rcv CONNECT" << std::endl;
    fiche.ip = _socket.remote_endpoint().address().to_string();
    fiche.name = msg.substr(pos + 1);
    pos = fiche.name.find_first_of('\n');
    fiche.name = fiche.name.substr(0, pos);
    request_stream << "100 Connected\r\n";
    boost::asio::async_write(_socket, request,
      boost::bind(&UUConnectionTCPTCP::handle_write, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    //boost::asio::write(_socket, boost::asio::buffer(make_string(request), sizeof(make_string(request))));
    _ip_adresses.push_front(fiche);
    std::cout << "msg sent: " << make_string(request);
    std::cout << "add: ip: " << fiche.ip << ", name: " << fiche.name << std::endl;
  }
  else if (make_string(_message) == "GETADDRESSES\r\n")
  {
    std::cout << "rcv GETADDRESSES" << std::endl;
    request_stream << "101 IP Adresses:\n";
    std::list<Fiche>::iterator p = _ip_adresses.begin();
    while (p != _ip_adresses.end())
    {
      if (p->ip == _socket.remote_endpoint().address().to_string())
      {
        p++;
        continue;
      }
      request_stream << p->ip << " ";
      request_stream << p->name << std::endl;
      p++;
    }
    request_stream << "101 End\r\n";
    boost::asio::async_write(_socket, request,
      boost::bind(&UUConnectionTCPTCP::handle_write, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    //boost::asio::write(_socket, boost::asio::buffer(make_string(request), sizeof(make_string(request))));
    std::cout << "msg SENT: " << make_string(request) << std::endl;
  }
  msg_stream.clear();
 }
*/
