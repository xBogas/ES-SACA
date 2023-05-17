#include "TCPsocket.hpp"


#include <sys/socket.h>
//#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdexcept>

#define INVALID_SOCKET -1

namespace Network
{
	TCPsocket::TCPsocket()
	  : handle(INVALID_SOCKET)
	{
		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (handle == INVALID_SOCKET)
			throw std::runtime_error("[TCPsocket] failed to create socket");
	}

	TCPsocket::~TCPsocket()
	{
		shutdown(handle, SHUT_RDWR);
		close(handle);
	}

	void TCPsocket::bind(uint16_t port, AddressType address, bool reuse)
	{
		if (reuse)
		{
			int optval = 1;
			setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));
		}
		
		sockaddr_in _this;
		_this.sin_family 		= AF_INET;
		_this.sin_addr.s_addr 	= (address == AddressType::Any) ? INADDR_ANY : INADDR_BROADCAST;
		_this.sin_port 			= htons(port);
		
		if(::bind(handle, (struct sockaddr*)&_this, sizeof(_this)))
			throw std::runtime_error("[TCPsocket] failed to bind socket");
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

	void TCPsocket::listen(int _n)
	{
		if (::listen(handle, _n) < 0)
			throw std::runtime_error("[TCPsocket] unable to listen on socket");
	}
}
