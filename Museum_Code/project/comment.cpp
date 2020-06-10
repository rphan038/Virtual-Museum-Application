#include "comment.h"
string e;
vector<string> d;
Comment::Comment(DBTool* db,string name, bool _isTesting):
    DBTable(db,name)
{
    isTesting = _isTesting;
    store_add_row_sql();
    store_create_sql();

    build_table();
}

void Comment::store_add_row_sql() {
    sql_template =  "SELECT name ";
    sql_template += "FROM   sqlite_master ";
    sql_template += "WHERE";
    sql_template += "    type = \"table\"";
    sql_template += ";";
}


void Comment::store_create_sql() {

    //std::cerr << "calling store_create_sql from DBTableEx\n";

    sql_create =  "CREATE TABLE ";
    sql_create += table_name;
    sql_create += " ( ";
    sql_create += "  commentId INTEGER PRIMARY KEY AUTOINCREMENT, ";
    sql_create += "  museumId INTEGER, ";
    sql_create += "  author TEXT, ";
    sql_create += "  content TEXT, ";
    sql_create += "  deleted BOOL";
    sql_create += " );";
}

void Comment::newComment(int mID, string a, string c) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    add_row(mID, a, c, "0");
    sqlite3_close(db);
}

vector<string> Comment::getMuseumComments(int mID) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cells(to_string(mID), "museumId", "*");
    sqlite3_close(db);
    return d;
}

bool Comment::removeComment(string content) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cells(content, "content", "*");
    //In the case that there are identical comments, looks for the one that has not been deleted yet
    int i;
    for(i = 0; i < (int) d.size(); i += 5) {
        if(d.at(i + 4) == "0")
            break;
    }
    if(d.size() == 0 || i >= (int) d.size())
        return false;
    int ID = stoi(d.at(i));
    update_value("deleted", "1", "commentId", to_string(ID));
    sqlite3_close(db);
    return true;
}

bool Comment::add_row(int museumId, string author, string content, string deleted) {
    int   retCode = 0;
    char *zErrMsg = 0;

    char  tempval[128];

    sql_add_row  = "INSERT INTO ";
    sql_add_row += table_name;
    sql_add_row += " (museumId, author, content, deleted) ";
    sql_add_row += "VALUES (";

    sprintf (tempval, "%d", museumId);
    sql_add_row += tempval;
    sql_add_row += ", ";

    sql_add_row += "\"";
    sql_add_row += std::string(author);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(content);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(deleted);
    sql_add_row += "\"";

    sql_add_row += " );";

//    std::cout << sql_add_row << std::endl;

    retCode = sqlite3_exec(db,
                           sql_add_row.c_str(),
                           comment_add_row,
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

vector<string> Comment::select_elements_sql(string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += table_name;
    sql_select += ";";

    d.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_c,
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
    return d;
}

vector<string> Comment::select_cells(string whichRow, string row, string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += table_name + " WHERE ";
    sql_select += row + " = '";
    sql_select += whichRow;
    sql_select += "';";

    d.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_cell_c,
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
    return d;
}

void Comment::update_value(string colName, string newValue, string rowName, string rowValue) {
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "UPDATE ";
    sql_select_all += table_name;
    sql_select_all += " SET " + colName;
    sql_select_all += " = '" + newValue;
    sql_select_all += "' WHERE " + rowName + " = '" + rowValue;
    sql_select_all += "';";

    e = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_update_value_c,
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
 * @brief Comment::remove_row Removes a row given a column name and key value
 * @param n The key value of the row
 * @param cn The column name to match the key
 */
void Comment::remove_entries() {
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "DELETE FROM ";
    sql_select_all += table_name;
    sql_select_all += ";";

    e = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_select_c,
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

void Comment::setDB(sqlite3* x) {db = x;}


int comment_add_row(void  *data,
                   int    argc,
                   char **argv,
                  char **azColName)
{



//    std::cerr << "game_add_row being called\n";

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;

    }

    int i;

    Comment *obj = (Comment *) data;

    std::cout << "------------------------------\n";
//    std::cout << obj->get_name()
//              << std::endl;

    for(i = 0; i < argc; i++){
        std::cout << azColName[i]
                  << " = "
                  <<  (argv[i] ? argv[i] : "NULL")
                  << "\t";
    }
    cout<<endl;

    return 0;
}

int Comment::count_rows() {
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select_all  = "SELECT COUNT(*) FROM ";
    sql_select_all += table_name;
    sql_select_all += ";";

    e = "";
    retCode = sqlite3_exec(db,
                           sql_select_all.c_str(),
                           cb_count_rows_c,
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
    return stoi(e);
}

int cb_select_cell_c(void *data,
                    int argc,
                    char **argv,
                    char ** azColName)
{

    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }
    int i;
    for(i = 0; i < argc; i++) {
        d.push_back(argv[i] ? std::string(argv[i]) : "NULL");
    }

    return 0;
}

int cb_select_c(void *data,
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
        d.push_back(argv[i] ? std::string(argv[i]) : "NULL");
    }
    return 0;
}

int cb_count_rows_c(void* data,
                  int argc,
                  char **argv,
                  char **azColName)
{
    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    e = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}

int cb_update_value_c(void* data,
                  int argc,
                  char **argv,
                  char **azColName)
{
    if(argc < 1) {
        std::cerr << "No data presented to callback "
                  << "argc = " << argc
                  << std::endl;
    }

    e = argv[0] ? std::string(argv[0]) : "NULL";

    return 0;
}


