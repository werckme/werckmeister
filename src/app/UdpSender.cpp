#include "UdpSender.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

namespace ip = boost::asio::ip;

namespace funk
{
	UdpSender::UdpSender(const std::string &hostStr)
	{
		ip::udp::resolver resolver(_service);
		auto hostInfo = getHostInfo(hostStr);
		ip::udp::resolver::query query(ip::udp::v4(), std::get<0>(hostInfo), std::get<1>(hostInfo));
		_endpoint = *resolver.resolve(query);
	}
	void UdpSender::start()
	{
		_socket = std::move(SocketPtr(new Socket(_service)));
		_socket->open(ip::udp::v4());
	}
	void UdpSender::stop()
	{
		_socket->close();
	}
	void UdpSender::send(const char *bytes, size_t length)
	{
		if (!_socket || !_socket->is_open())
		{
			throw std::runtime_error("tried to send via a closed socked");
		}
		namespace pc = boost::asio::placeholders;
		_socket->send_to(boost::asio::buffer(bytes, length), _endpoint);
	}
	std::tuple<UdpSender::Host, UdpSender::Port>
	UdpSender::getHostInfo(const std::string &str) const
	{
		std::vector<std::string> strs;
		boost::split(strs, str, boost::is_any_of(":"));
		if (strs.size() < 2)
		{
			return std::make_tuple(str, "");
		}
		return std::make_tuple(strs[0], strs[1]);
	}
}
