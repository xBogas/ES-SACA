#ifndef NETWORK_TCP_SOCKET_HPP
#define NETWORK_TCP_SOCKET_HPP

#include <netinet/in.h>

namespace Network
{
	class TCPsocket
	{
	public:

		enum AddressType
		{
			Any,
			Broadcast
		};
		
		TCPsocket();

		~TCPsocket();

		// server side connection only accept one connection
		void setPort(uint16_t port,  bool accept = true, AddressType address = AddressType::Any);

		void accept();

		void connect(uint16_t port, const char* addr);

		int read(void* buff, int size);

		void write(const void* msg, int size);
		
	private:

		int handle;
	};
	
}

#endif