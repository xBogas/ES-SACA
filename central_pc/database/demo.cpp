// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdlib.h>
// #include <limits>
// #include <stdio.h>
// #include <string>
// #include <pqxx/pqxx>
// #include <ctime>

// #include "../database/database.h"


// using namespace pqxx;
// using namespace std;

// connection& db_connection();
// int db_INSERT_Athlete(connection& conn, int licenseid, string name, string gender, string nationality, int age, string club);
// int db_INSERT_Competition(connection& conn, string competitionid, string name, string location, string date);
// int db_INSERT_Series(connection& conn, string seriesid, int participantrow, string category, float finalscore, int licenseid, string competitionid, bool ispractice);
// int db_INSERT_Coordinates(connection& conn, float coordinatex, float coordinatey, float score, string seriesid);
// int db_INSERT_Rank(connection& conn, int rank, int licenseid, string competitionid);

// int db_SELECT();
// bool verify_ID(int ID);
// string get_name_from_id(int ID);
// string create_seriesid(int licenseid, string competitionid);
// bool update_score(int licenseid, string competitionid, float score);
// //int db_Import(connection& conn ,string file_loc, string table);

// int main()
// {   
//     //putenv("PQXX_DEBUG=1");
//     /************************/
//     /*Para correr usar comando "g++ demo.cpp -L/usr/lib/x86_64-linux-gnu/ -lpqxx -lpq -o demo.o no terminal*/
//     /*Necesário instalar libpqxx e postgresql no pc*/
//     /*Pensar se é melhor adicionar um paramentro com a conexão a cada função em vez de fazer essa conexão cada vez que se pretenda usar*/
//     /************************/

//     /*Para correr usar comando "g++ demo.cpp database.cpp -L/usr/lib/x86_64-linux-gnu/ -lpqxx -lpq -o demo.o" no terminal*/
    
//     //db_INSERT_Competition("POR0804", "Torneio Natal", "Porto", "8/4/2023");
//     //db_INSERT_Series("255POR0804", 4, "P", 602.1, 255, "POR0804", false);
//     //db_INSERT_Coordinates(1.5, 1.2, 9.3, "255POR0804");
//     //db_INSERT_Rank(3, 255, "POR0804");
//     //db_INSERT_Remark("Q", "255POR0804");

//     //db_SELECT();

//     /*connection& conn = db_connection();

//     db_INSERT_Competition(conn, "BRG1904", "Torneio Páscoa", "Braga", "19/4/2023");
//     db_INSERT_Series(conn, "115BRG1904", 3, "P", 632.1, 115, "BRG1904", false);
//     db_INSERT_Coordinates(conn, 3.2, 3.4, 10, "115BRG1904");
//     db_INSERT_Rank(conn, 1, 115, "BRG1904");

//     conn.disconnect();*/

//     //connection& conn=db_connection();
//     //db_Import(conn,"/tmp/db_nonull.csv","Athlete");
//     //conn.disconnect();

//     Database my_db;
//     my_db.db_EXPORT_Athletes("ines", "/home/ines");
//     //my_db.db_IMPORT("/home/ines", "ines");
 
//     //my_db.db_EXPORT_CompetitionResults(3000002, "BRAGA_12.02.2021_P", "ines", "/home/ines");
//     // my_db.db_INSERT_Competition("Natal", "Braga", "12.02.2021", "P");
//     // my_db.db_INSERT_Athlete(3000002, "Joana", "FGB", "", "", "12.2.2010", "PORTUGAL", "");
//     // my_db.db_INSERT_Athlete(3000001, "Joana", "FGB", "", "", "12.2.2010", "PORTUGAL", "");
//     // my_db.db_INSERT_Athlete(3000000, "Joana", "FGB", "", "", "12.2.2010", "PORTUGAL", "");

//     // my_db.db_INSERT_Series(5, 3000002, "BRAGA_12.02.2021_P", false);
//     // my_db.db_INSERT_Series(3, 3000001, "BRAGA_12.02.2021_P", false);
//     //     my_db.db_INSERT_Series(3, 3000001, "BRAGA_12.02.2021_P", true);
//     // my_db.db_INSERT_Series(1, 3000000, "BRAGA_12.02.2021_P", false);
//     //     my_db.db_INSERT_Series(1, 3000002, "BRAGA_12.02.2021_P", true);

//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 9, 60, 1, false);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 60, 2, false);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 60, 3, false);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 90, 4, false);

//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 9, 60, 1, false);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 70, 2, false);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 80, 3, false);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 90, 4, false);

//     // my_db.update_score(3000000, "BRAGA_12.02.2021_P", 12.5, 2.3, 9, 60, 1, false);
//     // my_db.update_score(3000000, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 70, 2, false);
//     // my_db.update_score(3000000, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 80, 3, false);
//     // my_db.update_score(3000000, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 60, 4, false);

