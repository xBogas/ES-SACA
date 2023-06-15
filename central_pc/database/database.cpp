#include "database.h"

Database::Database(){
    const string DbHostIP="127.0.0.1";
    const string DbName="saca";
    const string DbUser="postgres";
    const string DbPassword="saca";
    const string DbPort="5432";

    conn = std::make_unique<pqxx::connection>("host="+DbHostIP+" dbname="+DbName+" user="+DbUser+" password="+DbPassword+" port="+DbPort);

    if (conn->is_open()) {
        std::cout << "Successful Connection to " << conn->dbname() << std::endl;
    } else {
        std::cout << "Failed Connection" << std::endl;
        throw std::runtime_error("Can't open database");
    }
}

Database::~Database() {
    conn->disconnect();
}


bool Database::update_score(int licenseid, string competitionid, int coordinatex, int coordinatey, float individual_score, float finalscore, int i, bool isFinal){
    std::cout << "Updating score" << std::endl;
    string seriesid = create_seriesid(licenseid, competitionid, isFinal);

    db_INSERT_Coordinates(licenseid, competitionid, coordinatex, coordinatey, individual_score, i, isFinal);

    db_UPDATE_Series(finalscore, licenseid, competitionid, isFinal);
    return true;

}

bool Database::update_rank(string competitionid, bool isFinal){
    int i = 0;

    try{
        string sql = "SELECT \"Athlete\".\"Licenca\", \"Series\".finalscore FROM \"Athlete\" JOIN \"Series\" ON \"Series\".licenseid = \"Athlete\".\"Licenca\" WHERE \"Series\".competitionid = '" 
                    + competitionid + "' ORDER BY \"Series\".finalscore DESC;";

        vector<vector<string>> rows = execute(sql, true);

        i = 0;

        for (const auto& row : rows) {
            int license = stoi(row[0]);
            //float score = stof(row[1]);
            i++;
            db_INSERT_Rank(i, license, competitionid, isFinal);
        }
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    //ordenar atletas por rank com outros parametros

    // SELECT "Rank".place AS "Rank", "Athlete"."Nome" AS "Nome", "Athlete"."Licenca" AS "Licenca", "Series".finalscore AS "Pontuação"   
    // FROM "Athlete"
    // JOIN "Rank" ON "Rank".licenseid = "Athlete"."Licenca"
    // JOIN "Series" ON "Series".licenseid = "Athlete"."Licenca"
    // ORDER BY "Rank".place;
}

bool Database::verify_id(int ID){

    try{
        string sql = "SELECT * FROM \"Athlete\" WHERE \"Licenca\" = " + to_string(ID) + ";";
        vector<vector<string>> rows = execute(sql, true);

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
}

string Database::get_name_from_id(int ID){

    try{
        string sql = "SELECT \"Nome\" FROM \"Athlete\" WHERE \"Licenca\" = " + to_string(ID) + ";";

        vector<vector<string>> rows = execute(sql, true);

        if (rows.empty()) return "";
        else {
            return rows[0][0];
        }

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return "";
    }

}

bool Database::db_IMPORT(string file_loc, string user){

    try{
        //give permission
        ifstream file(file_loc + "/Atletas.csv");
        

        if(!file) return false;

        string command1 = "sudo chown postgres /home/" + user +"\n";
        command1 += "sudo chgrp postgres /home/" + user;
        system(command1.c_str());

        //create command
        string sql = "COPY temp FROM '" + file_loc + "/Atletas.csv' WITH (FORMAT csv, HEADER, DELIMITER ',',ENCODING 'ISO-8859-1'); ";
        sql += "UPDATE \"Athlete\" SET \"Licenca\" = temp.\"Licenca\", \"Nome\" = temp.\"Nome\", \"Clube\" = temp.\"Clube\", \"Disciplina\" = temp.\"Disciplina\", \"Escalao\" = temp.\"Escalao\", \"Data de Nascimento\" = temp.\"Data de Nascimento\", \"Pais\" = temp.\"Pais\", \"Observacoes\" = temp.\"Observacoes\" FROM temp WHERE \"Athlete\".\"Licenca\" = temp.\"Licenca\"; ";
        sql += "INSERT INTO \"Athlete\" (\"Licenca\", \"Nome\", \"Clube\", \"Disciplina\", \"Escalao\", \"Data de Nascimento\", \"Pais\", \"Observacoes\") SELECT * FROM temp WHERE NOT EXISTS (SELECT 1 FROM \"Athlete\" WHERE \"Athlete\".\"Licenca\" = temp.\"Licenca\");";
        
        execute(sql, false);

        string sql2 = "DELETE FROM temp";
        execute(sql2, false);

        //retrieve permission
        string command2 = "sudo chown " + user + " /home/" + user + "\n";
        command2 += "sudo chgrp " + user + " /home/" + user;
        system(command2.c_str());

        cout << "Imported successfully" << endl;

    }catch (const std::exception &e) {
        string command3 = "sudo chown " + user + " /home/" + user + "\n";
        command3 += "sudo chgrp " + user + " /home/" + user;
        system(command3.c_str());
        cerr << e.what() << std::endl;
        return false;
    }


    return true;
}

bool Database::db_EXPORT_CompetitionResults(int licenseid, string competitionid, string user, string file_loc){

    string seriesid_f = create_seriesid(licenseid, competitionid, true);
    string seriesid_q = create_seriesid(licenseid, competitionid, false);
    string name = get_name_from_id(licenseid);
    try{

        //give permission
        string command1 = "sudo chown postgres /home/" + user +"\n";
        command1 += "sudo chgrp postgres /home/" + user;
        system(command1.c_str());

        string sql = "COPY (SELECT CASE WHEN \"Series\".isFinal = true THEN 'Final' WHEN \"Series\".isFinal = false THEN 'Qualificacao' END AS \"Tipo\", \"Coordinates\".coordinatex AS \"X\", \"Coordinates\".coordinatey AS \"Y\", \"Coordinates\".score AS \"Pontuacao do Tiro\", \"Series\".finalscore AS \"Pontuacao Final\" FROM \"Coordinates\" JOIN \"Series\" ON \"Coordinates\".seriesid = \"Series\".seriesid WHERE \"Coordinates\".seriesid IN (SELECT \"Series\".seriesid FROM \"Series\" WHERE isFinal = false) AND \"Coordinates\".seriesid = '" 
        + seriesid_q + "' UNION ALL SELECT NULL, NULL, NULL, NULL, NULL UNION ALL SELECT CASE WHEN \"Series\".isFinal = true THEN 'Final' WHEN \"Series\".isFinal = false THEN 'Qualificação' END AS \"Tipo\", \"Coordinates\".coordinatex AS \"X\", \"Coordinates\".coordinatey AS \"Y\", \"Coordinates\".score AS \"Pontuação\", \"Series\".finalscore AS \"Pontuação Final\" FROM \"Coordinates\" JOIN \"Series\" ON \"Coordinates\".seriesid = \"Series\".seriesid WHERE \"Coordinates\".seriesid IN (SELECT \"Series\".seriesid FROM \"Series\" WHERE isFinal = true) AND \"Coordinates\".seriesid = '" 
        + seriesid_f + "') TO '" + file_loc + "/" + to_string(licenseid) + "_" + name + "_" 
                    + competitionid + ".csv' WITH (FORMAT csv, HEADER, DELIMITER ',',ENCODING 'ISO-8859-1');";
        execute(sql, false);

        //retrieve permission
        string command2 = "sudo chown " + user + " /home/" + user + "\n";
        command2 += "sudo chgrp " + user + " /home/" + user;
        system(command2.c_str());

        return true;

    }catch (const std::exception &e) {
        string command3 = "sudo chown " + user + " /home/" + user + "\n";
        command3 += "sudo chgrp " + user + " /home/" + user;
        system(command3.c_str());
        cerr << e.what() << std::endl;
        return false;
    }
}

bool Database::db_EXPORT_Athletes(string user, string file_loc){

    try{

        //give permission
        string command1 = "sudo chown postgres /home/" + user +"\n";
        command1 += "sudo chgrp postgres /home/" + user + "\n";

        ifstream file(file_loc + "/Atletas.csv");
        if(file) {
            command1 += "sudo chown postgres /home/" + user + "/Atletas.csv \n";
            command1 += "sudo chgrp postgres /home/" + user + "/Atletas.csv"; 
        }

        system(command1.c_str());
        
        string sql = "COPY (SELECT * FROM \"Athlete\" ORDER BY \"Athlete\".\"Licenca\") TO '" + file_loc + "/Atletas.csv' WITH (FORMAT csv, HEADER, DELIMITER ',',ENCODING 'ISO-8859-1');";
        execute(sql, false);

        //retrieve permission
        string command2 = "sudo chown " + user + " /home/" + user + "\n";
        command2 += "sudo chgrp " + user + " /home/" + user + "\n";
        command2 += "sudo chown " + user + " /home/" + user + "/Atletas.csv\n";
        command2 += "sudo chgrp " + user + " /home/" + user + "/Atletas.csv";
        system(command2.c_str());

        return true;

    }catch (const std::exception &e) {
        string command3 = "sudo chown " + user + " /home/" + user + "\n";
        command3 += "sudo chgrp " + user + " /home/" + user;
        system(command3.c_str());
        cerr << e.what() << std::endl;
        return false;
    }
}



bool Database::db_INSERT_Athlete(int licenseid, string nome, string clube, string disciplina, string escalao, string dataNascimento, string pais, string observacoes){

    if(verify_id(licenseid)) return false;

    try{
        string sql = "INSERT INTO \"Athlete\" VALUES (" 
                        + to_string(licenseid) + ", '" + nome + "', '" + clube + "', '" + disciplina + "', '" + escalao + "', '" 
                        + dataNascimento + "', '" + pais + "', '" + observacoes + "');";

        execute(sql, false);

        cout << "Updated Athlete successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}

bool Database::db_INSERT_Competition(string competitionid, string name, string location, string date, string category){

    if(verify_competitionid(competitionid)) return false;

    try{
        string sql = "INSERT INTO \"Competition\" VALUES ('" 
                        + competitionid + "', '" + name + "', '" + location + "', '" + date + "', '" + category +"');";
       
        execute(sql, false);

        cout << "Updated Competition successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

}

bool Database::db_INSERT_Series(int participantrow, int licenseid, string competitionid, bool isFinal){

    string seriesid = create_seriesid(licenseid, competitionid, isFinal);

    if(verify_seriesid(seriesid)) return false;
    if(!verify_id(licenseid)) return false;
    if(!verify_competitionid(competitionid)) return false;

    try{
        string sql = "INSERT INTO \"Series\" (seriesid, participantrow, licenseid, competitionid, isFinal) VALUES ('"
             + seriesid + "', " + to_string(participantrow) + ", " + to_string(licenseid) + ", '" + competitionid + "', '" + to_string(isFinal) + "');";

        execute(sql, false);

        cout << "Updated Series successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}

bool Database::db_INSERT_Coordinates(int licenseid, string competitionid, int coordinatex, int coordinatey, float score, int i, bool isFinal){

    string seriesid = create_seriesid(licenseid, competitionid, isFinal);

    string coordinatesid = create_coordenatesid(seriesid, i);

    if(!verify_seriesid(seriesid)) return false;
    
    try{
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << score;
        std::string scoreString = oss.str();

        string sql = "INSERT INTO \"Coordinates\" VALUES ('" 
                    + coordinatesid + "', " + to_string(coordinatex) + ", " + to_string(coordinatey) + ", " + scoreString  + ", '" + seriesid + "');";
 
        execute(sql, false);

        cout << "Updated Coordinates successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}

bool Database::db_INSERT_Rank(int place, int licenseid, string competitionid, bool isFinal){

    string seriesid = create_seriesid(licenseid, competitionid, isFinal);
    if(!verify_seriesid(seriesid)) return false;

    try{
        string sql = "INSERT INTO \"Rank\" VALUES ('" + seriesid + "', " + to_string(place) + ", " + to_string(licenseid) + ", '" + competitionid + "')";

        execute(sql, false);

        cout << "Updated Rank successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
} 

bool Database::db_UPDATE_Series(float finalscore, int licenseid, string competitionid, bool isFinal){

    string seriesid = create_seriesid(licenseid, competitionid, isFinal);

    if(!verify_seriesid(seriesid)) return false;

    try{

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << finalscore;
        std::string finalscoreString = oss.str();

        string sql = "UPDATE \"Series\" SET finalscore = " + finalscoreString + " WHERE seriesid = '" + seriesid + "';";

        execute(sql, false);

        cout << "Updated Series Row successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}


//1234_PORTO_12/02/22_pistola_Final
string Database::create_seriesid(int licenseid, string competitionid, bool isFinal){
    string id;

    if(isFinal) id = to_string(licenseid) + "_" + competitionid + "_F";
    else id = to_string(licenseid) + "_" + competitionid + "_Q";

    //cout << "series id: " << id << endl;
    
    return id;
}

//PORTO_12/02/22_pistola
string Database::create_competitionid(string location, string date, string category){
    string id;

    transform(location.begin(), location.end(), location.begin(), ::toupper);

    id += location + "_" + date + "_" + category;
    //cout << "competition id: " << id << endl;
    
    return id;
}

//1234_PORTO_12/02/22_pistola_1
string Database::create_coordenatesid(string seriesid, int i){
    string id = seriesid + "_" + to_string(i);

    //cout << "series id: " << id << endl;
    
    return id;
}

bool Database::verify_competitionid(string id){

    try{
        string sql = "SELECT * FROM \"Competition\" WHERE competitionid = '" + id + "';";
        vector<vector<string>> rows = execute(sql, true);

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
}

bool Database::verify_seriesid(string id){

    try{
        string sql = "SELECT * FROM \"Series\" WHERE seriesid = '" + id + "';";
        
        vector<vector<string>> rows = execute(sql, true);

        cout << sql << endl;

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
}

bool Database::verify_coordinatesid(string id){

    try{
        string sql = "SELECT * FROM \"Coordianates\" WHERE coordinatesid = '" + id + "';";
        vector<vector<string>> rows = execute(sql, true);

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
}

vector<vector<string>> Database::execute(const string& query, bool is_select) {

    try {
        work W(*conn);
        result r = W.exec(query);
        W.commit();
        vector<vector<string>> rows;
        if(is_select){
            
            for (const auto& row : r) {
                vector<std::string> values;
                for (const auto& field : row) {
                    values.push_back(field.c_str());

                }
                rows.push_back(values);
            }
        }
        return rows;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

}