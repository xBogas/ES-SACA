#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  boost::asio::io_context io_context;

  tcp::socket socket(io_context);
  tcp::resolver resolver(io_context);
  boost::asio::connect(socket, resolver.resolve("192.168.4.1", "80")); // ESP8266 IP address and port

  while (true) {
    std::string message;
    std::cout << "Enter message to send (or 'quit' to exit): ";
    std::getline(std::cin, message);

    if (message == "quit") {
      break;
    }

    message += "\n";
    boost::asio::write(socket, boost::asio::buffer(message));

    std::array<char, 128> buffer;
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(buffer), error);

    if (error == boost::asio::error::eof) {
      std::cout << "Connection closed by server." << std::endl;
      break;
    } else if (error) {
      std::cout << "Error: " << error.message() << std::endl;
      break;
    } else {
      std::cout << "Response: ";
      std::cout.write(buffer.data(), length);
      std::cout << std::endl;
    }
  }

  return 0;
}


