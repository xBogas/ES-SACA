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

bool Database::update_score(int licenseid, string competitionid, int coordinatesid, float coordinatex, float coordinatey, float score){
    string seriesid = create_seriesid(licenseid, competitionid);

    if(db_INSERT_Coordinates(coordinatesid, coordinatex, coordinatey, score, seriesid)) return true;
    else return false;
}

bool Database::verify_id(int ID){

    try{
        string sql = "SELECT * FROM \"Athlete\" WHERE \"Licença\" = " + to_string(ID) + ";";
        vector<vector<string>> rows = execute(sql, true);

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
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

        return !rows.empty();

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return false;
}

string Database::get_name_from_id(int ID){

    try{
        string sql = "SELECT \"Nome\" FROM \"Athlete\" WHERE \"Licença\" = " + to_string(ID) + ";";

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

bool Database::db_INSERT_Competition(string name, string location, string date, string category){

    string competitionid = create_competitionid(location, date, category);

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

bool Database::db_INSERT_Series(int participantrow, float finalscore, int licenseid, string competitionid){

    string seriesid = create_seriesid(licenseid, competitionid);

    if(verify_seriesid(seriesid)) return false;
    if(!verify_id(licenseid)) return false;
    if(!verify_competitionid(competitionid)) return false;

    try{
        string sql = "INSERT INTO \"Series\" VALUES ('"
             + seriesid + "', " + to_string(participantrow) + ", " + to_string(finalscore) + ", " + to_string(licenseid) + ", '" + competitionid + "');";

        execute(sql, false);

        cout << "Updated Series successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}

//ver problemas em ter que mandar seriesid, que melhor forma é para otimizar isto
//ver problemas com coordinatesid
bool Database::db_INSERT_Coordinates(int coordinatesid, float coordinatex, float coordinatey, float finalscore, string seriesid){

    if(!verify_seriesid(seriesid)) return false;
    
    try{
        string sql = "INSERT INTO \"Coordinates\" VALUES (" 
                    + to_string(coordinatesid) + ", " + to_string(coordinatex) + ", " + to_string(coordinatey) + ", " + to_string(finalscore)  + ", '" + seriesid + "');";

        execute(sql, false);

        cout << "Updated Coordinates successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}

bool Database::db_Import(string file_loc, string table){


    try{
        

        string sql = "copy \""+ to_string(table)+"\" FROM '"+ to_string(file_loc)+"' WITH (FORMAT csv, HEADER, DELIMITER ';',ENCODING 'ISO-8859-1');";
        execute(sql, false);
        cout << "Imported successfully" << endl;


    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }

    return true;
}



bool Database::db_INSERT_Rank(int place, int licenseid, string competitionid){

    string seriesid = create_seriesid(licenseid, competitionid);
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

bool Database::update_Routine(){

    try{
        string sql1="UPDATE \"Athlete\" SET \"Licença\" = temp.\"Licença\", \"Nome\" = temp.\"Nome\", \"Clube\" = temp.\"Clube\", \"Disciplina\" = temp.\"Disciplina\", \"Escalão\" = temp.\"Escalão\", \"Data de Nascimento\" = temp.\"Data de Nascimento\", \"País\" = temp.\"País\", \"Observações\" = temp.\"Observações\" FROM temp WHERE \"Athlete\".\"Licença\" = temp.\"Licença\";";

        string sql2="INSERT INTO \"Athlete\" (\"Licença\", \"Nome\", \"Clube\", \"Disciplina\", \"Escalão\", \"Data de Nascimento\", \"País\", \"Observações\")SELECT * FROM temp WHERE NOT EXISTS (SELECT 1 FROM \"Athlete\" WHERE \"Athlete\".\"Licença\" = temp.\"Licença\");";

        string sql=sql1+sql2;

        execute(sql, false);

        cout << "Updated Temp and Athlete successfully" << endl;
        return true;

    }catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
    }
}   

//1234_PORTO_12/02/22_P
string Database::create_seriesid(int licenseid, string competitionid){
    string id = to_string(licenseid) + "_" + competitionid;

    //cout << "series id: " << id << endl;
    
    return id;
}

//PORTO_12/02/22_P
string Database::create_competitionid(string location, string date, string category){
    string id;

    transform(location.begin(), location.end(), location.begin(), ::toupper);

    size_t slash1 = date.find('/');
    size_t slash2 = date.find('/', slash1+1);
    size_t slash3 = date.find('/', slash2+1);

    id += location + "_" + date.substr(0, slash1) + "/" + date.substr(slash1+1, slash2-slash1-1) + "/" + date.substr(slash2+3, 2) + "_" + category;
    //cout << "competition id: " << id << endl;
    
    return id;
}
