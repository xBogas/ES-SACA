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

// connection to server in central_pc
using boost::asio::ip::tcp;
boost::asio::io_context io_context;

// connection to ESP
int port = 80;
const char* addr = "192.168.4.1";

// handle decisions in client_thread
bool initial = true, decideType = false, decideMode = false, canStart = false, readSignal = false;
bool matchORfinalMode = false, trainMode = false;
bool matchORfinal = false, train = false;
bool entry = true;
bool isPistol = false, isRifle = false;

// finish threads
bool finishClientThread = false;
bool finishESPThread = false;

// detect the shot and pass the values to client_thread
bool continueReading = false;
int coordinateX, coordinateY;
double score;
bool shot = false;

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow, Detector *detector);
void handle_new_score(int x, int y, double radius, double score);
void handle_ESP_communication(Detector *detector, bool& finishESPThread, bool& continueRunning);

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    PistolWindow *ptl = w.getPistolWindow();
    RifleWindow *rfl = w.getRifleWindow();
    Detector *detector = new Detector(0, port, addr);
    
    //start threads
    std::thread client(client_thread, &w, ptl, rfl, detector);
    std::thread ESP(handle_ESP_communication, detector, std::ref(finishESPThread), std::ref(continueReading));

    //connect signals
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        io_context.stop();
        finishClientThread = true;
        finishESPThread = true;
        ESP.join();
        client.join();
    });

    // Connect the signal to a slot (a member function or a lambda)
    QObject::connect(detector, &Detector::new_score, [](int x, int y, double radius, double shotScore) {
        handle_new_score(x, y, radius, shotScore);
    });

    return a.exec();;
}

void handle_ESP_communication(Detector *detector, bool& finishESPThread, bool& continueReading){
    detector->onMain(finishESPThread, continueReading);
}

void handle_new_score(int x, int y, double radius, double shotScore){
    coordinateX = x;
    coordinateY = y;
    score = shotScore;
    shot = true;

    std::cout << "Shot detected in local_pc!" << std::endl;
    std::cout << "x: " << x << " y: " << y << " radius: " << radius << " score: " << score << std::endl;
}

