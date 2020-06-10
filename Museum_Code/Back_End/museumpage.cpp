#include "museumpage.h"
vector<string> colu;
string elem;
/**
 * @brief MuseumPage::MuseumPage Default Constructor
 */
MuseumPage::MuseumPage()
{
    name = "";
    id = 0;
    picString = "";
    description = "";
}

/**
 * @brief MuseumPage::MuseumPage Constructor that initializes the ID of museum
 * @param i The desired ID
 */
MuseumPage::MuseumPage(int i) {
    id = i;
    name = "";
    picString = "";
    description = "";
}

/**
 * @brief MuseumPage::MuseumPage Constructor that initializes database class tools
 * @param _db Reference to DBTool class
 * @param name Name of the Database table
 */
MuseumPage::MuseumPage(DBTool *_db,std::string name, bool _isTesting):
    DBTable(_db,name){
    isTesting = _isTesting;
    dbtool = _db;
    store_add_row_sql();
    store_create_sql();

    build_table();
    name = "";
    picString = "";
    description = "";
}

/**
 * @brief MuseumPage::getMuseumPageFromId Searches for museum by ID and returns all data related to it
 * @param _id Museum ID
 * @return Reference to Museum ID object that contains all needed data
 */
MuseumPage* MuseumPage::getMuseumPageFromId(int _id){
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_elements_sql("museumId");
    int index = 0;
    while(index < (int) colu.size() && colu.at(index) != to_string(_id))
        index++;
    if(index >= (int) colu.size())
        return NULL;
    index++;
    MuseumPage* newPage = new MuseumPage(dbtool, "Comments", isTesting);
    //Gets the data from the database using the unique museum ID
    string tmp = to_string(index);
    select_cell(tmp,"museumId","name");
    string newName = colu.at(0);
    select_cell(tmp,"museumId","imageFile");
    string newPicString = colu.at(0);
    select_cell(tmp,"museumId","parentId");
    int newParentId = stoi(colu.at(0));
    select_cell(tmp,"museumId","description");
    string newDesc = colu.at(0);
    select_cell(tmp,"museumId","curatorId");
    int newCuratorId = stoi(colu.at(0));
    //Initialization of MuseumPage data
    newPage->setId(_id);
    newPage->setName(newName);
    newPage->setCuratorId(newCuratorId);
    newPage->setParentId(newParentId);
    newPage->setDescription(newDesc);
    newPage->setPicString(newPicString);
    sqlite3_close(db);
    return newPage;
}

/**
 * @brief MuseumPage::addMuseum Checks formatting of new museum entry and adds to database if everything is correct
 * @param _pID The Parent Museum's ID
 * @param _n The name of the new museum
 * @param _d The description of the new museum
 * @param _p The picture file name of the new museum
 * @param _c The ID of the curator that created this new museum
 * @param priv The privelege of the user
 * @return Whether the addition of the museum was successful
 */
bool MuseumPage::addMuseum(int _pID, string _n, string _d, string _p, int _c, string priv) {
    //Checks for correct formatting and privelege levels
    if(priv != "Admin" && priv != "Curator")
        return false;
    if(_n == "" || _p == "")
        return false;
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    //Museums cannot have the same name as each other
    select_cell(_n, "name", "museumId");
    if(colu.size() != 0)
        return false;
    add_row(_pID, _n, _d, _p, _c);
    sqlite3_close(db);
    return true;
}

/**
 * @brief MuseumPage::getMuseumChildrenId Retrieves all of the museum IDs of a given parent's children museums
 * @param parent The ID of the parent museum
 * @return A vector full of id's of children museum
 */
vector<string> MuseumPage::getMuseumChildrenId(int parent) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cell(to_string(parent), "parentId", "*");
    vector<string> x;
    //THe SQL query loads the entire row, but only need the museum's ID which is on every sixth index
    for(int i = 0; i < (int) colu.size(); i += 6)
        x.push_back(colu.at(i));
    sqlite3_close(db);
    return x;
}

