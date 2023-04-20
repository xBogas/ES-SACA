#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits>
#include <stdio.h>
#include <string>
#include <pqxx/pqxx>
#include <ctime>

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
    connection* conn;
public:
    Database();
    ~Database();
    void execute(const std::string& query);
    int db_INSERT_Athlete(int licenseid, string name, string gender, string nationality, int age, string club);
};





#endif // DATABASE_H
