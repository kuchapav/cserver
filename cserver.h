#ifndef CSERVER_H
#define CSERVER_H

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <tuple>
#include "tracked_object.h"
#include "position.h"
#include "udpSender.h"

using boost::asio::ip::udp;

class cserver
{
public:
  cserver(boost::asio::io_service& io_service, position init);
  ~cserver(void);

private:
  void start_receive();
  void handle_cam_receive(const boost::system::error_code& error, std::size_t size);
  void handle_pos_receive(const boost::system::error_code& error, std::size_t size);
  void handle_cam_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t size);
  void handle_pos_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t size);

  udp::socket socket_cam;
  udp::socket socket_pos;
  udp::endpoint cam_remote_endpoint, pos_remote_endpoint;
  boost::array<std::tuple<unsigned long, unsigned long, imr::STrackedObject>, 1> cam_recv_buffer;
  boost::array<position, 1> pos_recv_buffer;
  position pos;
  udpSender pos_sender;
};


#endif // CSERVER_H