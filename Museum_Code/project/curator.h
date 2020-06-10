#ifndef CURATOR_H
#define CURATOR_H
#include "viewer.h"
using namespace std;

class Curator : public Viewer {
public:
    /*!
     * \brief Curator constuctor for curator
     */
    Curator();
    Curator(int _userID, string _un, string _pw, string _priv, string _desc, string _selfPic);
};

#endif // CURATOR_H
