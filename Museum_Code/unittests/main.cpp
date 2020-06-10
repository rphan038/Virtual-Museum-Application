#include "gtest/gtest.h"
#include "../project/comment.h"
#include "../project/account.h"
#include "../project/admin.h"
#include "../project/curator.h"
#include "../project/dbtable.h"
#include "../project/museumpage.h"
#include "../project/viewer.h"
#include "../project/dbtable.h"
#include "../project/dbtool.h"


// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    FooTest() {
    // You can do set-up work for each test here.
    }

    virtual ~FooTest() {
    // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.

    /** replace with your own **/


};

TEST(MuseumPageTest, SELECT_ELEMENTS_SQL_VALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    vector<string> x = mp->select_elements_sql("name");
    sqlite3_close(db);
    vector<string> y = { "Home", "Animals", "Presidents", "Countries", "Foods", "Pizza",
                         "Sandwich", "Barack Obama", "Panda", "Soup", "Po Boy", "George Washington",
                         "Turtle", "Abraham Lincoln", "Michelle Obama", "Programming Languages", "C++", "Movies",
                       "The United States of America"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, SELECT_ELEMENTS_SQL_INVALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    //No column named uname
    vector<string> x = mp->select_elements_sql("uname");
    sqlite3_close(db);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, SELECT_CELL_VALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    vector<string> x = mp->select_cell("1", "museumId", "description");
    sqlite3_close(db);
    vector<string> y = {"Welcome to the Museum of "};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, SELECT_CELL_INVALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    //No museumId = 200
    vector<string> x = mp->select_cell("200", "museumId", "description");
    sqlite3_close(db);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, SELECT_CELL_SEARCH_VALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    vector<string> x = mp->select_cell_search("Pro%", "name", "museumId");
    sqlite3_close(db);
    vector<string> y = {"16"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, SELECT_CELL_SEARCH_INVALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    //No LIKE pattern defined
    vector<string> x = mp->select_cell_search("Pro", "name", "museumId");
    sqlite3_close(db);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGEFROMID_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    mp = mp->getMuseumPageFromId(1);

    ASSERT_EQ(mp->getDescription(), "Welcome to the Museum of ") << mp->getDescription() << " and "
                                                                 << "Welcome to the Museum of "
                                                                 << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGEFROMID_OUTOFBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    mp = mp->getMuseumPageFromId(200);
    bool x = false;
    if(mp == NULL)
        x = true;

    ASSERT_EQ(x, true) << x << " and "<< true << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGEFROMID_INBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    mp = mp->getMuseumPageFromId(11);

    ASSERT_EQ(mp->getName(), "Po Boy") << mp->getDescription() << " and "
                                                                 << "Po Boy"
                                                                 << " should match!";
}

TEST(MuseumPageTest, ADDMUSEUM_VALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    sqlite3* db;
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    mp->remove_entries();
    bool x = mp->addMuseum(14, "Random", "A Random Museum", "pic.jpg", 1, "Curator");
    sqlite3_close(db);
    ASSERT_EQ(x, true) << x << " and " << true << " should match!";
}

TEST(MuseumPageTest, ADDMUSEUM_INVALID1) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    //Not an Admin or Curator
    bool x = mp->addMuseum(10, "Random", "", "pic.pic", 20, "Viewer");

    ASSERT_NE(x, true) << x << " and " << true << " should match!";
}

TEST(MuseumPageTest, ADDMUSEUM_INVALID2) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    //Museum name cannot be empty
    bool x = mp->addMuseum(1, "", "", "pic.pic", 20, "Curator");

    ASSERT_NE(x, true) << x << " and " << true << " should match!";
}

