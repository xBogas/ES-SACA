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

		void bind(uint16_t port, AddressType address, bool reuse = true);

		void connect(uint16_t port, const char* addr);

		void listen(int _n);
	private:
		int handle;
	};
	
}
