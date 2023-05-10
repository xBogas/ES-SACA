#include "../gui/initwindow.h"
#include "../gui/mainwindow.h"
#include "../gui/mainwindow2.h"
#include "../gui/pistolwindow.h"
#include "../gui/riflewindow.h"
#include "../database/database.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <mutex>

using boost::asio::ip::tcp;
boost::asio::io_context io_context;

std::vector<std::string> connected_clients;
std::mutex connected_clients_mutex;
std::string type;

bool ready = false;
int totalClients = 0;
int checkedClients = 0;
std::mutex barrierMutex;
std::condition_variable barrierCV;

void handle_client(boost::asio::ip::tcp::socket&& socket, MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow, Database* database);
void server_thread(MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow, Database* database);
void waitForOthers();

bool initial = true, decideType = false, decideMode = false, canStart = false;
bool wasClicked = false;
bool newPractice = false, newMatch = false, newFinal = false;
bool oldPractice = false, oldFinal = false, oldMatch = false;
bool oldStart = false;
bool isPistol = false, isRifle = false;
bool matchORfinal = false;

int aux = 0;

std::atomic<bool> finish(false);

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);

    //create database
    Database* database = new Database();
    
    //create windows
    InitWindow i = InitWindow(nullptr, database);  
    i.show();
    MainWindow* w = i.getMainWindow();
    MainWindow2* w2 = w->getMainWindow2();
    RifleWindow* rfl = w2->getRifleWindow();
    PistolWindow* ptl = w2->getPistolWindow();

    //start threads
    std::thread server(server_thread, w, w2, ptl, rfl, database);

    //connect signals
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        io_context.stop();
        finish = true;
        server.join();
    });

    return a.exec();
}

void start_accept(tcp::acceptor& acceptor, MainWindow* window, MainWindow2* window2, PistolWindow* ptlwindow, RifleWindow* rflwindow, Database* database) {
    acceptor.async_accept(
        [window, window2, ptlwindow, rflwindow, database, &acceptor](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::string client_ip = socket.remote_endpoint().address().to_string();

                /*****************************************/
                aux++;
                boost::asio::ip::address_v4 ip = boost::asio::ip::address_v4::from_string(client_ip);
                uint32_t ip_num = ip.to_ulong();
                ip_num += aux;
                boost::asio::ip::address_v4 new_ip(ip_num);
                client_ip = new_ip.to_string();
                /*****************************************/

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
                std::thread client_thread(handle_client, std::move(socket), window, window2, ptlwindow, rflwindow, database);
                client_thread.detach();
            }

            // call start_accept again to wait for the next connection
            start_accept(acceptor, window, window2, ptlwindow, rflwindow, database);
        });
}