//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 9, 60, 1, true);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 60, 2, true);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 60, 3, true);
//     // my_db.update_score(3000002, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 80, 4, true);

//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 9, 60, 1, true);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 70, 2, true);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 80, 3, true);
//     // my_db.update_score(3000001, "BRAGA_12.02.2021_P", 12.5, 2.3, 10, 90, 4, true);

    

//     //my_db.db_INSERT_Competition("Torneio Páscoa", "Braga", "22/4/2022", "P");
//     //my_db.db_INSERT_Competition("Torneio Mensal", "Coimbra", "1/2/2023", "P");
//     //my_db.db_INSERT_Series(4, 602.1, 2423, "BRAGA_22/4/22_P");
//     //my_db.update_score(2423, "BRAGA_22/4/22_P", 1, 1.4, 1.3, 9.3);
//     //my_db.db_INSERT_Rank(3, 2423, "BRAGA_22/4/22_P");
//     //my_db.get_name_from_id(2647);



// }

// // connection& db_connection() {
// //     const string DbHostIP="127.0.0.1";
// //     const string DbName="saca";
// //     const string DbUser="postgres";
// //     const string DbPassword="saca";
// //     const string DbPort="5432";

// //     static connection conn("host="+DbHostIP+" dbname="+DbName+" user="+DbUser+" password="+DbPassword+" port="+DbPort);

// //     if (conn.is_open()) {
// //         std::cout << "Successful Connection to " << conn.dbname() << std::endl;
// //     } else {
// //         std::cout << "Failed Connection" << std::endl;
// //         throw std::runtime_error("Can't open database");
// //     }

// //     return conn;
// // }


// // //enviar os parametros assim não é seguro, diz para parameterizar?

// // int db_INSERT_Athlete(connection& conn, int licenseid, string name, string gender, string nationality, int age, string club){

// //     std::__cxx11::basic_string<char> sql;

// //     try{
        

// //         work W(conn);

// //         sql = "INSERT INTO \"Athlete\" (licenseid, name, gender, nationality, age, club) VALUES (" + to_string(licenseid) + ", '" + name + "', '" + gender + "', '" + nationality + "', " + to_string(age) +", '" + club + "')";

// //         W.exec(sql.c_str());
// //         W.commit();
// //         cout << "Updated successfully" << endl;


// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }


// // int db_INSERT_Competition(connection& conn, string competitionid, string name, string location, string date){

// //     std::__cxx11::basic_string<char> sql;

// //     //string competitionid = location.substr(0, 3) + 

// //     try{
        

// //         work W(conn);

// //         sql = "INSERT INTO \"Competition\" (competitionid, name, location, date) VALUES ('" + competitionid + "', '" + name + "', '" + location + "', '" + date + "')";
// //         //sql = "INSERT INTO \"Competition\" (competitionid, name, location, date) VALUES ($1, $2, $3, $4)";
             
// //         //cout << "sql" + sql << endl;
// //         //pqxx::prepare::invocation invoc = W.prepared(sql)(competitionid)(name)(location)(to_string(date));
// //         //invoc.exec();


// //         //pqxx::prepare::invocation invoc = W.prepared(sql)(competitionid)(name)(location)(date);
// //         //invoc.exec();


// //         W.exec(sql.c_str());
        
// //         //pqxx::prepare::invocation stmt = W.prepared(sql);
// //         //stmt(competitionid)(name)(location)(to_string(date)).exec();


// //         //pqxx::prepare::invocation invoc = txn.prepared("myquery", "INSERT INTO Competition (competitionid, name, location, date) VALUES ($1, $2, $3, $4)");

// //         //pqxx::result res = invoc(competitionid, name, location, date).exec();

// //         W.commit();
// //         cout << "Updated successfully" << endl;
        

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }


// // int db_INSERT_Coordinates(connection& conn, float coordinatex, float coordinatey, float score, string seriesid){

// //     std::__cxx11::basic_string<char> sql;

// //     try{

// //         work W(conn);

// //         sql = "INSERT INTO \"Coordinates\" (coordinatex, coordinatey, score, seriesid) VALUES (" + to_string(coordinatex) + ", " + to_string(coordinatey) + ", " + to_string(score)  + ", '" + seriesid + "')";

// //         W.exec(sql.c_str());
// //         W.commit();
// //         cout << "Updated successfully" << endl;
 

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }


// // int db_INSERT_Series(connection& conn, string seriesid, int participantrow, string category, float finalscore, int licenseid, string competitionid, bool ispractice){

// //     std::__cxx11::basic_string<char> sql;

// //     try{

// //         work W(conn);

