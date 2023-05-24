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
		
		TCPsocket(bool non_block = true);

		~TCPsocket();

		// server side connection only accept one connection
		//! not implemented for non_blocking
		void setPort(uint16_t port,  bool accept = true, AddressType address = AddressType::Any);

		//! non implemented for non_blocking
		void accept();

		void connect(uint16_t port, const char* addr);

		int read(void* buff, int size);

		void write(const void* msg, int size);

		bool waitData();
		
	private:

		int handle;
	};
	
}

#endif