void server_thread(MainWindow* window, MainWindow2* window2, PistolWindow* ptlwindow, RifleWindow* rflwindow, Database* database) {
    try {
        tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("10.0.2.15"), 8080));
        //tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("192.168.0.1"), 8080));

        std::cout << "Server started. Listening on port 8080..." << std::endl;

        // start accepting connections
        start_accept(acceptor, window, window2, ptlwindow, rflwindow, database);

        // run the I/O service event loop to handle async_accept operation
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handle_client(boost::asio::ip::tcp::socket&& socket, MainWindow* window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow, Database* database){
    try{
        std::string client_ip = socket.remote_endpoint().address().to_string();
        int old_clientID = 0;

        
        /*****************************************/
        boost::asio::ip::address_v4 ip = boost::asio::ip::address_v4::from_string(client_ip);
        uint32_t ip_num = ip.to_ulong();
        ip_num += aux;
        boost::asio::ip::address_v4 new_ip(ip_num);
        client_ip = new_ip.to_string();
        /*****************************************/

        while(!finish){
            if(initial){
                if(window->isMainWindow){
                    if(window->cellWasChanged[client_ip]){
                        int new_clientID = window->clientPlayerIds[client_ip];

                        if(database->verify_id(new_clientID) && !window->samePlayerIds[client_ip]){
                            std::string name = database->get_name_from_id(new_clientID);
                            std::string athlete = name + ";" + std::to_string(new_clientID);
                            boost::asio::write(socket, boost::asio::buffer(athlete));

                            old_clientID = new_clientID;
                            window->nonPlayerIds[client_ip] = false;
                        }
                        else if(!database->verify_id(new_clientID) && !window->samePlayerIds[client_ip]){
                            emit window->showErrorMessageSignal("nonID");
                            
                            window->nonPlayerIds[client_ip] = true;
                        } 
                        else if(window->samePlayerIds[client_ip]){
                            emit window->showErrorMessageSignal("sameID");

                            window->nonPlayerIds[client_ip] = false;
                        }
                            
                        window->cellWasChanged[client_ip] = false;
                    }
                }                                                    
                else{
                    waitForOthers();
                    
                    boost::asio::write(socket, boost::asio::buffer("continue"));

                    decideType = true;
                    initial = false;
                }
            }
            else if(decideType){
                if(window2->pistol){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("pistol"));

                    isPistol = true;
                    isRifle = false;
                    decideMode = true;
                    decideType = false;
                    window2->pistol = false;
                } 
                else if(window2->rifle){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("rifle"));

                    isRifle = true;
                    isPistol = false;
                    decideMode = true;
                    decideType = false;
                    window2->rifle = false;
                } 
            }
            else if(decideMode){
                if(isPistol){
                    newPractice = ptlwindow->practiceSignal;
                    newMatch = ptlwindow->matchSignal;
                    newFinal = ptlwindow->finalSignal;

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        matchORfinal = false;
                        ptlwindow->practiceSignal = false;
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        matchORfinal = true;
                        ptlwindow->matchSignal = false;
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        matchORfinal = true;
                        ptlwindow->finalSignal = false;
                        canStart = true;
                    }

                    oldPractice = newPractice;
                    oldFinal = newFinal;
                    oldMatch = newMatch;

                    if(canStart && ptlwindow->startSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("start"));

                        ptlwindow->startSignal = false;

                        if(matchORfinal){
                            decideMode = false;
                            canStart = false;
                            oldPractice = false;
                            oldFinal = false;
                            oldMatch = false;
                            matchORfinal = false;
                        }
                    }

                    if(canStart && ptlwindow->switchModeSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideMode"));

                        emit ptlwindow->backToDecideModeSignal();
                        
                        decideMode = true;
                        ptlwindow->switchModeSignal = false;
                        oldPractice = false;
                        oldFinal = false;
                        oldMatch = false;
                        canStart = false;
                        matchORfinal = false;
                    }
                }
                else if(isRifle){
                    newPractice = rflwindow->practiceSignal;
                    newMatch = rflwindow->matchSignal;
                    newFinal = rflwindow->finalSignal;

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        matchORfinal = false;
                        rflwindow->practiceSignal = false;
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        matchORfinal = true;
                        rflwindow->matchSignal = false;
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        matchORfinal = true;
                        rflwindow->finalSignal = false;
                        canStart = true;
                    }

                    oldPractice = newPractice;
                    oldFinal = newFinal;
                    oldMatch = newMatch;

                    if(canStart && rflwindow->startSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("start"));
   
                        rflwindow->startSignal = false;

                        if(matchORfinal){
                            decideMode = false;
                            canStart = false;
                            oldPractice = false;
                            oldFinal = false;
                            oldMatch = false;
                            matchORfinal = false;
                        }
                    }

                    if(canStart && rflwindow->switchModeSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideMode"));

                        emit rflwindow->backToDecideModeSignal();
     
                        decideMode = true;
                        rflwindow->switchModeSignal = false;
                        oldPractice = false;
                        oldFinal = false;
                        oldMatch = false;
                        matchORfinal = false;
                    }
                }
            }
            else{
                boost::system::error_code ec;
                char data[1024];
                size_t length = socket.read_some(boost::asio::buffer(data), ec);

                if (ec == boost::asio::error::eof){
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
                else if (ec){
                    throw boost::system::system_error(ec);
                }

                std::string message(data, length);
                std::cout << "Received message from client " << client_ip << ": " << message << std::endl;
                
                // analyse the received message
                if(std::strncmp(data, "timeout", std::strlen("timeout")) != 0){ // if the message has the total shots
                    //database->db_INSERT_Coordinates;
                }

                //boost::asio::write(socket, boost::asio::buffer("Received message."));
                if(rflwindow->switchModeSignal || ptlwindow->switchModeSignal){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("backToDecideMode"));

                    if(isPistol){
                        emit ptlwindow->backToDecideModeSignal();
                    }
                    else if(isRifle){
                        emit rflwindow->backToDecideModeSignal();
                    }

                    decideMode = true;
                    rflwindow->switchModeSignal = false;
                    ptlwindow->switchModeSignal = false;
                }
                // else if(){

                // }
                else{
                    boost::asio::write(socket, boost::asio::buffer("proceed"));
                }
                
            }
        }

        socket.close();
        std::cout << "socket closed" << std::endl;
    }
    catch (std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void waitForOthers(){
    {
        std::unique_lock<std::mutex> lock(barrierMutex);
        ready = false;
        totalClients++;
        if (totalClients < connected_clients.size()) {
            barrierCV.wait(lock, [&]() { return ready; });
        } else {
            ready = true;
            barrierCV.notify_all();
            totalClients = 0;
        }
    }
}