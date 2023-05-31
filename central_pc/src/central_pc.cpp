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

int aux = 0;

std::atomic<bool> finish(false);

Database* database;

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);

    //create database
    database = new Database();
    
    //create windows
    InitWindow i = InitWindow(nullptr, database);  
    i.show();
    MainWindow* w = i.getMainWindow();
    MainWindow2* w2 = w->getMainWindow2();
    RifleWindow* rfl = w2->getRifleWindow();
    PistolWindow* ptl = w2->getPistolWindow();

    //start server thread
    std::thread server(server_thread, w, w2, ptl, rfl);

    //conectar sinal para fechar threads e sockets
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        io_context.stop();
        finish = true;
        //server.join();
    });

    return a.exec();
}

void server_thread(MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow){
    // std::vector<boost::asio::ip::tcp::socket*> open_sockets; // vetor para armazenar os sockets abertos

    try{
        tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("10.0.2.15"), 8080));
        std::cout << "Server started. Listening on port 8080..." << std::endl;

        for(;;){
            if(finish){

                break;
            }

            tcp::socket socket(io_context);

            boost::system::error_code ec;
            acceptor.accept(socket, ec);

            // verificar se houve erro ou se a condição de término foi atingida
            if (ec == boost::asio::error::operation_aborted || finish) {
                break; // sair do loop
            }
            else if (ec) {
                // tratar o erro, caso ocorra
                std::cerr << "Erro ao aceitar conexão: " << ec.message() << std::endl;
                continue;
            }

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
            std::thread client_thread(handle_client, std::move(socket), window, window2, ptlwindow, rflwindow);
            client_thread.detach();
        }
    }
    catch (std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handle_client(tcp::socket&& socket, MainWindow* window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow){
    try{
        boost::system::error_code error;
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
                if(window2->backSignal){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("backToInitial"));
                    
                    window->isMainWindow = true;
                    window2->backSignal = false;
                    decideType = false;
                    initial = true;
                }

                if(window2->pistol){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("pistol"));

                    decideMode = true;
                    decideType = false;
                } 
                else if(window2->rifle){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("rifle"));

                    isRifle = true;
                    isPistol = false;
                    decideMode = true;
                    decideType = false;
                } 
            }
            else if(decideMode){
                if(window2->pistol){
                    newPractice = ptlwindow->practiceSignal;
                    newMatch = ptlwindow->matchSignal;
                    newFinal = ptlwindow->finalSignal;
                    //std::cout << "practice: " << ptlwindow->practiceSignal << " | match: " << ptlwindow->matchSignal << " | final: " << ptlwindow->finalSignal << std::endl;

                    if(ptlwindow->backSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideType"));

                        ptlwindow->backSignal = false;
                        decideMode = false;
                        decideType = true;
                    }

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        canStart = true;
                    }

                    if(canStart && ptlwindow->startSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("start"));

                        ptlwindow->startSignal = false; 
                        ptlwindow->canBack = false;

                        if(newMatch || newFinal)
                            decideMode = false;
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
                        ptlwindow->canBack = true;
                    }
                }
                else if(window2->rifle){
                    newPractice = rflwindow->practiceSignal;
                    newMatch = rflwindow->matchSignal;
                    newFinal = rflwindow->finalSignal;

                    if(rflwindow->backSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideType"));

                        rflwindow->backSignal = false;
                        decideMode = false;
                        decideType = true;
                    }

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        canStart = true;
                    }

                    if(canStart && rflwindow->startSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("start"));
                            
                        canStart = false;    
                        rflwindow->startSignal = false;
                        rflwindow->canBack = false;

                        if(newMatch || newFinal)
                            decideMode = false;
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
                        rflwindow->canBack = true;
                    }
                }

                // if(canStart){
                //     if(window2->pistol){
                //         if(ptlwindow->startSignal){
                //             boost::asio::write(socket, boost::asio::buffer("start"));

                //             if(ptlwindow->matchSignal)
                //                 decideMode = false;
                //         }
                //     }
                //     else if(window2->rifle){
                //         if(rflwindow->startSignal){
                //             boost::asio::write(socket, boost::asio::buffer("start"));
                            
                //             if(rflwindow->matchSignal)
                //                 decideMode = false;
                //         }
                //     }
                // }
            }
            else{
                char data[1024];
                size_t length = socket.read_some(boost::asio::buffer(data), error);

                if (error == boost::asio::error::eof){
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
                else if (error){
                    throw boost::system::system_error(error);
                }

                std::string message(data, length);
                std::cout << "Received message from client " << socket.remote_endpoint().address().to_string() << ": " << message << std::endl;

                //boost::asio::write(socket, boost::asio::buffer("Received message."));
                if(rflwindow->switchModeSignal || ptlwindow->switchModeSignal){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("backToDecideMode"));

                    if(isPistol){
                        emit ptlwindow->backToDecideModeSignal();
                        ptlwindow->canBack = true;
                        ptlwindow->switchModeSignal = false;
                    }
                    else if(isRifle){
                        emit rflwindow->backToDecideModeSignal();
                        rflwindow->canBack = true;
                        rflwindow->switchModeSignal = false;
                    }

                    decideMode = true;
                }
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