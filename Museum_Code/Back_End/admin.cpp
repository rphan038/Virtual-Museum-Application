#include "admin.h"

/**
 * @brief Admin::Admin Default Constructor
 */
Admin::Admin()
{

}

/**
 * @brief Admin::Admin Constructor that initializes all of the class instance variables
 * @param _userID Admin's ID
 * @param _un Admin's username
 * @param _pw Admin's password
 * @param _priv Admin's privelege level
 * @param _desc Admin's self description
 * @param _selfPic Admin's profile picture
 * @param _uniquePW Admin's determined unique authentication PIN password
 */
Admin::Admin(int _userID, string _un, string _pw, string _priv, string _desc, string _selfPic, string _uniquePW) {
    userID = _userID;
    un = _un;
    pw = _pw;
    privelege = _priv;
    description = _desc;
    selfPic = _selfPic;
    uniquePW = _uniquePW;
}
