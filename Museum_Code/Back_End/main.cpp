#include <iostream>
#include "dbtool.h"
#include "account.h"
#include "viewer.h"
#include "comment.h"
using namespace std;

int main() {
//    DBTool *dbtool = new DBTool("AccountsDB");
//    dbtool->print(std::cout);
//    Account* a = new Account(dbtool, "Accounts");
//    Viewer* x = a->login("Jack Black", "password1234");
//    cout << x->getUserID() << " " << x->getUn() << " " << x->getPw() << " "
//         << x->getPrivelege() << " " << x->getDescription() << " " << x->getSelfPic() << " " << x->getUniquePW() <<endl;
    DBTool *dbtool = new DBTool("AccountsDB");
    dbtool->print(std::cout);
    Comment* c = new Comment(dbtool, "Comments", true);
    vector <string> x = c->getMuseumComments(0);
    for(int i = 0; i < (int) x.size(); i++) {
        cout << x.at(i) << "|";
    }
    return 0;
}
