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
void extractValues(std::string message);

bool initial = true, decideType = false, decideMode = false, canStart = false;
bool wasClicked = false;
bool newPractice = false, newMatch = false, newFinal = false;
bool oldPractice = false, oldFinal = false, oldMatch = false;
bool oldStart = false;
bool isPistol = false, isRifle = false;

// mode of the competition
bool practice = false, match = false, Final = false;
int totalShotsMatch = 0. , totalShotsFinal = 0;

// competition data
std::string nome, local, dataComp, categoria, competitionid;

// shot values
int coordinateX, coordinateY;
double score;

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
        int new_clientID = 0;

        
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
                        new_clientID = window->clientPlayerIds[client_ip];

                        if(database->verify_id(new_clientID) && !window->samePlayerIds[client_ip]){
                            std::string name = database->get_name_from_id(new_clientID);
                            std::string athlete = name + ";" + std::to_string(new_clientID);
                            boost::asio::write(socket, boost::asio::buffer(athlete));

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
                    
                    dataComp = window->data;
                    local = window->local;
                    nome = window->nome;

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

                    categoria = "pistola";
                    competitionid = database->create_competitionid(local, dataComp, categoria);
                    std::cout << "competitionid: " << competitionid << std::endl;

                    isPistol = true;
                    isRifle = false;
                    decideMode = true;
                    decideType = false;
                    window2->pistol = false;
                } 
                else if(window2->rifle){
                    waitForOthers();

                    boost::asio::write(socket, boost::asio::buffer("rifle"));

                    categoria = "carabina";
                    competitionid = database->create_competitionid(local, dataComp, categoria);
                    std::cout << "competitionid: " << competitionid << std::endl;

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

                    if(ptlwindow->backSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideType"));

                        ptlwindow->backSignal = false;
                        decideMode = false;
                        decideType = true;
                        practice = false;
                        match = false;
                        Final = false;
                    }

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        practice = true;
                        match = false;
                        Final = false;
                        ptlwindow->practiceSignal = false;
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        match = true;
                        Final = false;
                        practice = false;
                        ptlwindow->matchSignal = false;
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        Final = true;
                        practice = false;
                        match = false;
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
                        ptlwindow->canBack = false;

                        if(practice || match || Final){
                            decideMode = false;
                            canStart = false;
                            oldPractice = false;
                            oldFinal = false;
                            oldMatch = false;
                        }
                    }
                }
                else if(isRifle){
                    newPractice = rflwindow->practiceSignal;
                    newMatch = rflwindow->matchSignal;
                    newFinal = rflwindow->finalSignal;

                    if(rflwindow->backSignal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("backToDecideType"));

                        rflwindow->backSignal = false;
                        decideMode = false;
                        decideType = true;
                        practice = false;
                        match = false;
                        Final = false;
                    }

                    if(newPractice && !oldPractice){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("practice"));
                        
                        practice = true;
                        match = false;
                        Final = false;
                        rflwindow->practiceSignal = false;
                        canStart = true;
                    }
                    else if(newMatch && !oldMatch){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("match"));
                        
                        match = true;
                        Final = false;
                        practice = false;
                        rflwindow->matchSignal = false;
                        canStart = true;
                    }
                    else if(newFinal && !oldFinal){
                        waitForOthers();

                        boost::asio::write(socket, boost::asio::buffer("final"));
                        
                        Final = true;
                        practice = false;
                        match = false;
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
                        rflwindow->canBack = false;

                        if(practice || match || Final){
                            decideMode = false;
                            canStart = false;
                            oldPractice = false;
                            oldFinal = false;
                            oldMatch = false;
                        }
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
                if(!(std::strncmp(data, "timeout", std::strlen("timeout")) == 0) && !(std::strncmp(data, "shotInTrain", std::strlen("shotInTrain")) == 0)){ // if the message has the total shots
                    extractValues(data);

                    //update score in database
                    database->update_score(new_clientID, competitionid, coordinateX, coordinateY, score);

                    //detect if it is the last shot
                    if(match){
                        totalShotsMatch++;
                        if(totalShotsMatch == 60){
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
                            match = false;
                            totalShotsMatch = 0;
                        }
                    }
                    else if(Final){
                        totalShotsFinal++;
                        if(totalShotsFinal == 24){
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
                            Final = false;
                            totalShotsFinal = 0;
                        }
                    }
                }

                // if back button was clicked
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

void extractValues(std::string message){
    // first read if it is shot or not
    std::string delimiter_first = "->";
    size_t pos_first = 0;
    std::string token_first;

    pos_first = message.find(delimiter_first);
    token_first = message.substr(0, pos_first);
    message.erase(0, pos_first + delimiter_first.length());

    // extract values
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    // Extract coordinateX
    pos = message.find(delimiter);
    token = message.substr(0, pos);
    coordinateX = std::stoi(token);
    message.erase(0, pos + delimiter.length());

    // Extract coordinateY
    pos = message.find(delimiter);
    token = message.substr(0, pos);
    coordinateY = std::stoi(token);
    message.erase(0, pos + delimiter.length());

    // Extract score
    pos = message.find(delimiter);
    token = message.substr(0, pos);
    score = std::stod(token);
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