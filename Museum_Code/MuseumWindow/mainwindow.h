#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../project/account.h"
#include "../project/comment.h"
#include "../project/museumpage.h"
#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QSignalMapper>
#include <QFrame>
#include <QTextEdit>
#include <QListWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTableWidget>
#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initGraph();
    void loadScreen(MuseumPage*);
    void loginScreen();

public slots:
    void l1_clicked();
    void search_clicked();
    void login_clicked();
    void newUser_clicked();
    void submit_clicked();
    void quit_clicked();
    void accountManager_clicked();
    void kick_clicked();
    void genPIN_clicked();
    void goBackToMuseum();
    void submitPic();
    void seeUser();
    void addNewComment_clicked();
    void createNewPage();
    void submitMuseum_clicked();
    void visitSelectedMuseum();
    void removeAComment();

private:
    Ui::MainWindow *ui;
    //Widgets that help with logging in process and displaying of the login screen
    QFrame* lg;
    QLineEdit* inputUn;
    QLineEdit* inputPw;
    QPushButton* loginButton;
    QPushButton* newUserButton;
    QLabel* unL;
    QLabel* pwL;
    QLabel* pinL;
    QLabel* descL;
    //References to backend objects so that databases can be used with the application
    Account* a;
    Comment* c;
    Viewer* currUser;   //Reference to all data about the current user
    //Button for the admin to remove a user
    QPushButton* kick;
    //Allows users to visit another user's profile
    QPushButton* goToUser;
    //Allows users to visit a museum a curator has created
    QPushButton* goToMuseum;
    //Admin can generate a new PIN for a new user to input
    QPushButton* genNewPIN;
    //Contains a list of users in the account management page
    QListWidget* userList;
    //Lists museums a curator has created
    QListWidget* museumList;
    //Logout button
    QPushButton* quit;
    //Tracks whether the Account Manager button has been clicked for the first time
    bool tracker;
    //Used in some methods to keep track of the name of a picture file
    QString pic_file_name;

    //These are used in creating a new account. The text entered into these widgets are used
    QLineEdit* newUn;
    QLineEdit* newPw;
    QLineEdit* uPIN;
    QTextEdit* newDesc;
    QPushButton* newPic;
    QPushButton* submit;

    //These help make the loading of the museumpages work including the picture, description, traversal buttons
    //and account mangement buttons
    MuseumPage* curr;   //Reference to all of the information of the current museum page
    QLabel* pic;    //Displays picture
    QLabel* descLabel;  //Description label of either the museum or user profile
    QLabel* curatorName;    //Displays who created the museum
    QListWidget* children;  //Shows the children/parent museum of the current museum in a list
    QPushButton* visit;     //Button allows user to visit parent and chilren museums
    QPushButton* accountManager;
    //Widgets used in the displaying, addition, and removing of comments
    QTableWidget* commentTb;
    QPushButton* addComment;
    QPushButton* removeComment;
    int entries = 0;

    //Used in search function and the option to create a new museum page
    QLineEdit* inputSearch;
    QPushButton* searchButton;
    QPushButton* newPage;
    QPushButton* submit1;
};
#endif // MAINWINDOW_H
