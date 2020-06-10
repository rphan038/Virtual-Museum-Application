#include "viewer.h"

/**
 * @brief Viewer::Viewer Default Constructor
 */
Viewer::Viewer()
{

}

/**
 * @brief Viewer::Viewer Constructor that initializes class instance variables with parameters
 * @param _userID The Viewer's ID
 * @param _un The Viewer's Username
 * @param _pw The Viewer's Password
 * @param _priv The Viewer's Privelege level
 * @param _desc The Viewer's self description
 * @param _selfPic The Viewer's Profile picture
 */
Viewer::Viewer(int _userID, string _un, string _pw, string _priv, string _desc, string _selfPic) {
    userID = _userID;
    un = _un;
    pw = _pw;
    privelege = _priv;
    description = _desc;
    selfPic = _selfPic;
}

//Getter and setter methods
int Viewer::getUserID() {return userID;}

void Viewer::setUserID(int x) {userID = x;}

string Viewer::getUn() {return un;}

void Viewer::setUn(string x) {un = x;}

string Viewer::getPw() {return pw;}

void Viewer::setPw(string x) {pw = x;}

string Viewer::getPrivelege() {return privelege;}

void Viewer::setPrivelege(string x) {privelege = x;}

string Viewer::getDescription() {return description;}

void Viewer::setDescription(string x) {description = x;}

string Viewer::getSelfPic() {return selfPic;}

void Viewer::setSelfPic(string x) {selfPic = x;}

string Viewer::getUniquePW() {return uniquePW;}

void Viewer::setUniquePW(string x) {uniquePW = x;}