void client_thread(MainWindow *window, PistolWindow *ptlwindow, RifleWindow *rflwindow, Detector *detector){
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
            char init[1024], athlete[1024], type[1024], mode[1024], backToType[1024], backToMode[1024];

            if(finishClientThread)
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
                    if (delimiter_pos != std::string::npos) {
                        name = athl.substr(0, delimiter_pos);
                        ID = std::stoi(athl.substr(delimiter_pos + 1));
                    }
                }
            }
            else if(decideType){
                // Receive reply from server
                size_t type_length = socket.read_some(boost::asio::buffer(type));
                std::cout << "Server replied: ";
                std::cout.write(type, type_length);
                std::cout << std::endl;

                if(std::strncmp(type, "backToInitial", std::strlen("backToInitial")) == 0){
                    initial = true;
                    decideType = false; 
                }

                if(std::strncmp(type, "pistol", std::strlen("pistol")) == 0){
                    emit window->openPistolWindowSignal();
                    ptlwindow->findChild<QLabel*>("name")->setText(QString::fromStdString(name));
                    ptlwindow->findChild<QLabel*>("ID")->setText(QString::number(ID));

                    detector->changeMode(0); //set the detector type to pistol
                    isPistol = true;
                    isRifle = false;
                    decideMode = true;
                    decideType = false; 
                }
                else if(std::strncmp(type, "rifle", std::strlen("rifle")) == 0){
                    emit window->openRifleWindowSignal();
                    rflwindow->findChild<QLabel*>("name")->setText(QString::fromStdString(name));
                    rflwindow->findChild<QLabel*>("ID")->setText(QString::number(ID));
                    
                    detector->changeMode(1); //set the detector type to rifle
                    isRifle = true;
                    isPistol = false;
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

                if(isPistol){
                    if(std::strncmp(mode, "backToDecideType", std::strlen("backToDecideType")) == 0){
                        emit ptlwindow->backToDecideTypeSignal();

                        matchORfinal = false;
                        train = false;
                        decideMode = false;
                        decideType = true;
                    }

                    if(std::strncmp(mode, "practice", std::strlen("practice")) == 0){
                        emit ptlwindow->practiceButtonClickedSignal();
                        
                        train = true;
                        matchORfinal = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "match", std::strlen("match")) == 0){
                        emit ptlwindow->matchButtonClickedSignal();
                        
                        matchORfinal = true;
                        train = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "final", std::strlen("final")) == 0){
                        emit ptlwindow->finalButtonClickedSignal();

                        matchORfinal = true;
                        train = false;
                        canStart = true;
                    }

                    if(canStart && std::strncmp(mode, "start", std::strlen("start")) == 0){
                        emit ptlwindow->startButtonClickedSignal();
                        continueReading = true;

                        if(matchORfinal){
                            matchORfinal = false;
                            matchORfinalMode = true;
                        }
                        else if(train){
                            train = false;
                            trainMode = true;
                        }
                        
                        canStart = false;
                        decideMode = false;
                    }
                }
                else if(isRifle){
                    if(std::strncmp(mode, "backToDecideType", std::strlen("backToDecideType")) == 0){
                        emit rflwindow->backToDecideTypeSignal();

                        matchORfinal = false;
                        train = false;
                        decideMode = false;
                        decideType = true;
                    }

                    if(std::strncmp(mode, "practice", std::strlen("practice")) == 0){
                        emit rflwindow->practiceButtonClickedSignal();
                        
                        train = true;
                        matchORfinal = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "match", std::strlen("match")) == 0){
                        emit rflwindow->matchButtonClickedSignal();

                        matchORfinal = true;
                        train = false;
                        canStart = true;
                    }
                    else if(std::strncmp(mode, "final", std::strlen("final")) == 0){
                        emit rflwindow->finalButtonClickedSignal();
                        
                        matchORfinal = true;
                        train = false;
                        canStart = true;
                    }

                    if(canStart && std::strncmp(mode, "start", std::strlen("start")) == 0){
                        emit rflwindow->startButtonClickedSignal();
                        continueReading = true;
                        
                        if(matchORfinal){
                            matchORfinal = false;
                            matchORfinalMode = true;
                        }
                        else if(train){
                            train = false;
                            trainMode = true;
                        }
                        
                        canStart = false;
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

                if((now_millis - initial_millis > delay_ms) and !shot){ // timeout
                    // Send message to server
                    boost::asio::write(socket, boost::asio::buffer("timeout"));

                    entry = true;
                    readSignal = true;
                }
                else if(shot){ // process vision_analysis
                    std::cout << "Shot detected" << std::endl;

                    // Send message to server
                    if(matchORfinalMode){
                        boost::asio::write(socket, boost::asio::buffer("shot ; "));
                    }
                    else if(trainMode){
                        boost::asio::write(socket, boost::asio::buffer("shotInTrain"));
                    }

                    if(isPistol){
                        emit ptlwindow->new_score(coordinateX, coordinateY, score);
                    }
                    else if(isRifle){
                        emit rflwindow->new_score(coordinateX, coordinateY, score);
                    }

                    readSignal = true;
                    shot = false;
                    continueReading = false;
                }

                if(readSignal){
                    // Receive reply from server
                    size_t backToMode_length = socket.read_some(boost::asio::buffer(backToMode));
                    std::cout << "Server replied: ";
                    std::cout.write(backToMode, backToMode_length);
                    std::cout << std::endl;

                    if(std::strncmp(backToMode, "backToDecideMode", std::strlen("backToDecideMode")) == 0){
                        if(isPistol){
                            emit ptlwindow->backToDecideModeSignal();
                        }
                        else if(isRifle){
                            emit rflwindow->backToDecideModeSignal();
                        }
                        
                        decideMode = true;
                        matchORfinalMode = false;
                        trainMode = false;
                        readSignal = false;
                        continueReading = false;
                    }
                    else if(std::strncmp(backToMode, "proceed", std::strlen("proceed")) == 0){
                        readSignal = false;
                        continueReading = true;
                    }
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

