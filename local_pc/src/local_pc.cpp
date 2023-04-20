#include "../gui/mainwindow.h"
#include "../gui/pistolwindow.h"
#include "../gui/riflewindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

bool initial = true, decideType = false, decideMode = false, canStart = false;
bool matchORfinal = false;

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow);

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    PistolWindow *ptl = w.getPistolWindow();
    RifleWindow *rfl = w.getRifleWindow();
    
    //start client thread
    std::thread client(client_thread, &w, ptl, rfl);

    return a.exec();;
}

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow){
    try{
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("10.0.2.15", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server." << std::endl;

        for(;;){
            char init[1024];
            char type[1024];
            char mode[1024];

            if(initial){
                // Receive reply from server
                size_t init_length = socket.read_some(boost::asio::buffer(init));
                std::cout << "Server replied: ";
                std::cout.write(init, init_length);
                std::cout << std::endl;

                if (std::strncmp(init, "continue", std::strlen("continue")) == 0){
                    decideType = true;
                    initial = false;
                }
            }
            else if(decideType){
                // Receive reply from server
                size_t type_length = socket.read_some(boost::asio::buffer(type));
                std::cout << "Server replied: ";
                std::cout.write(type, type_length);
                std::cout << std::endl;

                if(std::strncmp(type, "pistol", std::strlen("pistol")) == 0){
                    emit window->openPistolWindowSignal();

                    decideMode = true;
                    decideType = false; 
                }
                else if(std::strncmp(type, "rifle", std::strlen("rifle")) == 0){
                    emit window->openRifleWindowSignal();

                    decideMode = true;
                    decideType = false; 
                }    
            }
            else if(decideMode){
                // Receive reply from server
                size_t mode_length = socket.read_some(boost::asio::buffer(mode));
                std::cout << "Server replied: ";
                std::cout.write(mode, mode_length);
                std::cout << std::endl;

                if(std::strncmp(type, "pistol", std::strlen("pistol")) == 0){
                    if(std::strncmp(mode, "practice", std::strlen("practice")) == 0){
                        emit ptlwindow->practiceButtonClickedSignal();

                        matchORfinal = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "match", std::strlen("match")) == 0){
                        emit ptlwindow->matchButtonClickedSignal();
                        
                        matchORfinal = true;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "final", std::strlen("final")) == 0){
                        emit ptlwindow->finalButtonClickedSignal();

                        matchORfinal = true;
                        canStart = true;
                    }

                    if(canStart && std::strncmp(mode, "start", std::strlen("start")) == 0){
                        emit ptlwindow->startButtonClickedSignal();
                        
                        canStart = false;

                        if(matchORfinal)
                            decideMode = false;
                    }
                }
                else if(std::strncmp(type, "rifle", std::strlen("rifle")) == 0){
                    if(std::strncmp(mode, "practice", std::strlen("practice")) == 0){
                        emit rflwindow->practiceButtonClickedSignal();
                        
                        matchORfinal = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "match", std::strlen("match")) == 0){
                        emit rflwindow->matchButtonClickedSignal();

                        matchORfinal = true;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "final", std::strlen("final")) == 0){
                        emit rflwindow->finalButtonClickedSignal();
                        
                        matchORfinal = true;
                        canStart = true;
                    }

                    if(canStart && std::strncmp(mode, "start", std::strlen("start")) == 0){
                        emit ptlwindow->startButtonClickedSignal();
                        
                        canStart = false;

                        if(matchORfinal)
                            decideMode = false;
                    }
                }

                // if(canStart && std::strncmp(mode, "start", std::strlen("start")) == 0){
                //     if(std::strncmp(type, "pistol", std::strlen("pistol")) == 0){
                //         emit ptlwindow->startButtonClickedSignal();
                        
                //         if(std::strncmp(mode, "match", std::strlen("match")) == 0)
                //             decideMode = false;
                //     }
                //     else if(std::strncmp(type, "rifle", std::strlen("rifle")) == 0){
                //         emit rflwindow->startButtonClickedSignal();

                //         if(std::strncmp(mode, "match", std::strlen("match")) == 0)
                //             decideMode = false;
                //     }
                // }
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
