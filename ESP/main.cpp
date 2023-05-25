#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  boost::asio::io_context io_context;

  tcp::socket socket(io_context);
  tcp::resolver resolver(io_context);
  boost::asio::connect(socket, resolver.resolve("192.168.4.1", "80")); // ESP8266 IP address and port

  boost::asio::write(socket, boost::asio::buffer(""));

  while (true) {
    char buffer[1024];
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(buffer), error);

    if (error == boost::asio::error::eof) {
      std::cout << "Connection closed by server." << std::endl;
      break;
    } else if (error) {
      std::cout << "Error: " << error.message() << std::endl;
      break;
    } else {
      std::string message(buffer, length);
      std::cout << "Response: " << message << std::endl;
    }

    if(std::strncmp(buffer, "disparo", std::strlen("disparo")) == 0){

      boost::asio::write(socket, boost::asio::buffer("anda"));
    }
    else{
      boost::asio::write(socket, boost::asio::buffer("erro"));
    }
  }

  return 0;
}


