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
    string create_competitionid(string location, string date, string category);
    string create_seriesid(int licenseid, string competitionid);   
public:
    Database();
    ~Database();

    void db_INSERT_Athlete(int licenseid, string name, string gender, string nationality, int age, string club);
    void db_INSERT_Competition(string name, string location, string date, string category);
    void db_INSERT_Series(int participantrow, float finalscore, int licenseid, string competitionid);
    void db_INSERT_Coordinates(int coordinatesid, float coordinatex, float coordinatey, float finalscore, string seriesid);
    void db_INSERT_Rank(int place, int licenseid, string competitionid);

    void update_score(int licenseid, string competitionid, int coordinatesid, float coordinatex, float coordinatey, float score);
    bool verify_id(int ID);
    string get_name_from_id(int ID);

    //update table series para atualizar final score

};


#endif // DATABASE_H