/**
 * @brief MuseumPage::getMuseumChildrenNames Retrieves all museum names of a give parent's children museums
 * @param parent The ID of the parent museum
 * @return A vector full of names of children museum
 */
vector<string> MuseumPage::getMuseumChildrenNames(int parent) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cell(to_string(parent), "parentId", "*");
    vector<string> x;
    for(int i = 2; i < (int) colu.size(); i += 6) {
        x.push_back(colu.at(i));
    }
    sqlite3_close(db);
    return x;
}

/**
 * @brief MuseumPage::getMuseumByCuratorId Retrieves all of the names of the museum's given a curator's ID
 * @param CID Curator's unique ID
 * @return A vector full of museum names created by the curator
 */
vector<string> MuseumPage::getMuseumByCuratorId(int CID) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cell(to_string(CID), "curatorId", "*");
    vector<string> x;
    for(int i = 2; i < (int) colu.size(); i += 6) {
        x.push_back(colu.at(i));
    }
    sqlite3_close(db);
    return x;
}

/**
 * @brief MuseumPage::getMuseumId Finds the museum's ID given the museum's name
 * @param mname The museum's name
 * @return The museum's unique ID
 */
int MuseumPage::getMuseumId(string mname) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    select_cell(mname, "name", "museumId");
    if(colu.size() == 0)
        return 0;
    int x = stoi(colu.at(0));

    sqlite3_close(db);
    return x;
}

/**
 * @brief MuseumPage::getMuseumSearchId Finds the museum's ID given the museum's name and specifically uses the like
 * keyword to find similar searches to the inputted query
 * @param mname The museum's name
 * @return The museum's unique ID
 */
int MuseumPage::getMuseumSearchId(string mname) {
    if(isTesting) {
        //Use this if we are testing
        sqlite3_open("../../museum/project/museum.sqlite", &db);
    } else {
        //Use this if we are using for GUI
        sqlite3_open("../../../../../museum/project/museum.sqlite", &db);
    }
    string pattern = "%";
    int idx = 0;
    pattern += mname[idx];
    idx++;
    pattern += '%';
    select_cell_search(pattern, "name", "museumId");
    while(colu.size() > 1) {
        pattern.pop_back();
        pattern += mname[idx];
        pattern += '%';
        idx++;
        select_cell_search(pattern, "name", "museumId");
    }
    if(colu.size() == 0)
        return 0;
    int x = stoi(colu.at(0));

    sqlite3_close(db);
    return x;
}

void MuseumPage::store_add_row_sql() {
    sql_template =  "SELECT name ";
    sql_template += "FROM   sqlite_master ";
    sql_template += "WHERE";
    sql_template += "    type = \"table\"";
    sql_template += ";";
}

void MuseumPage::store_create_sql() {

    //std::cerr << "calling store_create_sql from DBTableEx\n";

    //UPDATE sqlite_sequence SET seq = (SELECT MAX(col) FROM Tbl) WHERE name="Tbl" resets autoincrement

    sql_create =  "CREATE TABLE ";
    sql_create += table_name;
    sql_create += " ( ";
    sql_create += "  museumId INTEGER PRIMARY KEY AUTOINCREMENT, ";
    sql_create += "  parentId INTEGER, ";
    sql_create += "  name TEXT, ";
    sql_create += "  description TEXT, ";
    sql_create += "  imageFile TEXT, ";
    sql_create += "  curatorId INTEGER";
    sql_create += " );";

}

