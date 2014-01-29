#include "udpSender.h"

udpSender::udpSender(boost::asio::io_service& io_service_, const std::string host_, const std::string port_) 
	: host(host_),
	  port(port_),
	  resolver(io_service_),
	  query(boost::asio::ip::udp::v4(), host_, port_),
	  receiver_endpoint(*resolver.resolve(query)),
	  socket(io_service_)
{
	socket.open(boost::asio::ip::udp::v4());
}

udpSender::~udpSender(void)
{

}

void udpSender::send_camData(std::tuple<unsigned long, unsigned long, imr::STrackedObject> data)
{
	boost::array<std::tuple<unsigned long, unsigned long, imr::STrackedObject>, 1> send_buf  = {{ data }};
	socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
}

void udpSender::send_posData(position data)
{
	boost::array<position, 1> send_buf  = {{ data }};
	socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
}