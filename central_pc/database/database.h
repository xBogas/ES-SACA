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

/*class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    bool login(const QString &username, const QString &password);
    QStringList getAthletes();
    bool insertScore(const QString &athlete, const QString &competition, int score);

private:
    QSqlDatabase m_db;
};*/

class Database {
private:
    std::unique_ptr<pqxx::connection> conn;
    string create_competitionid(string location, string date, string category);
public:
    Database();
    ~Database();
    void execute(const string& query);
    void db_INSERT_Athlete(int licenseid, string name, string gender, string nationality, int age, string club);
    void db_INSERT_Competition(string name, string location, string date, string category);

    
};





#endif // DATABASE_H
