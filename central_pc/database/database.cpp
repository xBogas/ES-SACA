#include "database.h"

Database::Database() {
    const string DbHostIP="127.0.0.1";
    const string DbName="es_saca";
    const string DbUser="postgres";
    const string DbPassword="equipaE_saca";
    const string DbPort="5432";

    static connection conn("host="+DbHostIP+" dbname="+DbName+" user="+DbUser+" password="+DbPassword+" port="+DbPort);

    if (conn.is_open()) {
        std::cout << "Successful Connection to " << conn.dbname() << std::endl;
    } else {
        std::cout << "Failed Connection" << std::endl;
        throw std::runtime_error("Can't open database");
    }
}


Database::~Database() {
    conn->disconnect();
    delete conn;
}

/*int db_INSERT_Athlete(int licenseid, string name, string gender, string nationality, int age, string club){

    std::__cxx11::basic_string<char> sql;

    try{
        work W(*conn);

        sql = "INSERT INTO \"Athlete\" (licenseid, name, gender, nationality, age, club) VALUES (" + to_string(licenseid) + ", '" + name + "', '" + gender + "', '" + nationality + "', " + to_string(age) +", '" + club + "')";

        W.exec(sql.c_str());
        W.commit();
        cout << "Updated successfully" << endl;


    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
    }

    return 0;
}*/











/*bool Database::login(const QString &username, const QString &password)
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
}*/