bool MuseumPage::add_row(int pID, string n, string d, string p, int c) {
    int   retCode = 0;
    char *zErrMsg = 0;

    char  tempval[128];

    sql_add_row  = "INSERT INTO ";
    if(isTesting) {
        sql_add_row += "TestMuseumPages";
    } else {
        sql_add_row += "MuseumPages";
    }
    sql_add_row += " (parentId, name, description, imageFile, curatorId) ";
    sql_add_row += "VALUES (";

    sprintf (tempval, "%d", pID);
    sql_add_row += tempval;
    sql_add_row += ", ";

    sql_add_row += "\"";
    sql_add_row += std::string(n);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(d);
    sql_add_row += "\", ";

    sql_add_row += "\"";
    sql_add_row += std::string(p);
    sql_add_row += "\", ";

    sprintf (tempval, "%d", c);
    sql_add_row += tempval;

    sql_add_row += " );";

//    std::cout << sql_add_row << std::endl;

    retCode = sqlite3_exec(db,
                           sql_add_row.c_str(),
                           museum_add_row,
                           this,
                           &zErrMsg          );

    if( retCode != SQLITE_OK ){

//        std::cerr << table_name
//                  << " template ::"
//                  << std::endl
//                  << "SQL error: "
//                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }

    return retCode;
}


vector<string> MuseumPage::select_elements_sql(string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += "MuseumPages";
    sql_select += ";";

    colu.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select,
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
    return colu;
}


vector<string> MuseumPage::select_cell(string whichRow, string row, string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += "MuseumPages WHERE ";
    sql_select += row + " = '";
    sql_select += whichRow;
    sql_select += "';";

    colu.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_cell_m,
                           this,
                           &zErrMsg         );

    if(retCode != SQLITE_OK) {
//        std::cerr << table_name
//                  << " template ::"
//                  << std::endl
//                  << "SQL error: "
//                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
    return colu;
}

/**
 * @brief MuseumPage::select_cell_search Looks for an element in the table using the LIKE keyword
 * @param whichRow The pattern in which the LIKE keyword will be compared to
 * @param row The row that is desired
 * @param col The column that is desired
 * @return The result of the SQL query
 */
vector<string> MuseumPage::select_cell_search(string whichRow, string row, string col) {
    int retCode = 0;
    char *zErrMsg = 0;

    sql_select = "SELECT ";
    sql_select += col + " FROM ";
    sql_select += "MuseumPages WHERE ";
    sql_select += row + " LIKE '";
    sql_select += whichRow;
    sql_select += "';";

    colu.clear();
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select_cell_m,
                           this,
                           &zErrMsg         );

    if(retCode != SQLITE_OK) {
//        std::cerr << table_name
//                  << " template ::"
//                  << std::endl
//                  << "SQL error: "
//                  << zErrMsg;

        sqlite3_free(zErrMsg);
    }
    return colu;
}

/**
 * @brief MuseumPage::remove_row Removes a row given a column name and key value
 * @param n The key value of the row
 * @param cn The column name to match the key
 */
void MuseumPage::remove_entries() {
    int   retCode = 0;
    char *zErrMsg = 0;

    sql_select  = "DELETE FROM ";
    sql_select += "TestMuseumPages";
    sql_select += ";";

    elem = "";
    retCode = sqlite3_exec(db,
                           sql_select.c_str(),
                           cb_select,
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


int museum_add_row(void  *data,
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

    MuseumPage *obj = (MuseumPage *) data;

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

int cb_select(void *data,
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
        colu.push_back(argv[i] ? std::string(argv[i]) : "NULL");
    }
    return 0;
}

int cb_select_cell_m(void *data,
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
        colu.push_back(argv[i] ? std::string(argv[i]) : "NULL");
    }

    return 0;
}

//Getter and setter methods
int MuseumPage::getId() {return id;}

void MuseumPage::setId(int x) {id = x;}

string MuseumPage::getDescription() {return description;}

void MuseumPage::setDescription(string x) {description = x;}

string MuseumPage::getName() {return name;}

void MuseumPage::setName(string x) {name = x;}

string MuseumPage::getPicString() {return picString;}

void MuseumPage::setPicString(string x) {picString = x;}

int MuseumPage::getParentId() {return parentID;}

void MuseumPage::setParentId(int x) {parentID = x;}

int MuseumPage::getCuratorId() {return curatorId;}

void MuseumPage::setCuratorId(int x) {curatorId = x;}

void MuseumPage::setDB(sqlite3* x) {db = x;}
