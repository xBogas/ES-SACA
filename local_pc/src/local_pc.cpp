#include "../gui/mainwindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

bool decideType = true;
bool decideMode = false;

void client_thread();

int main(int argc, char *argv[]){
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
    try{
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("10.0.2.15", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server." << std::endl;

        for(;;){
            if(decideType || decideMode){
                if(decideType){
                    // Receive reply from server
                    char type[1024];
                    size_t type_length = socket.read_some(boost::asio::buffer(type));

                    std::cout << "Server replied: ";
                    std::cout.write(type, type_length);
                    std::cout << std::endl;
                    
                    decideMode = true;
                    decideType = false;     
                }
                else if(decideMode){
                    // Receive reply from server
                    char mode[1024];
                    size_t mode_length = socket.read_some(boost::asio::buffer(mode));

                    std::cout << "Server replied: ";
                    std::cout.write(mode, mode_length);
                    std::cout << std::endl;

                    decideMode = false;   
                }
                
            }
            else{
                // Read input from user
                std::string message;
                std::cout << "Enter message: ";
                std::getline(std::cin, message);

                // Exit loop if something happens
                if (message == "quit") {
                    break;
                }

                // Send message to server
                boost::asio::write(socket, boost::asio::buffer(message));

                // Receive reply from server
                char reply[1024];
                size_t reply_length = socket.read_some(boost::asio::buffer(reply));

                std::cout << "Server replied: ";
                std::cout.write(reply, reply_length);
                std::cout << std::endl;
            }
        }

        // Close socket
        socket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return;
}
