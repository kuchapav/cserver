#include "udpSender.h"

udpSender::udpSender(boost::asio::io_service& io_service_, const std::string host_) 
	: host(host_),
	  resolver(io_service_),
	  query(boost::asio::ip::udp::v4(), host_, "daytime"),
	  receiver_endpoint(*resolver.resolve(query)),
	  socket(io_service_)
{
	socket.open(boost::asio::ip::udp::v4());
}

udpSender::~udpSender(void)
{

}

void udpSender::send(std::tuple<unsigned long, unsigned long, imr::STrackedObject> data)
{
	std::cout << "Something sexy" << std::endl;
	boost::array<std::tuple<unsigned long, unsigned long, imr::STrackedObject>, 1> send_buf  = {{ data }};
	socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
}