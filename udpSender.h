#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <tuple>
#include "tracked_object.h"

class udpSender
{
public:
	udpSender(boost::asio::io_service& io_service, const std::string host_, const std::string port_);
	~udpSender(void);
	void send(std::tuple<unsigned long, unsigned long, imr::STrackedObject> data);

private:
	const std::string host, port;
	boost::asio::ip::udp::resolver resolver;
    boost::asio::ip::udp::resolver::query query;
    boost::asio::ip::udp::endpoint receiver_endpoint;
	boost::asio::ip::udp::socket socket;
};


#endif // UDPSENDER_H