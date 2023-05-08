#include "../gui/mainwindow.h"
#include "../gui/pistolwindow.h"
#include "../gui/riflewindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <boost/asio.hpp>

#include <opencv2/opencv.hpp>
#include "../vision_analysis/src/Detector.hpp"
#include <fstream>

using boost::asio::ip::tcp;
boost::asio::io_context io_context;

bool initial = true, decideType = false, decideMode = false, canStart = false;
bool matchORfinal = false;
bool entry = true;
bool finish = false;
bool electretSignal = false;

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow);
void handle_ESP_communication();

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    PistolWindow *ptl = w.getPistolWindow();
    RifleWindow *rfl = w.getRifleWindow();
    
    //start threads
    std::thread client(client_thread, &w, ptl, rfl);
    // std::thread ESP(handle_ESP_communication);

    //connect signals
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        io_context.stop();
        finish = true;
        client.join();
    });

    return a.exec();;
}

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow){
    try{
        long long initial_millis;
        int delay_ms;

        tcp::resolver resolver(io_context);
        //tcp::resolver::results_type endpoints = resolver.resolve("192.168.0.1", "8080");
        tcp::resolver::results_type endpoints = resolver.resolve("10.0.2.15", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server." << std::endl;
        
        std::string name;
        int ID;

        for(;;){
            char init[1024], athlete[1024], type[1024], mode[1024];

            if(finish)
                break;
            
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
                else{
                    std::strcpy(athlete, init);
                    std::string athl(athlete);

                    size_t delimiter_pos = athl.find(';');
                    name = athl.substr(0, delimiter_pos);
                    ID = std::stoi(athl.substr(delimiter_pos + 1));
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
                    ptlwindow->findChild<QLabel*>("name")->setText(QString::fromStdString(name));
                    ptlwindow->findChild<QLabel*>("ID")->setText(QString::number(ID));

                    decideMode = true;
                    decideType = false; 
                }
                else if(std::strncmp(type, "rifle", std::strlen("rifle")) == 0){
                    emit window->openRifleWindowSignal();
                    rflwindow->findChild<QLabel*>("name")->setText(QString::fromStdString(name));
                    rflwindow->findChild<QLabel*>("ID")->setText(QString::number(ID));

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
                        emit rflwindow->startButtonClickedSignal();
                        
                        canStart = false;

                        if(matchORfinal)
                            decideMode = false;
                    }
                }
            }
            else{
                if(entry){
                    auto initial = std::chrono::system_clock::now();
                    auto initial_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(initial);
                    initial_millis = initial_ms.time_since_epoch().count();

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(5000, 8000); //random time between 5000 and 8000 milliseconds
                    delay_ms = dis(gen);
                    //delay_ms = 5000;

                    entry = false;
                }

                auto now = std::chrono::system_clock::now();
                auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
                long long now_millis = now_ms.time_since_epoch().count();

                electretSignal = ptlwindow->electretSignal;
                if((now_millis - initial_millis > delay_ms) and !electretSignal){ // timeout
                    // Send message to server
                    boost::asio::write(socket, boost::asio::buffer("timeout"));

                    // Receive reply from server
                    char reply[1024];
                    size_t reply_length = socket.read_some(boost::asio::buffer(reply));

                    std::cout << "Server replied: ";
                    std::cout.write(reply, reply_length);
                    std::cout << std::endl;

                    entry = true;
                }
                else if(electretSignal){ // process vision_analysis
                    std::cout << "ElectretSignal Activated" << std::endl;

                    ptlwindow->electretSignal = false;
                }
                
            }
        }

        // Close socket
        socket.close();
        std::cout << "Socket closed" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return;
}

void handle_ESP_communication(){
    return;
}
