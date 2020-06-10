#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Constructor for GUI window and initializes most of the widgets/variables used in the
 * app. The purpose of each widget is described in the .h file
 * @param parent Reference to a QWidget object
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lg = new QFrame(this);
    inputUn = new QLineEdit(this);
    inputPw = new QLineEdit(this);
    loginButton = new QPushButton(this);
    newUserButton = new QPushButton(this);
    unL = new QLabel(this);
    pwL = new QLabel(this);
    pinL = new QLabel(this);
    descL = new QLabel(this);
    quit = new QPushButton(this);
    quit->move(660, 530);
    quit->setText("Logout");
    connect(quit, SIGNAL(clicked()), this, SLOT(quit_clicked()));
    kick = new QPushButton(this);
    genNewPIN = new QPushButton(this);
    goToUser = new QPushButton(this);
    goToMuseum = new QPushButton(this);
    connect(genNewPIN, SIGNAL(clicked()), this, SLOT(genPIN_clicked()));
    connect(kick, SIGNAL(clicked()), this, SLOT(kick_clicked()));
    connect(goToUser, SIGNAL(clicked()), this, SLOT(seeUser()));
    connect(goToMuseum, SIGNAL(clicked()), this, SLOT(visitSelectedMuseum()));
    userList = new QListWidget(this);
    museumList = new QListWidget(this);
    tracker = false;
    pic_file_name = "";

    newUn = new QLineEdit(this);
    newPw = new QLineEdit(this);
    uPIN = new QLineEdit(this);
    newDesc = new QTextEdit(this);
    newPic = new QPushButton(this);
    connect(newPic, SIGNAL(clicked()), this, SLOT(submitPic()));
    submit = new QPushButton(this);
    connect(submit, SIGNAL(clicked()), this, SLOT(submit_clicked()));

    pic = new QLabel(this);
    descLabel = new QLabel(this);
    curatorName = new QLabel(this);
    commentTb = new QTableWidget(this);
    commentTb->setRowCount(20);
    commentTb->setColumnCount(2);
    addComment = new QPushButton(this);
    connect(addComment, SIGNAL(clicked()), this, SLOT(addNewComment_clicked()));
    removeComment = new QPushButton(this);
    connect(removeComment, SIGNAL(clicked()), this, SLOT(removeAComment()));
    visit = new QPushButton(this);
    connect(visit, SIGNAL (clicked()), this, SLOT(l1_clicked()));
    children = new QListWidget(this);
    accountManager = new QPushButton(this);
    connect(accountManager, SIGNAL(clicked()), this, SLOT(accountManager_clicked()));

    inputSearch = new QLineEdit(this);
    inputSearch->move(600, 10);
    searchButton = new QPushButton(this);
    searchButton->setText("Search");
    searchButton->move(700, 10);
    connect(searchButton, SIGNAL (clicked()), this, SLOT(search_clicked()));
    newPage = new QPushButton(this);
    connect(newPage, SIGNAL(clicked()), this, SLOT(createNewPage()));
    submit1 = new QPushButton(this);
    connect(submit1, SIGNAL(clicked()), this, SLOT(submitMuseum_clicked()));
    loginScreen();
}

/**
 * @brief MainWindow::~MainWindow Destructor for the GUI window
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::initGraph Sets up the basic layout of the app including traversal buttons, comment table/buttons
 * and loading the home page of the museum
 */
void MainWindow::initGraph() {
    //Initializes the current museum to be the home page
    DBTool *dbtool = new DBTool("MuseumsDB");
    dbtool->print(std::cout);
    curr = new MuseumPage(dbtool, "MuseumPages", false);
    curr = curr->getMuseumPageFromId(1);
    //Traversal widgets
    visit->setFixedSize(100, 30);
    visit->move(370, 540);
    visit->setText("Visit");
    children->move(370, 430);
    children->setFixedSize(100, 100);
    //Comment widgets
    commentTb->move(25, 100);
    commentTb->setFixedSize(200, 250);
    commentTb->horizontalHeader()->hide();
    commentTb->verticalHeader()->hide();
    commentTb->setWordWrap(true);
    commentTb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    descLabel->setFixedSize(500, 100);
    descLabel->setWordWrap(true);
    descLabel->move(260, 300);
    curatorName->setFixedSize(200, 30);
    curatorName->move(300, 400);
    addComment->move(50, 380);
    addComment->setFixedSize(120, 30);
    addComment->setText("Add Comment");
    museumList->clear();
    userList->clear();
    inputSearch->clear();
    tracker = false;
    //Buttons reserved for certain level of users
    if(currUser->getPrivelege() == "Admin") {
        removeComment->move(45, 410);
        removeComment->setFixedSize(150, 30);
        removeComment->setText("Remove Comment");
    }
    if(currUser->getPrivelege() != "Viewer") {
        newPage->move(650, 475);
        newPage->setFixedSize(130, 30);
        newPage->setText("Create New Page");
    }
    //Loads home page
    loadScreen(curr);
}

