#include "database.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("shooting_db");
    m_db.setUserName("postgres");
    m_db.setPassword("mypassword");
    m_db.setPort(5432);
}

bool Database::login(const QString &username, const QString &password)
{
    if (!m_db.open()) {
        qDebug() << "Failed to open database";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM athletes WHERE username=:username AND password=:password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 1) {
            return true;
        }
    }

    return false;
}

QStringList Database::getAthletes()
{
    QStringList athletes;

    if (!m_db.open()) {
        qDebug() << "Failed to open database";
        return athletes;
    }

    QSqlQuery query;
    query.exec("SELECT name FROM athletes");

    while (query.next()) {
        athletes.append(query.value(0).toString());
    }

    return athletes;
}

bool Database::insertScore(const QString &athlete, const QString &competition, int score)
{
    if (!m_db.open()) {
        qDebug() << "Failed to open database";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO scores (athlete, competition, score) VALUES (:athlete, :competition, :score)");
    query.bindValue(":athlete", athlete);
    query.bindValue(":competition", competition);
    query.bindValue(":score", score);
    return query.exec();
}