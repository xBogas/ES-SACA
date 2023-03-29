#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <pqxx/pqxx>
int main()
{   // DB de psw
    std::cout << "inicio\n";
    const std::string DbHostIP="10.227.240.130";
    const std::string DbName="pswa0309";
    const std::string DbUser="pswa0309";
    const std::string DbPassword="SouUmGato7";
    const std::string DbPort="5432";
    /*const std::string DbHostIP="127.0.0.1";
    const std::string DbName="postgres";
    const std::string DbUser="postgres";
    const std::string DbPassword="admin";
    const std::string DbPort="5432";*/
    //colocar variáveis de login 

    /************************/
    /*Para correr usar comando "g++ demo.cpp -L/usr/lib/x86_64-linux-gnu/ -lpqxx -lpq -o demo.o" no terminal*/
    /*Necesário instalar libpqxx e postgresql no pc*/
    /************************/
    

    try{
        pqxx::connection conn("dbname="+DbName+" user="+DbUser+" password="+DbPassword+" hostaddr="+DbHostIP+" port="+DbPort+"");
        if(conn.is_open()){
            std::cout<<"Sucessful Connection" << std::endl;
        }
        else{
            std::cout <<"Failed Connection"<<std::endl;
            return 1;
        }
        conn.disconnect();
        return 0;
    }
    catch(const std::exception &e){
        std::cerr<<e.what()<<std::endl;
        return 1;
    }

}
