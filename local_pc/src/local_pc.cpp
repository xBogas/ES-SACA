#include "../gui/mainwindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void client_thread();

int main(int argc, char *argv[])
{
    //start client thread
    std::thread client(client_thread);

    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();

    client.join();

    return 0;
}

void client_thread(){
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("192.168.0.1", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server." << std::endl;

        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        boost::asio::write(socket, boost::asio::buffer(message));

        char reply[1024];
        size_t reply_length = socket.read_some(boost::asio::buffer(reply));

        std::cout << "Server replied: ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return;
}
