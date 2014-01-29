#include "cserver.h"

using boost::asio::ip::udp;

cserver::cserver(boost::asio::io_service& io_service, position init)
    : socket_cam(io_service, udp::endpoint(udp::v4(), 122)),
      pos(init),
      socket_pos(io_service, udp::endpoint(udp::v4(), 123)),
      pos_sender(io_service, "191.168.1.103", "123")
    {
      start_receive();
    }

cserver::~cserver(void)
{

}

void cserver::start_receive()
  {
    socket_cam.async_receive_from(
          boost::asio::buffer(cam_recv_buffer), cam_remote_endpoint,
          boost::bind(&cserver::handle_cam_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

    socket_pos.async_receive_from(
          boost::asio::buffer(pos_recv_buffer), pos_remote_endpoint,
          boost::bind(&cserver::handle_pos_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

void cserver::handle_cam_receive(const boost::system::error_code& error, std::size_t size)
  {
    if (!error)
      {
        unsigned long TOid_;
        unsigned long time_;
        imr::STrackedObject tObj_;

        std::tie(TOid_, time_, tObj_) = *cam_recv_buffer.data();
        pos.TOid = TOid_;
        pos.time = time_;
        pos.trackedObj = tObj_;

        pos_sender.send_posData(pos);
        pos.printData();

        start_receive();
      }
  }

void cserver::handle_cam_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t)
    {}

void cserver::handle_pos_receive(const boost::system::error_code& error, std::size_t size)
  {
    if (!error)
      {
        // position pos_ = pos_recv_buffer;

        // pos_.printData();

        start_receive();
      }
  }

void cserver::handle_pos_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t)
    {}
