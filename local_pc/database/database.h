#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    bool login(const QString &username, const QString &password);
    QStringList getAthletes();
    bool insertScore(const QString &athlete, const QString &competition, int score);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
