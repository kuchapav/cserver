#ifndef CSERVER_H
#define CSERVER_H

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <tuple>
#include "tracked_object.h"
#include "position.h"

using boost::asio::ip::udp;

class cserver
{
public:
  cserver(boost::asio::io_service& io_service, position init);

private:
  void start_receive();
  void handle_receive(const boost::system::error_code& error, std::size_t size);
  void handle_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t size);

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<std::tuple<unsigned long, unsigned long, imr::STrackedObject>, 1> recv_buffer_;
  position pos;
};


#endif // CSERVER_H