/**
 * @brief MainWindow::loadScreen Loads the screen given a MuseumPage object, which contains all required data
 * @param mp Reference to a MuseumPage that is to be displayed
 */
void MainWindow::loadScreen(MuseumPage* mp) {
    //Will return the rows in which the museumId field equals mp->getId()
    vector<string> data = c->getMuseumComments(mp->getId());
    int row = 0;
    int col = 0;
    commentTb->clear();
    entries = 0;
    //Third index is the comment content. Add this to first column of comment table
    for(int i = 3; i < (int) data.size(); i += 5) {
        QString s = data.at(i).c_str();
        QTableWidgetItem* w = new QTableWidgetItem(s);
        if(i >= 20)
            commentTb->insertRow(commentTb->rowCount());
        if(data.at(i + 1) == "0") {
            commentTb->setItem(row, col, w);
            row++;
            entries++;
        }
    }
    row = 0;
    col++;
    //Second index is the author of the comment. Add this to the second column of comment table
    for(int i = 2; i < (int) data.size(); i += 5) {
        QString s = data.at(i).c_str();
        QTableWidgetItem* w = new QTableWidgetItem(s);
        if(data.at(i + 2) == "0") {
            commentTb->setItem(row, col, w);
            row++;
        }
    }
    commentTb->resizeRowsToContents();
    //Load the picture associated to the museum
    if(mp->getPicString() != "") {
        string path = "../../../../../museum/project/" + mp->getPicString();
        QImage image;
        QPixmap qmap;
        qmap.load(path.c_str());
        image.load(path.c_str());
        pic->setFixedSize(300, 300);
        pic->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        pic->move(275, 0);
        pic->setPixmap(QPixmap::fromImage(image));
        int h = pic->height();
        int w = pic->width();
        pic->setPixmap(qmap.scaled(w,h,Qt::KeepAspectRatio));
        pic->show();
    }
    //Loads the description and author of the museum page
    descLabel->setText(mp->getDescription().c_str());
    string cname = "Created By : ";
    cname += a->getNameFromId(mp->getCuratorId());
    curatorName->setText(cname.c_str());
    //Finds the museum's children museums using the current museum's ID
    vector<string> x = mp->getMuseumChildrenNames(mp->getId());
    //Looks for the parent museum if the current museum if the current museum is not Home
    children->clear();
    if(mp->getId() != 1) {
        MuseumPage* m = mp->getMuseumPageFromId(mp->getParentId());
        const QString s1 = QString::fromStdString(m->getName().c_str());
        children->addItem(s1);
    }
    //Adds all children museums of the current museum to the list widget
    for(int i = 0; i < (int) x.size(); i++) {
        const QString s = QString::fromStdString(x.at(i));
        children->addItem(s);
    }
}

/**
 * @brief MainWindow::loginScreen Turns off/on appropriate widgets to display the login screen + allow login functions
 */
