#ifndef UCONNECTIONMANAGER_H
# define  UCONNECTIONMANAGER_H

#include <set>
#include <list>

#include "UConnectionTCP.h"
#include "Contact.h"

class UConnectionManager : private boost::noncopyable
{
public:
  void start(UConnectionTCP::connection_ptr c);
  void stop(UConnectionTCP::connection_ptr c);
  void stop_all();
  void addContact(const std::string& ip, const std::string& name);
  std::list<Contact> getContacts() const;
  int send_message(const std::string &ip_sender, const std::string &ip_receiver, const std::string& msg);
private:
  std::set<UConnectionTCP::connection_ptr>  connections_;
  std::list<Contact>                        contacts_;
};

#endif
