#pragma once

#include <tuple>
#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <com/common.hpp>
#include <boost/core/noncopyable.hpp>
#include "IStringSender.hpp"

namespace funk
{
	/**
	 * test the connection using: socat UDP-RECV:$port STDOUT
	 */
	class UdpSender : boost::noncopyable, public app::IStringSender
	{
	public:
		void start();
		void stop();
		void send(const char *bytes, size_t length) override;

	private:
		typedef std::string Host;
		typedef std::string Port;
		std::tuple<Host, Port> getHostInfo(const std::string &str) const;
		typedef boost::asio::ip::udp::socket Socket;
		typedef std::unique_ptr<Socket> SocketPtr;
		typedef boost::asio::ip::udp::endpoint Endpoint;
		typedef std::unique_ptr<Endpoint> EndpointPtr;
		typedef boost::system::error_code ErrorCode;
		typedef boost::asio::io_service Service;
		Service _service;
		SocketPtr _socket;
		Endpoint _endpoint;

	protected:
	public:
		UdpSender(const std::string &host);
		virtual ~UdpSender() = default;
	};
}
