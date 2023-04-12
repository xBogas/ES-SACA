#include "../gui/mainwindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <mutex>

using boost::asio::ip::tcp;

std::vector<std::string> connected_clients;
std::mutex connected_clients_mutex;

void handle_client(tcp::socket&& socket, MainWindow* window);
void server_thread(MainWindow *window);

int main(int argc, char *argv[])
{
    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //start server thread
    std::thread server(server_thread, &w);

    return a.exec();
}

void server_thread(MainWindow *window)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("10.0.2.15"), 8080));

        std::cout << "Server started. Listening on port 8080..." << std::endl;

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string client_ip = socket.remote_endpoint().address().to_string();
            std::cout << "Client connected: " << client_ip << std::endl;

            {
                std::lock_guard<std::mutex> lock(connected_clients_mutex);
                connected_clients.push_back(client_ip);
            }

            // update GUI with new list of connected clients
            std::vector<std::string> clients;
            {
                std::lock_guard<std::mutex> lock(connected_clients_mutex);
                clients = connected_clients;
            }
            window->updateClientList(clients);

            // handle the client in a separate thread
            std::thread client_thread(handle_client, std::move(socket), window);
            client_thread.detach();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handle_client(tcp::socket&& socket, MainWindow* window)
{
    try
    {
        boost::system::error_code error;
        for (;;)
        {
            char data[1024];
            size_t length = socket.read_some(boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof)
            {
                std::string client_ip = socket.remote_endpoint().address().to_string();
                std::cout << "Client disconnected: " << client_ip << std::endl;

                {
                    std::lock_guard<std::mutex> lock(connected_clients_mutex);
                    connected_clients.erase(std::remove(connected_clients.begin(), connected_clients.end(), client_ip), connected_clients.end());
                }

                // update GUI with new list of connected clients
                std::vector<std::string> clients;
                {
                    std::lock_guard<std::mutex> lock(connected_clients_mutex);
                    clients = connected_clients;
                }
                window->updateClientList(clients);

                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }

            std::string message(data, length);
            std::cout << "Received message from client " << socket.remote_endpoint().address().to_string() << ": " << message << std::endl;

            boost::asio::write(socket, boost::asio::buffer("Received message."));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}