#ifndef COMMENT_H
#define COMMENT_H
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
using namespace std;

class Comment: public DBTable {
public:

    /*!
     * \brief Comment constructor
     * \param db database tool for all comments
     * \param name name for database
     */
    Comment(DBTool* db, string name, bool _isTesting);

    /**
     * @brief store_create_sql sql statement for creates on in table
     */
    void store_create_sql();

    /**
     * @brief store_add_row_sql sql statement for row add on ining to table
     */
    virtual void store_add_row_sql();

    /*!
     * \brief add_row adds entry to Comments database
     * \param museumId the museum id field
     * \param author the author field
     * \param content the content of the comment
     * \return bool
     */
    bool add_row(int museumId, string author, string content, string deleted);
    /*!
     * \brief select_cells selects a value for a single field for a single element in the table
     * \param whichRow the index of the row we want in the database
     * \param row the name of the primary key
     * \param col the field that we are looking for
     * \return vector of selected cells
     */
    vector<string> select_cells(string whichRow, string row, string col);
    /*!
     * \brief select_elements_sql gets the entire column of a certain field in the database
     * \param col the field we are searching for
     * \return vector of selected elements
     */
    vector<string> select_elements_sql(string col);
    /*!
     * \brief newComment basically just adds all functionality for
     *                  a new comment; calls add_row
     * \param mID the id of the museum that is being commented on
     * \param a the author of the comment
     * \param c the content in the comment
     */
    void newComment(int mID, string a, string c);

    /**
     * @brief Comment::update_value Updates the value of a given cell
     * @param colName The column name of the cell that needs to be changed
     * @param newValue New value of the cell
     * @param rowName The column name where the rowValue will be matched with
     * @param rowValue Decides which row to update
     */
    void update_value(string colName, string newValue, string rowName, string rowValue);

    /**
     * @brief removeComment Changes the boolean status to "Deleted" according to a given comment string
     * @param content The comment to be deleted
     * @return whether the deletion was successful or not
     */
    bool removeComment(string content);

    /*!
     * \brief count_rows
     * return number of rows
     */
    int count_rows();

    //Gets all of the comments for a given museum id
    vector<string> getMuseumComments(int);

    /*!
     * \brief setComment sets the comment for the current string
     * \param s the comment we are writing
     */
    void setComment(string s);
    void remove_entries();
    void setDB(sqlite3*);

protected:
    string sql_select_all;
    string sql_select;

private:
    sqlite3* db;
    bool isTesting;

};
int comment_add_row(void  *data,
                   int    argc,
                   char **argv,
                  char **azColName);
int cb_select_cell_c(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

int cb_select_c(void  *data,
              int    argc,
              char **argv,
              char **azColName);
int cb_count_rows_c(void *data,
                   int argc,
                   char **argv,
                   char **azColName);
int cb_update_value_c(void *data,
                   int argc,
                   char **argv,
                   char **azColName);

#endif // COMMENT_H
