#include "account.h"
vector<string> column;
string element;

Account::Account()
{

}
/**
 * @brief Account::Account Constructor that initializes table
 * @param _db Reference to DBTool
 * @param name Name of the database table
 * @param _ifTesting This is true if we are unit testing
 */
Account::Account(DBTool *_db, string name, bool _ifTesting) : DBTable(_db, name) {
    store_add_row_sql();
    store_create_sql();

    build_table();
    ifTesting = _ifTesting;
}

/**
 * @brief Account::login Logs the user in if the username and password match
 * @param _un Inputted username
 * @param _pw Inputted password
 * @return A reference to the viewer class that contains all user information
 */
Viewer* Account::login(string _un, string _pw) {
    if(ifTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    //Retrieves all usernames and looks for the id of the user
    select_elements_sql("username");
    int x = 0;
    for(int i = 0; i < (int) column.size(); i++) {
        select_cell(column.at(i), "username", "deleted");
        if(element != "1")
            allUsernames.push_back(column.at(i));
    }
    while(x < (int) column.size() && column.at(x) != _un) {
        x++;
    }
    x++;
    string tmp = to_string(x);
    //CHecks if the admin deleted this account or not
    select_cell(tmp, "id", "deleted");
    if(element == "1")
        return NULL;
    //Will return null if password and or username is incorrect
    select_cell(tmp, "id", "password");
    if(element != _pw)
        return NULL;

    //Gathers information from the database and adds it to the user class
    select_cell(tmp, "id", "privelege");
    Viewer* v;
    string level = element;
    if(level == "Viewer") {
        select_cell(tmp, "id", "description");
        string tmp1 = element;
        select_cell(tmp, "id", "picture");
        v = new Viewer(x, _un, _pw, level, tmp1, element);
    } else if(level == "Curator") {
        select_cell(tmp, "id", "description");
        string tmp1 = element;
        select_cell(tmp, "id", "picture");
        v = new Curator(x, _un, _pw, level, tmp1, element);
    } else if(level == "Admin") {
        select_cell(tmp, "id", "description");
        string tmp1 = element;
        select_cell(tmp, "id", "picture");
        string tmp2 = element;
        select_cell(tmp, "id", "uniquePW");
        v = new Admin(x, _un, _pw, level, tmp1, tmp2, element);
    } else {
        return NULL;
    }
    sqlite3_close(db);
    return v;
}

/**
 * @brief Account::addNewUser Adds new user to the database table if all formatting is correct and PIN matches
 * @param UN New username
 * @param PW New password
 * @param PIN The PIN that is supposed to match the Admin's PIN
 * @param DESC New description
 * @param PIC New picture file name
 * @return Reference to the new user
 */
Viewer* Account::addNewUser(string UN, string PW, string PIN, string DESC, string PIC) {
    if(UN == "" || PW == "")
        return NULL;
    if(ifTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
        //Need an admin account added to test this method
        add_row(1, "TMPADMIN", "2", "Admin", "", "", "V200", "0");
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    //Can't have identical usernames so returns if the proposed username already exists
    select_cell(UN, "username", "id");
    if(element != "")
        return NULL;
    //Looks for the admin's authentication PIN
    select_elements_sql("privelege");
    int x = 0;
    string tmp = to_string(x + 1);
    while(x < (int) column.size() && column.at(x) != "Admin") {
        x++;
    }
    count_rows();
    int numRows = stoi(element);
    select_cell(tmp, "id", "uniquePW");
    //IF the PINs don't match, return
    if(element != PIN)
        return NULL;
    //Creates a new user according to the format of the PIN
    string priv;
    Viewer* v;
    if(PIN[0] == 'C') {
        priv = "Curator";
        v = new Curator(numRows + 1, UN, PW, priv, DESC, PIC);
    } else if(PIN[0] == 'V') {
        priv = "Viewer";
        v = new Viewer(numRows + 1, UN, PW, priv, DESC, PIC);
    } else {
        return NULL;
    }
    //Adds data to the Accounts table
    add_row(numRows + 1, UN, PW, priv, DESC, PIC, "", "0");
    //Adds every valid user to vector
    select_elements_sql("username");
    for(int i = 0; i < (int) column.size(); i++) {
        select_cell(column.at(i), "username", "deleted");
        if(element != "1")
            allUsernames.push_back(column.at(i));
    }
    sqlite3_close(db);
    return v;
}

/**
 * @brief Account::removeUser Removes user by changing the boolean deleted field
 * @param name The username that should be deleted
 * @return The picture file name of the user so that the file can be deleted in another method
 */
string Account::removeUser(string name) {
    if(ifTesting) {
       //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    //Looks for the id of the username
    int x = 0;
    select_elements_sql("username");
    while(x < (int) column.size() && column.at(x) != name) {
        x++;
    }
    if(x >= (int) column.size())
        return "";
    x++;
    //Finds the picture file name according to the newly found ID
    select_cell(to_string(x), "id", "picture");
    string r = element;
    //Looks for the username in the allUsernames vector so that it can be deleted
    int y = 0;
    for(int i = 0; i < (int) allUsernames.size(); i++) {
        if(allUsernames.at(i) == name)
            break;
        y++;
    }
    allUsernames.erase(allUsernames.begin() + y);
    //Updates the deleted field in the database table
    update_value("deleted", "1", "id", to_string(x));
    sqlite3_close(db);
    return r;
}

/**
 * @brief Account::newPIN Changes the admin's authentication PIN in the table
 * @param p The new PIN
 * @return Whether the PIN change was successful or not
 */
bool Account::newPIN(string p) {
    if(ifTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    //PIN is saved in the admin's row so it looks for this cell
    select_cell("Admin", "privelege", "id");
    string adminID = element;

    //Checks formatting of the pin. The first letter must be C for the new created user to be a curator and if the
    //first letter of the PIN is a V the newly created user will be a viewer
    if(p == "" || (p[0] != 'C' && p[0] != 'V')) {
        sqlite3_close(db);
        return false;
    }
    //Update the new PIN in the database
    update_value("uniquePW", p, "id", adminID);
    sqlite3_close(db);
    return true;
}

/**
 * @brief Account::userData Returns all data about a given user
 * @param x The username
 * @return Vector of all of the data
 */
vector<string> Account::userData(string x) {
    if(ifTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cell(x, "username", "picture");
    vector<string> y;
    y.push_back(element);
    select_cell(x, "username", "description");
    y.push_back(element);
    select_cell(x, "username", "id");
    y.push_back(element);
    select_cell(x, "username", "privelege");
    y.push_back(element);
    sqlite3_close(db);
    return y;
}

/**
 * @brief Account::getNameFromId Returns the name of the user given the user's id
 * @param ID The user's ID
 * @return The username
 */
string Account::getNameFromId(int ID) {
    if(ifTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    if(ID == 0)
        return "";
    select_cell(to_string(ID), "id", "username");
    sqlite3_close(db);
    return element;
}


void Account::store_add_row_sql() {

    sql_template =  "SELECT name ";
    sql_template += "FROM   sqlite_master ";
    sql_template += "WHERE";
    sql_template += "    type = \"table\"";
    sql_template += ";";

}

/**
 * @brief Account::store_create_sql String that creates SQL query to create a new table
 */
void Account::store_create_sql() {

    //std::cerr << "calling store_create_sql from DBTableEx\n";

    sql_create =  "CREATE TABLE ";
    sql_create += table_name;
    sql_create += " ( ";
    sql_create += "  id INT PRIMARY KEY NOT NULL, ";
    sql_create += "  username TEXT NOT NULL,";
    sql_create += "  password TEXT NOT NULL, ";
    sql_create += "  privelege TEXT NOT NULL,";
    sql_create += "  description TEXT,";
    sql_create += "  uniquePW TEXT, ";
    sql_create += "  picture TEXT, ";
    sql_create += "  deleted BOOL ";
    sql_create += " );";

}

/**
 * @brief Account::add_row Adds a new row to the table
 * @param id Unique ID of the user
 * @param username The user's username
 * @param password The user's password
 * @param privelege The privelege level of the user
 * @param description The user's profile description
 * @param picture The user's profile picture file name
 * @param uniquePW The user's PIN, only the admin's cell will be filled
 * @param deleted Whether the admin deleted said account or not
 * @return Whether the addition was successful or not
 */
bool Account::add_row(int id, string username, string password, string privelege, string description, string picture, string uniquePW, string deleted) {
    int   retCode = 0;
    char *zErrMsg = 0;

    char  tempval[128];

    sql_add_row  = "INSERT INTO ";
    sql_add_row += table_name;
    sql_add_row += " ( id, username, password, privelege, description, uniquePW, picture, deleted) ";
    sql_add_row += "VALUES (";

    sprintf (tempval, "%d", id);
    sql_add_row += tempval;
    sql_add_row += ", ";

    sql_add_row += "\"";
    sql_add_row += std::string(username);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(password);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(privelege);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(description);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(uniquePW);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(picture);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(deleted);
    sql_add_row += "\"";

    sql_add_row += " );";

    //std::cout << sql_add_row << std::endl;

    retCode = sqlite3_exec(db,
                           sql_add_row.c_str(),
                           cb_add_row,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }

    return retCode;
}

/**
 * @brief Account::count_rows Counts how many rows are in the table
 */
void Account::count_rows() {
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "SELECT COUNT(*) FROM ";
    sql_select_all += table_name;
    sql_select_all += ";";

    element = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_count_rows,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
}

/**
 * @brief Account::update_value Updates the value of a given cell
 * @param colName The column name of the cell that needs to be changed
 * @param newValue New value of the cell
 * @param rowName The column name where the rowValue will be matched with
 * @param rowValue Decides which row to update
 */
void Account::update_value(string colName, string newValue, string rowName, string rowValue) {
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "UPDATE ";
    sql_select_all += table_name;
    sql_select_all += " SET " + colName;
    sql_select_all += " = '" + newValue;
    sql_select_all += "' WHERE " + rowName + " = '" + rowValue;
    sql_select_all += "';";

    element = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_update_value,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
}

/**
 * @brief Account::remove_row Removes a row given a column name and key value
 * @param n The key value of the row
 * @param cn The column name to match the key
 */
void Account::remove_entries() {
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "DELETE FROM ";
    sql_select_all += table_name;
    sql_select_all += ";";

    element = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_remove_row,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

/**
 * @brief Account::select_all Prints out all of the entries in the table
 * @return Whether the query was successful or not
 */
bool Account::select_all() {

    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "SELECT * FROM ";
    sql_select_all += table_name;
    sql_select_all += ";";

    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_select_all,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }

    return retCode;
}

/**
 * @brief Account::select_elements_sql Returns the values of an entire column
 * @param col Column name
 */
void Account::select_elements_sql(string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += table_name;
    sql_select += ";";

    column.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_a,
                           this,
                           &zErrMsg         );

    if(retCode != SQLITE_OK) {
        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
}

/**
 * @brief Account::select_cell Finds the requested value in a particular cell
 * @param whichRow Key to match so that the row is identified
 * @param row Column name to match whichRow with to identify the row of the desired cell
 * @param col Column name of the desired cell
 */
void Account::select_cell(string whichRow, string row, string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += table_name + " WHERE ";
    sql_select += row + " = '";
    sql_select += whichRow;
    sql_select += "';";

    element = "";
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_cell_a,
                           this,
                           &zErrMsg         );

    if(retCode != SQLITE_OK) {
        std::cerr << table_name
                  << " template ::"
                  << std::endl
                  << "SQL error: "
                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
}

int cb_add_row(void  *data,
               int    argc,
               char **argv,
               char **azColName)
{



    std::cerr << "cb_add_row being called\n";

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    int i;

    Account *obj = (Account *) data;

    std::cout << "------------------------------\n";
    std::cout << obj->get_name()
              << std::endl;

    for(i = 0; i < argc; i++){
        std::cout << azColName[i]
                  << " = "
                  <<  (argv[i] ? argv[i] : "NULL")
                  << std::endl;
    }

    return 0;
}

int cb_select_a(void *data,
              int argc,
              char **argv,
              char ** azColName)
{
    //std::cerr << "cb_select being called\n";

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    int i;
    for(i = 0; i < argc; i++) {
        column.push_back(argv[i] ? std::string(argv[i]) : "NULL");
    }
    return 0;
}

int cb_select_all(void  *data,
                  int    argc,
                  char **argv,
                  char **azColName)
{



    std::cerr << "cb_select_all being called\n";

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    int i;

    Account *obj = (Account *) data;

    std::cout << "------------------------------\n";
    std::cout << obj->get_name()
              << std::endl;

    for(i = 0; i < argc; i++){
        std::cout << azColName[i]
                  << " = "
                  <<  (argv[i] ? std::string(argv[i]) : "NULL")
                  << std::endl;
    }

    return 0;
}

int cb_select_cell_a(void *data,
                    int argc,
                    char **argv,
                    char ** azColName)
{

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    element = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}

int cb_count_rows(void* data,
                  int argc,
                  char **argv,
                  char **azColName)
{
    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    element = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}

int cb_remove_row(void* data,
                  int argc,
                  char **argv,
                  char **azColName)
{
    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    element = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}

int cb_update_value(void* data,
                  int argc,
                  char **argv,
                  char **azColName)
{
    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    element = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}

/**
 * @brief Account::getAllUsernames Getter method for allUsernames
 * @return Vector of all usernames in the system
 */
vector<string> Account::getAllUsernames() {return allUsernames;}

/**
 * @brief Account::setAllUsernames Setter method for allUsernames
 * @param x New vector that allUsernames will be set to
 */
void Account::setAllUsernames(vector<string> x) {allUsernames = x;}