void MainWindow::loginScreen() {
    inputSearch->hide();
    searchButton->hide();
    newPage->hide();
    visit->hide();
    children->hide();
    submit1->hide();
    commentTb->hide();
    curatorName->hide();
    addComment->hide();
    removeComment->hide();
    accountManager->hide();
    userList->hide();
    museumList->hide();
    kick->hide();
    goToUser->hide();
    goToMuseum->hide();
    genNewPIN->hide();
    newUn->hide();
    newPw->hide();
    uPIN->hide();
    newDesc->hide();
    newPic->hide();
    submit->hide();
    quit->hide();
    pic->clear();
    //Display "Login" title
    pic->setText("Login");
    QFont font = pic->font();
    font.setPointSize(27);
    pic->setFont(font);
    pic->move(380, 0);
    pic->setFixedSize(100, 50);
    pinL->hide();
    descL->hide();
    descLabel->hide();
    lg->setFixedSize(250, 250);
    lg->move(300, 200);
    lg->setStyleSheet("background-color: rgb(120, 215, 236);");
    lg->show();
    inputUn->move(375, 250);
    inputUn->clear();
    inputUn->show();
    inputPw->move(375, 305);
    inputPw->clear();
    inputPw->show();
    unL->move(375, 225);
    unL->setText("Username");
    unL->show();
    pwL->move(375, 280);
    pwL->setText("Password");
    pwL->show();
    loginButton->move(375, 350);
    loginButton->setText("Login");
    loginButton->show();
    connect(loginButton, SIGNAL(clicked()), this, SLOT(login_clicked()));

    newUserButton->move(375, 375);
    newUserButton->setText("New User?");
    newUserButton->show();
    connect(newUserButton, SIGNAL(clicked()), this, SLOT(newUser_clicked()));
}

/**
 * @brief MainWindow::newUser_clicked Slot for when the newUserButton has been clicked
 * Changes the display to allow the user to input data to create a new account. Modifies widgets as appropriate
 */
void MainWindow::newUser_clicked() {
    pic->setText("Create A New Account");
    QFont font = pic->font();
    font.setPointSize(20);
    pic->setFixedWidth(310);
    pic->move(325, 0);
    pic->setFont(font);
    lg->setStyleSheet("background-color: rgb(237, 101, 90);");
    lg->setFixedSize(250, 400);
    lg->move(300,100);
    inputUn->hide();
    inputPw->hide();
    loginButton->hide();
    newUserButton->hide();
    newUn->show();
    newPw->show();
    uPIN->show();
    newDesc->show();
    newPic->show();
    submit->show();

    newUn->move(375, 130);
    newUn->clear();
    unL->move(375, 105);
    unL->setText("New Username");
    newPw->move(375, 180);
    newPw->clear();
    pwL->move(375, 155);
    pwL->setText("New Password");
    uPIN->move(375, 225);
    uPIN->clear();
    pinL->move(350, 200);
    pinL->setText("Enter Authentication PIN");
    pinL->setFixedWidth(200);
    pinL->show();
    newPic->move(375, 260);
    newPic->setText("Save Picture");
    newDesc->move(370, 310);
    newDesc->setFixedSize(110, 130);
    newDesc->clear();
    descL->move(379, 285);
    descL->setText("Self Description");
    descL->show();
    submit->move(375, 460);
    submit->setText("Submit");
}

/**
 * @brief MainWindow::submitPic Slot for the submission of a picture. Uses the QFileDialog widget to allow the user
 * to add their own picture to the system. The code will add this picture to the project directory and save the filename
 */
void MainWindow::submitPic() {
    QString filter = "Image File (*.png *.jpg *.jpeg)";
    QString file_name = QFileDialog::getOpenFileName(this, "Submit A Picture", QDir::homePath(), filter);
    QFileInfo fo(file_name);
    QString name = fo.fileName();
    QString newPath = "../../../../../museum/project/" + name;
    if(!QFile::copy(file_name, newPath)) {
        QMessageBox::information(this, tr("Picture"), tr("Rename picture file:File name already exists"));
        return;
    }
    pic_file_name = name;
}

/**
 * @brief MainWindow::submit_clicked Slot for the submission of new user data. Saves this submitted data and
 * communicates with the database to save data
 */
void MainWindow::submit_clicked() {
    QString UN = newUn->text();
    string str = UN.toStdString();
    QString PW = newPw->text();
    string str1 = PW.toStdString();
    QString PIN = uPIN->text();
    string str2 = PIN.toStdString();
    QString DESC = newDesc->toPlainText();
    string str3 = DESC.toStdString();
    string str4 = pic_file_name.toStdString();

    //Allocation of memory for a new Account object. Adds a new user to the database
    DBTool *dbtool = new DBTool("AccountsDB");
    dbtool->print(std::cout);
    a = new Account(dbtool, "Accounts", false);
    currUser = a->addNewUser(str, str1, str2, str3, str4);
    //The addNewUser detects if there are any formatting or invalid PIN issues. Prompts the user to re enter data
    //if there is a problem. If not proceeds to the home page of the museum
    if(currUser == NULL) {
        QMessageBox::information(this, tr("New user"), tr("Error(s): \n -Invalid PIN Number \n -Username taken"));
    } else {
        newUn->hide();
        newPw->hide();
        uPIN->hide();
        newDesc->hide();
        newPic->hide();
        submit->hide();
        lg->hide();
        pic->clear();
        unL->hide();
        pwL->hide();
        pinL->hide();
        descL->hide();

        inputSearch->show();
        searchButton->show();
        if(currUser->getPrivelege() != "Viewer")
            newPage->show();
        visit->show();
        children->show();
        descLabel->show();
        quit->show();
        commentTb->show();
        curatorName->show();
        addComment->show();
        accountManager->show();
        accountManager->setText(currUser->getUn().c_str());
        accountManager->move(50, 10);
        DBTool *dbtool1 = new DBTool("CommentsDB");
        dbtool1->print(std::cout);
        c = new Comment(dbtool1, "Comments", false);
        initGraph();
    }
}

