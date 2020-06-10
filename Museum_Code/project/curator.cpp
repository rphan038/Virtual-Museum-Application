#include "curator.h"

/**
 * @brief Curator::Curator Default constructor
 */
Curator::Curator()
{

}

/**
 * @brief Curator::Curator Constructor that initializes all class instance variables
 * @param _userID Curator's ID
 * @param _un Curator's username
 * @param _pw Curator's password
 * @param _priv Curator's privelege level
 * @param _desc Curator's self description
 * @param _selfPic Curator's Profile Picture
 */
Curator::Curator(int _userID, string _un, string _pw, string _priv, string _desc, string _selfPic) {
    userID = _userID;
    un = _un;
    pw = _pw;
    privelege = _priv;
    description = _desc;
    selfPic = _selfPic;
}
