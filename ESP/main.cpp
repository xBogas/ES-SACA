#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  boost::asio::io_context io_context;

  tcp::socket socket(io_context);
  tcp::resolver resolver(io_context);
  boost::asio::connect(socket, resolver.resolve("192.168.4.1", "80")); // ESP8266 IP address and port

  boost::asio::write(socket, boost::asio::buffer("\n"));
  bool flag = true;
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
      auto initial = std::chrono::system_clock::now();
      auto initial_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(initial);
      long long initial_millis = initial_ms.time_since_epoch().count();

      while(1){

        std::cout << "Aqui no while" << std::endl;
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        long long now_millis = now_ms.time_since_epoch().count();

        if(now_millis - initial_millis > 1000) break;
      }

      if(flag){

      }
    
      std::string msg; 
      if(flag){
        std::cout << "Atua motoooor" << std::endl;
        double move_ESP = 122.3444;
        msg = "Move " + std::to_string((int) move_ESP) + "\n";

        flag = false;
      }
      else{
        std::cout << "Para motoooor" << std::endl;
        msg = "DontMove\n";

        flag = true;
      }

      std::cout << msg << std::endl;
      boost::asio::write(socket, boost::asio::buffer(msg));
    }
  }

  return 0;
}


