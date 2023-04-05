#include "../gui/mainwindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void server_thread();

int main(int argc, char *argv[])
{
    //start client thread
    std::thread server(server_thread);

    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    server.join();

    return a.exec();
}

void server_thread(){
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("10.0.2.15"), 8080));

        std::cout << "Server started. Listening on port 8080..." << std::endl;

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Client connected: " << socket.remote_endpoint().address().to_string() << std::endl;

            boost::system::error_code error;
            for (;;)
            {
                char data[1024];
                size_t length = socket.read_some(boost::asio::buffer(data), error);

                if (error == boost::asio::error::eof)
                {
                    std::cout << "Client disconnected." << std::endl;
                    break;
                }
                else if (error)
                {
                    throw boost::system::system_error(error);
                }

                std::string message(data, length);
                std::cout << "Received message from client: " << message << std::endl;

                boost::asio::write(socket, boost::asio::buffer("Received message."));
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return;
}