/**
 * @brief MainWindow::login_clicked Slot for when the loginButton is clicked
 */
void MainWindow::login_clicked() {
    QString UN = inputUn->text();
    string str = UN.toStdString();
    QString PW = inputPw->text();
    string str1 = PW.toStdString();
    DBTool *dbtool = new DBTool("AccountsDB");
    dbtool->print(std::cout);
    a = new Account(dbtool, "Accounts", false);
    DBTool *dbtool1 = new DBTool("CommentsDB");
    dbtool1->print(std::cout);
    c = new Comment(dbtool1, "Comments", false);
    //Communicates with the database to see if the login information is correct. If not, prompts user to re enter
    //If login is successful, proceeds to the home page
    currUser = a->login(str, str1);
    if(currUser == NULL) {
        QMessageBox::information(this, tr("Login"), tr("Invalid Username or Password"));
    } else {
        lg->hide();
        inputUn->hide();
        inputPw->hide();
        loginButton->hide();
        newUserButton->hide();
        inputSearch->show();
        searchButton->show();
        if(currUser->getPrivelege() != "Viewer")
            newPage->show();
        visit->show();
        children->show();
        pic->clear();
        unL->hide();
        pwL->hide();
        pinL->hide();
        descL->hide();
        descLabel->show();
        quit->show();
        commentTb->show();
        curatorName->show();
        addComment->show();
        if(currUser->getPrivelege() == "Admin")
            removeComment->show();
        accountManager->show();
        accountManager->setText(currUser->getUn().c_str());
        accountManager->move(50, 10);
        initGraph();
    }
}

/**
 * @brief MainWindow::createNewPage SLot for the creation of a new museum page. Rearranges widgets appropriately
 */
void MainWindow::createNewPage() {
    inputSearch->hide();
    searchButton->hide();
    newPage->hide();
    visit->hide();
    children->hide();
    pic->setText("Create a New Museum");
    QFont font = pic->font();
    font.setPointSize(20);
    pic->setFont(font);
    pic->move(330, 0);
    pic->setFixedSize(300, 50);
    descLabel->hide();
    commentTb->hide();
    curatorName->hide();
    addComment->hide();
    removeComment->hide();
    //Allows the user to go back to the museum after the museum has been created
    accountManager->setText("Museum");
    connect(accountManager, SIGNAL(clicked()), this, SLOT(goBackToMuseum()));

    //Museum Name
    newUn->show();
    newUn->clear();
    //Museum Description
    newDesc->show();
    newDesc->clear();
    //Picture
    newPic->show();
    submit1->show();

    newUn->move(375, 200);
    unL->move(375, 175);
    unL->setText("Museum Name");
    unL->show();
    newDesc->move(365, 255);
    newDesc->setFixedSize(125, 135);
    pwL->move(365, 230);
    pwL->setText("Museum Description");
    pwL->setFixedWidth(300);
    pwL->show();
    newPic->move(375, 410);
    newPic->setText("Save Picture");
    submit1->move(375, 480);
    submit1->setText("Submit");
    lg->setStyleSheet("background-color: rgb(105, 28, 30);");
    lg->setFixedSize(250, 375);
    lg->move(300, 150);
    lg->show();
}

/**
 * @brief MainWindow::submitMuseum_clicked Slot for when submit1 is clicked. Communicates with the MuseumPages
 * database to add a new museum entry. If there are any errors, prompts user to re enter data.
 */
