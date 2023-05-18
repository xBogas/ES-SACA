#include "TCPsocket.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>

#define ERROR -1

namespace Network
{
	TCPsocket::TCPsocket()
	  : handle(ERROR)
	{
		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (handle == ERROR)
			throw std::runtime_error("[TCPsocket] failed to create socket");
	}

	TCPsocket::~TCPsocket()
	{
		shutdown(handle, SHUT_RDWR);
		close(handle);
	}

	void TCPsocket::setPort(uint16_t port, bool accept, AddressType address)
	{
		sockaddr_in addr;
		addr.sin_family 		= AF_INET;
		addr.sin_addr.s_addr 	= (address == AddressType::Any) ? INADDR_ANY : INADDR_BROADCAST;
		addr.sin_port 			= htons(port);
		
		socklen_t len = sizeof(addr);

		if(ERROR == ::bind(handle, (struct sockaddr*)&addr, len))
			throw std::runtime_error("Failed to bind socket");

		if (::listen(handle, 1) < 0)
			throw std::runtime_error("Unable to listen on socket");

		if(accept)
		{
			handle = ::accept(handle, ( struct sockaddr *)&addr, &len);
			if (handle == ERROR)
				throw std::runtime_error("[TCPsocket] failed to accept connection");
		}
	}

	void TCPsocket::accept()
	{

	}

	void TCPsocket::connect(uint16_t port, const char *addr)
	{
		sockaddr_in other;
		other.sin_family = AF_INET;
		other.sin_port = htons(port);

		if (inet_pton(AF_INET, addr, &other.sin_addr) <= 0)
			throw std::runtime_error("[TCPsocket] failed to convert addr");
		
		if(::connect(handle, (struct sockaddr*)&other, sizeof(other)) < 0)
			throw std::runtime_error("[TCPsocket] unable to connect");
	}
	
	int TCPsocket::read(void *buff, int size)
	{
		int rv = ::recv(handle, buff, size, 0);
		if(ERROR == rv)
			throw std::runtime_error("Failed to read socket");
		
		return rv;
	}

	void TCPsocket::write(const void* msg, int size)
	{
		if(ERROR == send(handle, msg, size, 0))
			throw std::runtime_error("Failed to send msg");
	}
}
