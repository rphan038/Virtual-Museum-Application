#ifndef ADMIN_H
#define ADMIN_H

#include "curator.h"

class Admin : public Curator {
public:
    //Constructors
    Admin();
    Admin(int _userID, string _un, string _pw, string _priv, string _desc, string _selfPic, string _uniquePW);
};

#endif // ADMIN_H
