#include "UConnectionManager.h"

void UConnectionManager::start(UConnectionTCP::connection_ptr c)
{
  connections_.insert(c);
  c->start();
}

void UConnectionManager::stop(UConnectionTCP::connection_ptr c)
{
  std::list<Contact>::iterator it = contacts_.begin() ;

  while(it != contacts_.end())
  {
    if(it->ip == c->getIp())
    {
      it = contacts_.erase(it);
    }
    else
    {
      it++ ;
    }
  }
  connections_.erase(c);
  c->stop();
}

void UConnectionManager::stop_all()
{
  std::for_each(connections_.begin(), connections_.end(),
      boost::bind(&UConnectionTCP::stop, _1));
  connections_.clear();
}

void UConnectionManager::addContact(const std::string& ip, const std::string& name)
{
  Contact new_contact;

  new_contact.ip = ip;
  new_contact.name = name;
  contacts_.push_front(new_contact);
}

std::list<Contact> UConnectionManager::getContacts() const
{
  return (contacts_);
}

int UConnectionManager::send_message(const std::string &ip_sender, const std::string &ip_receiver, const std::string& msg)
{
  std::set<UConnectionTCP::connection_ptr>::iterator p = connections_.begin();
  while (p != connections_.end())
  {
    if ((*p)->socket().remote_endpoint().address().to_string() == ip_receiver)
    {
      std::cout << "IP found" << std::endl;
      return ((*p)->receive_message(ip_sender, msg));
    }
    p++;
  }
  std::cout << "IP not found" << std::endl;
  return (1);
}
