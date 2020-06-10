#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <fstream>
#include <string>
#include "sqlite3.h"
#include <vector>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dbtool.h"
#include "dbtable.h"
#include "viewer.h"
#include "curator.h"
#include "admin.h"
using namespace std;

class Viewer;
class Account : public DBTable {
public:
    //Constructors
    Account();
    Account(DBTool *db, string name, bool _ifTesting);
    //User management methods
    Viewer* login(string _un, string _pw);
    Viewer* addNewUser(string UN, string PW, string PIN, string DESC, string PIC);
    string removeUser(string);
    //Saves a new PIN to the admin's row
    bool newPIN(string);
    //Returns the row of data of a particular row
    vector<string> userData(string);
    string getNameFromId(int);

    //SQL execution methods
    void store_create_sql();
    virtual void store_add_row_sql();
    bool add_row(int id, string username, string password, string privelege, string description, string picture, string uniquePW, string deleted);
    bool select_all();
    void select_elements_sql(string);
    void select_cell(string, string, string);
    void count_rows();
    //TestAccounts table TEARDOWN method
    void remove_entries();
    void update_value(string, string, string, string);

    //Getter and setter methods
    vector<string> getAllUsernames();
    void setAllUsernames(vector<string>);

protected:
    string sql_select_all;
    string sql_select;

private:
    vector<string> allUsernames;
    sqlite3* db;
    bool ifTesting;
};

//Callback methods

// This is a callback function that is sent to the library and used
// to parse the sql request being sent to the database.
int cb_add_row(void  *data,
               int    argc,
               char **argv,
               char **azColName);


// This is a callback function that is sent to the library and used
// to parse the sql request being sent to the database.
int cb_select_all(void  *data,
                  int    argc,
                  char **argv,
                  char **azColName);

int cb_select_a(void  *data,
              int    argc,
              char **argv,
              char **azColName);

int cb_select_cell_a(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

int cb_count_rows(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

int cb_remove_row(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

int cb_update_value(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

#endif // ACCOUNT_H