void MainWindow::submitMuseum_clicked() {
    QString name = newUn->text();
    string str = name.toStdString();
    QString desc = newDesc->toPlainText();
    string str1 = desc.toStdString();
    string str2 = pic_file_name.toStdString();

    //Possible errors that could occur are if the picture file they submitted has the same name as another picture
    //file in the system. If the museum title already exists in the system, this will also result in an error
    if(!curr->addMuseum(curr->getId(), str, str1, str2, currUser->getUserID(), currUser->getPrivelege())) {
        QMessageBox::information(this, tr("New Museum Page"),
                                 tr("Error(s): \n -Museum name exists \n "
                                    "-Incorrect museum entry format \n -No picture submitted"));
        QFile file("../../../../../museum/project/" + pic_file_name);
        file.remove();
    } else {
        QMessageBox::information(this, tr("New Museum Page"), tr("Museum Added Successfully"));
    }
}

/**
 * @brief MainWindow::accountManager_clicked Slot for when accountManager is clicked. Opens options for the user to
 * look through curator profiles as well as the museums each curator has created. If the user is an admin, it allows
 * them to also remove users. This method also changes the slot for the accountManager button to return to museum
 */
void MainWindow::accountManager_clicked() {
    inputSearch->hide();
    searchButton->hide();
    newPage->hide();
    visit->hide();
    children->hide();
    commentTb->hide();
    curatorName->hide();
    addComment->hide();
    removeComment->hide();
    if(currUser->getSelfPic() != "") {
        string path = "../../../../../museum/project/" + currUser->getSelfPic();
        QImage image;
        QPixmap qmap;
        qmap.load(path.c_str());
        image.load(path.c_str());
        pic->setFixedSize(300, 300);
        pic->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        pic->move(275, 0);
        pic->setPixmap(QPixmap::fromImage(image));
        int h = pic->height();
        int w = pic->width();
        pic->setPixmap(qmap.scaled(w,h,Qt::KeepAspectRatio));
        pic->show();
    } else {
        pic->hide();
    }
    descLabel->setText(currUser->getDescription().c_str());
    userList->show();
    userList->move(50, 100);
    userList->setFixedSize(110, 130);
    museumList->show();
    museumList->move(650, 100);
    museumList->setFixedSize(110, 130);
    goToUser->show();
    goToUser->move(55, 230);
    goToUser->setText("User Profile");
    goToMuseum->show();
    goToMuseum->move(655, 230);
    goToMuseum->setText("Visit");
    //Connects the accountManager to return to the museum that was visited prior to clicking the button
    connect(accountManager, SIGNAL(clicked()), this, SLOT(goBackToMuseum()));
    accountManager->setText("Museum");
    //Adds elements to the QListWidget according to what the user has clicked
    //The boolean tracker variable makes sure that things are not repeatedly added to the listwidget
    if(!tracker) {
        //Lists all the valid users in the system
        for(int i = 0; i < (int) a->getAllUsernames().size(); i++) {
            const QString s = QString::fromStdString(a->getAllUsernames().at(i));
            userList->addItem(s);
        }
        //Lists all of the museums the curator has created
        vector<string> m = curr->getMuseumByCuratorId(currUser->getUserID());
        for(int j = 0; j < (int) m.size(); j++) {
            const QString str = QString::fromStdString(m.at(j));
            museumList->addItem(str);
        }
        //Turns true after the first time accountManager is clicked. It will never turn false again
        tracker = true;
    }
    //Turns on additional buttons if the user is the admin
    if(currUser->getPrivelege() == "Admin") {
        kick->show();
        kick->move(55, 265);
        kick->setText("Remove User");
        genNewPIN->show();
        genNewPIN->move(50, 300);
        genNewPIN->setFixedSize(120, 30);
        genNewPIN->setText("Enter New PIN");
    }
}

/**
 * @brief MainWindow::seeUser Slot for when the user has clicked an option on QListWidget and wants to see the user
 * profile. Loads all of the stored information about tha user->picture, description, created museums
 */