TEST(MuseumPageTest, ADDMUSEUM_INVALID3) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    sqlite3* db;
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    mp->setDB(db);
    mp->remove_entries();
    mp->addMuseum(11, "", "", "pic.pic", 20, "Curator");
    //Duplicate museum titles
    bool x = mp->addMuseum(11, "", "", "pic.pic", 20, "Curator");
    sqlite3_close(db);
    ASSERT_EQ(x, false) << x << " and " << false << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENID_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    vector<string> x = mp->getMuseumChildrenId(1);
    vector<string> y = {"2", "3", "4", "5", "16", "18"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENID_OUTBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "TestMuseumPages",true);
    vector<string> x = mp->getMuseumChildrenId(90);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENID_INBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumChildrenId(5);
    vector<string> y = {"6", "7", "10"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENNAMES_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumChildrenNames(1);
    vector<string> y = {"Animals", "Presidents", "Countries", "Foods", "Programming Languages", "Movies"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENNAMES_INBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumChildrenNames(5);
    vector<string> y = {"Pizza", "Sandwich", "Soup"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMCHILDRENNAMES_OUTBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumChildrenNames(200);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMBYCURATORID_INBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumByCuratorId(2);
    vector<string> y = {"Soup", "Movies", "The United States of America"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}


TEST(MuseumPageTest, GETMUSEUMBYCURATORID_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumByCuratorId(6);
    vector<string> y = {"Po Boy", "Abraham Lincoln"};

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}


TEST(MuseumPageTest, GETMUSEUMBYCURATORID_OUTBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    vector<string> x = mp->getMuseumByCuratorId(200000);
    vector<string> y;

    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}


TEST(MuseumPageTest, GETMUSEUMID_INBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    int x = mp->getMuseumId("Turtle");
    int y = 13;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMID_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    int x = mp->getMuseumId("Home");
    int y = 1;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMID_OUTBOUNDS) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    int x = mp->getMuseumId("ScoobyDOO");
    int y = 0;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGESEARCH_VALID1) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    int x = mp->getMuseumSearchId("Progr");
    int y = 16;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGESEARCH_VALID2) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    int x = mp->getMuseumSearchId("hom");
    int y = 1;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(MuseumPageTest, GETMUSEUMPAGESEARCH_INVALID) {
    DBTool *dbtool1 = new DBTool("MuseumPagesDB");
    dbtool1->print(std::cout);
    MuseumPage* mp = new MuseumPage(dbtool1, "MuseumPages",true);
    //No Museum page name that is close to the input
    int x = mp->getMuseumSearchId("9");
    int y = 0;

    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, LOGIN_VALID1) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("Will Ferrell", "0");
    string x = v->getDescription();
    string y = "Hey there";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, LOGIN_VALID2) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("Will Ferrell", "0");
    string x = v->getUn();
    string y = "Will Ferrell";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, LOGIN_VALID3) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("Will Ferrell", "0");
    string x = v->getPrivelege();
    string y = "Curator";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, LOGIN_INVALID1) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("Will Ferrell", "010");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, LOGIN_INVALID2) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("WilL Ferrell", "0");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

//Checks "deleted" accounts
TEST(AccountTest, LOGIN_INVALID3) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    Viewer* v = a->login("Stud1", "1");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, ADDNEWUSER_VALID) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    Viewer* v = a->addNewUser("Prof", "1", "V200", "I'm a prof", "png.png");
    bool x = true;
    if(v == NULL)
        x = false;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, ADDNEWUSER_INVALID1) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    a->addNewUser("Prof", "10000", "V200", "Rand", "pic");
    //Can't add new account with the an existing username
    Viewer* v = a->addNewUser("Prof", "10000", "V200", "Rand", "pic");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, ADDNEWUSER_INVALID2) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    //Incorrect authentication PIN
    Viewer* v = a->addNewUser("Stud500", "10000", "V20", "Rand", "pic");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, ADDNEWUSER_INVALID3) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    //Blank username and password
    Viewer* v = a->addNewUser("", "10000", "V200", "Rand", "pic");
    bool x = false;
    if(v == NULL)
        x = true;
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, REMOVEUSER_INVALID1) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    //Can't add new account with the an existing username
    string x = a->removeUser("COMP2");
    string y = "";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, REMOVEUSER_VALID) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    a->addNewUser("UHAHAHA", "10000", "V200", "Rand", "pic");
    //Can't add new account with the an existing username
    string x = a->removeUser("UHAHAHA");
    string y = "pic";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, NEWPIN_VALID) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    a->addNewUser("", "10000", "V200", "Rand", "pic");
    //Can't add new account with an existing username
    bool x = a->newPIN("COMP2");
    bool y = true;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, NEWPIN_INVALID) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "TestAccounts",true);
    a->remove_entries();
    a->addNewUser("", "10000", "V200", "Rand", "pic");
    //Can't add new account with the an existing username
    bool x = a->newPIN("UU");
    bool y = false;
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, USERDATA_BOUNDARY) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    vector<string> x = a->userData("Jack Black");
    vector<string> y = {"JackBlack.jpg", "Hi I am the Admin of this museum", "1", "Admin"};
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(AccountTest, USERDATA_INBOUNDS) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    vector<string> x = a->userData("Kevin Hart");
    vector<string> y = {"KevinHart.jpg", "An actor", "5", "Curator"};
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(AccountTest, USERDATA_OUTBOUNDS) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    vector<string> x = a->userData("Jackie Chan");
    vector<string> y = {"", "", "", ""};
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(AccountTest, GETNAMEFROMID_INBOUNDS) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    string x = a->getNameFromId(3);
    string y = "A Name";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, GETNAMEFROMID_BOUNDARY) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    string x = a->getNameFromId(1);
    string y = "Jack Black";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(AccountTest, GETNAMEFROMID_OUTBOUNDS) {
    DBTool* dbtool1 = new DBTool("AccountsDB");
    dbtool1->print(std::cout);
    Account* a = new Account(dbtool1, "Accounts",true);
    //Can't add new account with the an existing username
    string x = a->getNameFromId(1000);
    string y = "";
    ASSERT_EQ(x, y) << x << " and " << y << " should match!";
}