// //         //sql = "INSERT INTO \"Series\" (seriesid, participantrow, category, finalscore, licenseid, competitionid, ispractice) VALUES (" + seriesid + ", " + to_string(participantrow) + ", " + category + ", " + to_string(finalscore)  + ", " + to_string(licenseid) +  ", '" + competitionid +  "', " + to_string(ispractice) + ")";
// //         string sql = "INSERT INTO \"Series\" (seriesid, participantrow, category, finalscore, licenseid, competitionid, ispractice) VALUES ('"
// //              + seriesid + "', " + to_string(participantrow) + ", '" + category + "', " + std::to_string(finalscore) + ", " + to_string(licenseid) + ", '" + competitionid + "', " + to_string(ispractice) + ")";

// //         W.exec(sql.c_str());
// //         W.commit();
// //         cout << "Updated successfully" << endl;
 

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }


// // int db_INSERT_Rank(connection& conn, int place, int licenseid, string competitionid){

// //     std::__cxx11::basic_string<char> sql;

// //     try{
    

// //         work W(conn);

// //         sql = "INSERT INTO \"Rank\" (place, licenseid, competitionid) VALUES (" + to_string(place) + ", " + to_string(licenseid) + ", '" + competitionid + "')";

// //         W.exec(sql.c_str());
// //         W.commit();
// //         cout << "Updated successfully" << endl;
 


// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }




// // int db_SELECT(connection conn){

// //     std::__cxx11::basic_string<char> sql;

// //     std::vector<std::vector<std::string>> matrix;

// //     try{

// //         work W(conn);

// //         sql = "SELECT * FROM \"Athlete\"";

    
// //         pqxx::result r = W.exec(sql.c_str());
// //         W.commit();

// //         for (const auto& row : r) {
// //             std::vector<std::string> values;
// //             for (const auto& field : row) {
// //                 values.push_back(field.c_str());

// //             }
// //             matrix.push_back(values);
// //         }
    
// //         cout << matrix[1][2] << endl;
 

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }

// // int db_Import(connection& conn ,string file_loc, string table){

// //     std::__cxx11::basic_string<char> sql;

// //     try{
        

// //         work W(conn);
// //         sql = "copy \""+ to_string(table)+"\" FROM '"+ to_string(file_loc)+"' WITH (FORMAT csv, HEADER, DELIMITER ';',ENCODING 'ISO-8859-1');";
// //         W.exec(sql.c_str());
// //         W.commit();
// //         cout << "Updated successfully" << endl;


// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// //     return 0;
// // }

// // //talvez repensar maneira de criar seriesid
// // //dar update ao nome da função 
// // //pode haver conflito se atleta fizer duas competicoes num dia
// // string create_seriesid(int licenseid, string competitionid){
// //     return to_string(licenseid) + competitionid;
// // }


// // bool update_score(int licenseid, string competitionid, float score){
// //     string seriesid = create_seriesid(licenseid, competitionid);
// //     int flag ;//= db_INSERT_Coordinates(NULL, NULL, score, seriesid);

// //     return !flag;
// // }

// // //tirar disconnect no final de cada funcao db

// // bool verify_ID(int ID){

// //     std::__cxx11::basic_string<char> sql;

// //     try{
// //         connection& conn = db_connection();

// //         work W(conn);

// //         sql = "SELECT * FROM \"Athlete\" WHERE licenseid =" + to_string(ID) + ";";

// //         pqxx::result r = W.exec(sql.c_str());  
// //         bool exists=!r.empty();
// //         //cout << exists << endl;
// //         W.commit();
// //         conn.disconnect();
// //         return exists;

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return 1;
// //     }

// // }

// // string get_name_from_id(int ID){
// //      std::__cxx11::basic_string<char> sql;

// //     try{
// //         connection& conn = db_connection();

// //         work W(conn);

// //         sql = "SELECT name FROM \"Athlete\" WHERE licenseid=" + to_string(ID) + ";";

// //       pqxx::result r = W.exec(sql.c_str());  
// //       std::string name= r[0]["Name"].as<std::string>();
// //       W.commit();
// //         conn.disconnect();
// //     return name;

// //     }catch (const std::exception &e) {
// //       cerr << e.what() << std::endl;
// //       return NULL;
// //     }

// // }


// // //codigo para criar competition id tendo em conta localizaçao e data + seriesid também

// // //ver melhor valores que podem ser NULL (por exemplo, nome da competiçao pode ser null?)

// // void time_now(){
// //     time_t ttime = time(0);
// //     tm *local_time = localtime(&ttime);

// //     cout << local_time->tm_mday << "/";
// //     cout << 1 + local_time->tm_mon<< "/";
// //     cout << 1900 + local_time->tm_year << endl;
// // }