void MainWindow::seeUser() {
    QListWidgetItem* item = userList->currentItem();
    //If nothing is clicked on QListWidget, return to avoid crashes
    if(item == NULL)
        return;
    QString name = item->text();
    //Index zero is the user's picString index one is description and index two is the id and index three is privelege
    vector<string> tmp = a->userData(name.toStdString());
    //Loads the profile picture
    if(tmp.at(0) != "") {
        string path = "../../../../../museum/project/" + tmp.at(0);
        QImage image;
        QPixmap qmap;
        qmap.load(path.c_str());
        image.load(path.c_str());
        pic->setFixedSize(300, 300);
        pic->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        pic->move(275, 0);
        pic->setPixmap(QPixmap::fromImage(image));
        int h = pic->height();
        int w = pic->width();
        pic->setPixmap(qmap.scaled(w,h,Qt::KeepAspectRatio));
        pic->show();
    } else {
        pic->hide();
    }
    //Loads the list of museums the curator has created
    if(tmp.at(3) != "Visitor") {
        museumList->clear();
        vector<string> m = curr->getMuseumByCuratorId(stoi(tmp.at(2)));
        for(int j = 0; j < (int) m.size(); j++) {
            const QString str = QString::fromStdString(m.at(j));
            museumList->addItem(str);
        }
    } else {
        museumList->clear();
    }

    descLabel->setText(tmp.at(1).c_str());
}

/**
 * @brief MainWindow::visitSelectedMuseum Slot for when user wants to visit museum selected on museumList
 */
void MainWindow::visitSelectedMuseum() {
    QListWidgetItem* item = museumList->currentItem();
    if(item == NULL)
        return;
    QString name = item->text();
    //Gathers data from database to find all data associated with a given unique museum ID
    curr = curr->getMuseumPageFromId(curr->getMuseumId(name.toStdString()));
    inputSearch->show();
    searchButton->show();
    if(currUser->getPrivelege() != "Viewer")
        newPage->show();
    visit->show();
    children->show();
    commentTb->show();
    curatorName->show();
    addComment->show();
    if(currUser->getPrivelege() == "Admin")
        removeComment->show();
    pic->show();
    descLabel->show();
    userList->hide();
    museumList->hide();
    goToUser->hide();
    goToMuseum->hide();
    kick->hide();
    genNewPIN->hide();
    newUn->hide();
    newDesc->hide();
    newPic->hide();
    submit->hide();
    lg->hide();
    submit1->hide();
    connect(accountManager, SIGNAL(clicked()), this, SLOT(accountManager_clicked()));
    accountManager->setText(currUser->getUn().c_str());
    //Loads the screen of the museum
    loadScreen(curr);
}

/**
 * @brief MainWindow::goBackToMuseum Slot for accountManager to go back to the prior visited museum
 * Basically readjusts widgets for a museum page
 */
void MainWindow::goBackToMuseum() {
    connect(accountManager, SIGNAL(clicked()), this, SLOT(accountManager_clicked()));
    accountManager->setText(currUser->getUn().c_str());
    inputSearch->show();
    searchButton->show();
    if(currUser->getPrivelege() != "Viewer")
        newPage->show();
    visit->show();
    children->show();
    commentTb->show();
    curatorName->show();
    addComment->show();
    if(currUser->getPrivelege() == "Admin")
        removeComment->show();
    pic->show();
    descLabel->show();
    userList->hide();
    museumList->hide();
    //Loads screen
    loadScreen(curr);
    goToUser->hide();
    goToMuseum->hide();
    kick->hide();
    genNewPIN->hide();
    newUn->hide();
    newDesc->hide();
    unL->hide();
    pwL->hide();
    newPic->hide();
    submit->hide();
    lg->hide();
    submit1->hide();
}

/**
 * @brief MainWindow::kick_clicked Slot for when the admin wants to remove a user from the system
 */
void MainWindow::kick_clicked() {
    QListWidgetItem* item = userList->currentItem();
    if(item == NULL)
        return;
    QString name = item->text();
    string n1 = name.toStdString();
    //This next line is added because don't want to delete admin account
    if(n1 == "Jack Black")
        return;
    //Changes a field in the appropriate cell of the Accounts table
    string fn = a->removeUser(n1);
    //Removes the entry in the QListWidget
    userList->takeItem(userList->currentRow());
}

/**
 * @brief MainWindow::search_clicked Slot for when search button is clicked. Search queries must exactly match any
 * museum page title or else it will tell user there are no results. All museum titles are unique so there are no
 * chances for errors/repeats
 */