TEST(CommentTest, NEWCOMMENTTEST_VALID) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "TestComments",true);
    c->remove_entries();
    sqlite3* db;
    c->newComment(1, "2", "COMMENTTTTT");
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string t = c->select_cells("1", "commentId", "content").at(0);
    sqlite3_close(db);
    ASSERT_EQ(t, "COMMENTTTTT") << t << " and " << "COMMENTTTTT" << " should match!";
}


TEST(CommentTest, SELECTCELLSTEST_VALID) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "TestComments",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string t = c->select_cells("1", "commentId", "content").at(0);
    sqlite3_close(db);
    ASSERT_EQ(t, "COMMENTTTTT") << t << " and " << "THISCOMMENT" << " should match!";
}

TEST(CommentTest, SELECTCELLSTEST_INVALID) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "TestComments",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string t = c->select_cells("1", "commentId", "content").at(0);
    sqlite3_close(db);
    ASSERT_NE(t, "something") << t << " and " << "something" << " should not match!";
}


TEST(CommentTest, REMOVECOMMENTTEST_VALID) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "TestComments",true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    c->remove_entries();
    sqlite3_close(db);
    c->newComment(1, "2", "THISCOMMENT");
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string g = c->select_cells("1", "commentId", "deleted").at(0);
    sqlite3_close(db);
    c->removeComment("THISCOMMENT");
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string m = c->select_cells("1", "commentId", "deleted").at(0);
    sqlite3_close(db);
    ASSERT_NE(g, m) << g << " and " << m << " should not match!";
}

TEST(CommentTest, REMOVECOMMENTEST_INVALID) {
    DBTool *dbtool = new DBTool("CommentsDB");
    dbtool->print(std::cout);
    Comment* c = new Comment(dbtool, "TestComments", true);
    sqlite3* db;
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string g = c->select_cells("1", "commentId", "deleted").at(0);
    sqlite3_close(db);
    //Comment has already been removed
    c->removeComment("THISCOMMENT");
    sqlite3_open("../../museum/project/museum.sqlite", &db);
    c->setDB(db);
    string m = c->select_cells("1", "commentId", "deleted").at(0);
    sqlite3_close(db);
    ASSERT_EQ(g, m) << g << " and " << m << " should not match!";
}

TEST(CommentTest, GETMUSEUMCOMMENTSTEST_INBOUND) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "Comments",true);
    vector<string> x = c->getMuseumComments(5);
    vector<string> y = {"8", "5", "Jack Black", "Food is great", "0"};
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(CommentTest, GETMUSEUMCOMMENTSTEST_BOUNDARY) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "Comments",true);
    vector<string> x = c->getMuseumComments(18);
    vector<string> y =  { "21", "18", "Kevin Hart", "My movies are the best movies out there",
                          "0", "46", "18", "Will Ferrell", "No mine are", "0", "49", "18",
                          "Will Ferrell", "Movies are great", "0" };
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}

TEST(CommentTest, GETMUSEUMCOMMENTSTEST_OUTBOUND) {
    DBTool *dbtool1 = new DBTool("CommentsDB");
    DBTool("CommentsDB");
    dbtool1->print(std::cout);
    Comment *c = new Comment(dbtool1, "Comments",true);
    vector<string> x = c->getMuseumComments(0);
    vector<string> y;
    ASSERT_EQ(x, y) << x.at(0) << " and " << y.at(0) << " should match!";
}




TEST_F(FooTest, TESTFIXTURE) {

    /** replace with your own **/
    ASSERT_EQ(1, 1) << "These should match!";
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
