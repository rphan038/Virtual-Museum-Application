#ifndef MUSEUMPAGE_H
#define MUSEUMPAGE_H
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
#include "curator.h"
using namespace std;

class MuseumPage: public DBTable {
public:

    /**
     * @brief store_create_sql sql statement for creates on in table
     */
    void store_create_sql();

    /**
     * @brief store_add_row_sql sql statement for row add on ining to table
     */
    virtual void store_add_row_sql();

    /**
     * @brief add_row adds row to database
     * @return bool
     */
    bool add_row(int pID, string n, string d, string p, int c);

    /**
     * @brief select_elements_sql selects all elements of specified column
     */
    vector<string> select_elements_sql(string);

    /**
     * @brief select_cell selects info from given cell
     */
    vector<string> select_cell(string, string, string);

    /**
     * @brief select_cell_search Uses LIKE SQL query to find valid entries in table
     * @return List of possible queries
     */
    vector<string> select_cell_search(string, string, string);

    /*!
     * \brief sql_select used for select_cell and select_elements_sql methods
     */
    std::string sql_select;

    /*!
     * \brief getMuseumPageFromId returns instance of a museum page from id
     * \param id the id of the museum
     * \return
     */
    MuseumPage* getMuseumPageFromId(int id);
    bool addMuseum(int, string, string, string, int, string);
    vector<string> getMuseumChildrenId(int);
    vector<string> getMuseumChildrenNames(int);
    vector<string> getMuseumByCuratorId(int);
    int getMuseumId(string mname);
    int getMuseumSearchId(string mname);
    //TestMuseumPages table TEARDOWN method
    void remove_entries();

    MuseumPage(DBTool *db,std::string name, bool _isTesting);

    /*!
     * \brief MuseumPage Constructor for this page
     */
    MuseumPage();
    /*!
     * \brief MuseumPage represents one page for this museum
     * \param i the id for this page
     */
    MuseumPage(int i);

    int getId();
    void setId(int);
    string getDescription();
    void setDescription(string);
    string getName();
    void setName(string);
    string getPicString();
    void setPicString(string);
    int getParentId();
    void setParentId(int);
    int getCuratorId();
    void setCuratorId(int);
    void setDB(sqlite3*);

private:
    bool isTesting;
    int id;
    sqlite3* db;
    int curatorId;
    /*!
     * \brief name the name of this page
     */
    string name;
    /*!
     * \brief description description of this page
     */
    string description;
    /*!
     * \brief picString caption of string
     */
    string picString;
    /*!
     * \brief hashtags represents all hashtags for this page
     */
    vector<string> hashtags;
    /*!
     * \brief parentID the id for the parent
     */
    int parentID;
    DBTool* dbtool;

};

    // This is a callback function that is sent to the library and used
    // to parse the sql request being sent to the database.
    int museum_add_row(void  *data,
                   int    argc,
                   char **argv,
                   char **azColName);

    // This is a callback function that is sent to the library and used
    // to parse the sql request being sent to the database.
    int museum_getpage_fromid(void  *data,
                   int    argc,
                   char **argv,
                   char **azColName);

    // This is a callback function that is sent to the library and used
    // to parse the sql request being sent to the database.
    int cb_select(void  *data,
                  int    argc,
                  char **argv,
                  char **azColName);

    // This is a callback function that is sent to the library and used
    // to parse the sql request being sent to the database.
    int cb_select_cell_m(void *data,
                       int argc,
                       char **argv,
                       char **azColName);



#endif // MUSEUMPAGE_H