void MainWindow::search_clicked() {
    QString searchString = inputSearch->text();
    string query = searchString.toStdString();
    if(searchString.isEmpty()) {
        QMessageBox::information(this, tr("Search"), tr("Enter a Museum Title"));
    } else {
        //Searches the MuseumPages table to see if the query is a legitamite museum title
        int currId = curr->getMuseumId(query);
        if(currId == 0) {
            currId = curr->getMuseumSearchId(query);
        }
        //Lets the user know if it is not legit and returns
        if(currId == 0) {
            QMessageBox::information(this, tr("Search"), tr("No Results Found"));
            return;
        }
        //Adds the names of all of the children of that museum to ids using the MuseumPages table
        vector<string> ids;
        query = curr->getMuseumPageFromId(currId)->getName();
        ids.push_back(query);
        for(int i = 0; i < (int) ids.size(); i++) {
            int w = curr->getMuseumId(ids.at(i));
            vector<string> x = curr->getMuseumChildrenNames(w);
            ids.insert(ids.end(), x.begin(), x.end());
        }
        bool ok;
        QStringList items;
        //This is for the QInputDialog to show all of the search results the algorithm came up with
        for(int j = 0; j < (int) ids.size(); j++) {
            items << tr(ids.at(j).c_str());
        }
        //Prompts user to select a search result. If they click ok, the associated museum page is loaded
        QString item = QInputDialog::getItem(this, tr("Search Results"), tr("Select a Museum"), items, 0, false, &ok);
        if(ok && !item.isEmpty()) {
            int currId = curr->getMuseumId(item.toStdString());
            curr = curr->getMuseumPageFromId(currId);
            loadScreen(curr);
        }
    }
}

/**
 * @brief MainWindow::addNewComment_clicked Slot for if addComment button is clicked
 */
void MainWindow::addNewComment_clicked() {
    //Popup window appears and user can submit text to be their comment
    bool ok;
    QString text = QInputDialog::getText(this, tr("Comment"),
                                         tr("Enter a Comment"),
                                         QLineEdit::Normal,
                                         "", &ok);
    //Adds the new comment to the Comments table as well as the QTableWidget
    if(ok && !text.isEmpty()) {
        string t = text.toStdString();
        c->newComment(curr->getId(), currUser->getUn(), t);
        QString s = t.c_str();
        QTableWidgetItem* w = new QTableWidgetItem(s);
        commentTb->setItem(entries, 0, w);
        s = currUser->getUn().c_str();
        w = new QTableWidgetItem(s);
        commentTb->setItem(entries, 1, w);
        commentTb->resizeRowsToContents();
        entries++;
    }
}

/**
 * @brief MainWindow::removeAComment Slot for the removal of a comment by admin
 */
void MainWindow::removeAComment() {
    QTableWidgetItem* item = commentTb->currentItem();
    if(item == NULL)
        return;
    int row = commentTb->currentRow();
    int col = commentTb->currentColumn();
    QString com = item->text();
    string n = com.toStdString();
    //Alters deleted field of the Comments table in database
    if(!c->removeComment(n)) {
        QMessageBox::information(this, tr("Delete Comment"), tr("Error: Author selected;Select comment to be deleted"));
    } else {
        commentTb->takeItem(row,col);
        if(col == 1) {
            commentTb->takeItem(row, col - 1);
        } else if(col == 0) {
            commentTb->takeItem(row, col + 1);
        }
    }
}

/**
 * @brief MainWindow::genPIN_clicked Slot for when the admin wants to generate a new authentication PIN
 */
void MainWindow::genPIN_clicked() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter a New PIN"),
                                         tr("PIN starts with C for Curator and V for Viewer"),
                                         QLineEdit::Normal,
                                         "", &ok);
    if(ok && !text.isEmpty()) {
        string t = text.toStdString();
        //Makes the adjustment in the database
        if(!a->newPIN(t))
            QMessageBox::information(this, tr("Enter a New PIN"), tr ("Invalid PIN format"));
    }
}

/**
 * @brief MainWindow::l1_clicked Slot for museum traversal button is
 * Communicates with the database to find the correct data needed to load the screen
 */
void MainWindow::l1_clicked() {
    QListWidgetItem* item = children->currentItem();
    if(item == NULL)
        return;
    QString name = item->text();
    string n1 = name.toStdString();
    int ID = curr->getMuseumId(n1);
    curr = curr->getMuseumPageFromId(ID);
    loadScreen(curr);
}

/**
 * @brief MainWindow::quit_clicked Logs out the user and frees all variables with allocated memory. Loads login screen
 */
void MainWindow::quit_clicked() {
    free(a);
    free(c);
    free(curr);
    loginScreen();
}
