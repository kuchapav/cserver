#include "cserver.h"

using boost::asio::ip::udp;

cserver::cserver(boost::asio::io_service& io_service, position init)
    : socket_(io_service, udp::endpoint(udp::v4(), 13)),
      pos(init)
    {
      start_receive();
    }

void cserver::start_receive()
  {
    socket_.async_receive_from(
          boost::asio::buffer(recv_buffer_), remote_endpoint_,
          boost::bind(&cserver::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

void cserver::handle_receive(const boost::system::error_code& error, std::size_t size)
  {
    if (!error || error == boost::asio::error::message_size)
      {
        unsigned long TOid_;
        unsigned long time_;
        imr::STrackedObject tObj_;

        std::tie(TOid_, time_, tObj_) = *recv_buffer_.data();
        pos.TOid = TOid_;
        pos.time = time_;
        pos.trackedObj = tObj_;

        pos.printData();

        start_receive();
      }
  }

void cserver::handle_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t)
    {}