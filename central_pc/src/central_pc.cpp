#include "../gui/mainwindow.h"
#include "../gui/mainwindow2.h"
#include "../gui/pistolwindow.h"
#include "../gui/riflewindow.h"
#include <QtWidgets/QApplication>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <mutex>

int aux = 0;

using boost::asio::ip::tcp;
boost::asio::io_context io_context;

std::vector<std::string> connected_clients;
std::mutex connected_clients_mutex;
std::string type;

void handle_client(tcp::socket&& socket, MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow);
void server_thread(MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow);

bool initial = true;
bool decideType = false;
bool decideMode = false;

int main(int argc, char *argv[]){
    //gui code
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MainWindow2* w2 = w.getMainWindow2();
    RifleWindow* rfl = w2->getRifleWindow();
    PistolWindow* ptl = w2->getPistolWindow();
    
    //start server thread
    std::thread server(server_thread, &w, w2, ptl, rfl);

    return a.exec();
}

void server_thread(MainWindow *window, MainWindow2 *window2, PistolWindow *ptlwindow, RifleWindow *rflwindow){
    try{
        tcp::acceptor acceptor(io_context, tcp::endpoint(boost::asio::ip::address_v4::from_string("10.0.2.15"), 8080));

        std::cout << "Server started. Listening on port 8080..." << std::endl;

        for (;;){
            tcp::socket socket(io_context);
            acceptor.accept(socket);

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

        

        for (;;){
            if(initial){
                if(window->isMainWindow){
                    if(window->cellWasChanged){
                        int new_clientID = window->clientPlayerIds[client_ip];

                        if(new_clientID != old_clientID){
                            std::string clientID = "clientID: " + std::to_string(new_clientID);
                            boost::asio::write(socket, boost::asio::buffer(clientID));

                            old_clientID = new_clientID;
                        }

                        window->cellWasChanged = false;
                    }
                }
                else{
                    boost::asio::write(socket, boost::asio::buffer("continue"));

                    decideType = true;
                    initial = false;
                }
                
            }
            else if(decideType){
                std::cout << "Aqui no decideType" << std::endl;
                if(window2->pistol){
                    boost::asio::write(socket, boost::asio::buffer("pistol"));

                    decideMode = true;
                    decideType = false;
                } 
                else if(window2->rifle){
                    boost::asio::write(socket, boost::asio::buffer("rifle"));
                    
                    decideMode = true;
                    decideType = false;
                } 
            }
            else if(decideMode){
                if(ptlwindow->practice){
                    boost::asio::write(socket, boost::asio::buffer("practice"));
                    
                    decideMode = false;
                }
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

                boost::asio::write(socket, boost::asio::buffer("Received message."));
            }
        }
    }
    catch (std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

