#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits>
#include <stdio.h>

#include <pqxx/pqxx>
#include <ctime>
#include <cctype>

#include <algorithm>

using namespace pqxx;
using namespace std;

class Database {
private:
    unique_ptr<connection> conn;
    vector<vector<string>> execute(const string& query, bool is_select);
    string create_seriesid(int licenseid, string competitionid);
    string create_coordenatesid(string seriesid, int i);  
    bool verify_competitionid(string id); 
    bool verify_seriesid(string id);
    

public:
    Database();
    ~Database();

    bool db_INSERT_Athlete(int licenseid, string nome, string clube, string disciplina, string escalao, string dataNascimento, string pais, string observacoes);
    bool db_INSERT_Competition(string name, string location, string date, string category, string competitionid);
    bool db_INSERT_Series(int participantrow, int licenseid, string competitionid);
    bool db_INSERT_Coordinates(int licenseid, string competitionid, float coordinatex, float coordinatey, float score, int i);
    bool db_INSERT_Rank(int place, int licenseid, string competitionid);
    bool db_UPDATE_Series(float finalscore, int licenseid, string competitionid);

    bool update_score(int licenseid, string competitionid, float coordinatex, float coordinatey, float individual_score, float finalscore, int i);
    bool verify_id(int ID);
    string get_name_from_id(int ID);
    bool db_IMPORT(string file_loc, string user);
    bool update_Routine();


    string create_competitionid(string location, string date, string category);
};


#endif // DATABASE_H
