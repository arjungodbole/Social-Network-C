#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->invalid->hide();

    network = new Network();
    //load in all of the users
    network->readUsers("users.txt");
    qDebug() << "Loaded users:" << network->numUsers();

    //had to implement it this way for post because readPosts takes in char* not a const char*
    char posts[] = "posts.txt";
    network->readPosts(posts);

    //set users to null b/c no one is logged in
    LoggedIn = nullptr;
    lookingAt = nullptr;

    display();

    //all of the button click functions
    connect(ui->LoginButton,
            &QPushButton::clicked,
            this,
            &MainWindow::myButtonClick);

    connect(ui->friends, &QTableWidget::cellClicked,
            this, &MainWindow::onFriend);

    connect(ui->profileBack, &QPushButton::clicked,
            this, &MainWindow::BackToProfile);

    connect(ui->suggestions, &QTableWidget::cellClicked, this, &MainWindow::suggestionsTable);
    connect(ui->addFriend,&QPushButton::clicked,this, &MainWindow::addFriend);
    connect(ui->addPost, &QPushButton::clicked, this, &MainWindow::addPost);
    connect(ui->viewProfile, &QPushButton::clicked, this, &MainWindow::onProfileClicked);

}

void MainWindow::myButtonClick(){
    //could only use QString and found online the toPlainText function
    QString user = ui ->loginName->toPlainText();
    int userId = network->getId(user.toStdString());
    //check to see if the userid is valid
    if(userId == -1){
        ui->invalid->show();
        ui->loginName->clear();
        return;
    }
    //hide the error if the valid user is found
    ui->invalid->hide();

    //Get the user object and set them to be logged in
    User *user1 = network->getUser(userId);
    LoggedIn = user1;
    lookingAt = user1;

    display();


}

void MainWindow::display(){
    if(LoggedIn == nullptr){
        //not logged in
        ui->loginName->show();
        ui->LoginButton->show();
        ui->nameEnter->show();
        ui->invalid->hide();
        ui->Profile->hide();
        ui->fivePosts->hide();
        ui->profileBack->hide();
        ui->friends->hide();
        ui->suggestions->hide();
        ui->addFriend->hide();
        ui->addPost->hide();
        ui->post->hide();
        ui->bio->hide();
        ui->viewProfile->hide();
    } else {
        //logged in
        ui->loginName->hide();
        ui->LoginButton->hide();
        ui->nameEnter->hide();
        ui->invalid->hide();
        ui->Profile->show();
        ui->fivePosts->show();
        ui->friends->show();
        ui->addPost->show();
        ui->post->show();
        ui->viewProfile->show();

        displayProfile();

        if(lookingAt == LoggedIn){
            //show users profile
            ui->Profile->setText("My Profile");
            ui->profileBack->hide();
            ui->addFriend->hide();
            ui->suggestions->show();
            friendSuggestions();
        } else {
            //show someone elses profile
            ui->Profile->setText(QString::fromStdString(lookingAt->getName()) + "'s Profile");
            ui->profileBack->show();
            ui->addFriend->show();
            ui->suggestions->hide();
        }

        showFriends();
        showPosts();
    }
}

void MainWindow::showFriends(){
    //reset the friends table
    ui->friends->clear();
    ui->friends->setRowCount(0);
    ui->friends->setColumnCount(1);
    //get the friends id
    std::set<int> Ids = lookingAt->getFriends();
    ui->friends->setRowCount(Ids.size());

    //insert the friends into the table
    int row = 0;
    for(int friendId : Ids){
        User * newFriend = network->getUser(friendId);
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(newFriend->getName()));

        ui->friends->setItem(row,0,item);
        row++;
    }

}

//shows 5 posts
void MainWindow::showPosts(){
    ui->fivePosts->setText(QString::fromStdString(network->getPostsString(lookingAt->getId(), 5)));
}

//allows clicking on a friends name in the friends table
void MainWindow::onFriend(int row, int column){
    //get the friends name on the clicked cell
    QString friendName = ui->friends->item(row,column)->text();
    //look at friends id in the user object
    int id = network->getId(friendName.toStdString());
    lookingAt = network->getUser(id);
    display();
}

//allows the back to profile button to work
void MainWindow::BackToProfile(){
    lookingAt = LoggedIn;
    display();
}

void MainWindow::friendSuggestions(){
    //clear the suggestions table
    ui->suggestions->clear();
    ui->suggestions->setRowCount(0);
    ui->suggestions->setColumnCount(1);

    //get friend suggestions
    int count = 0;
    std::vector<int> suggestions = network->suggestFriends(LoggedIn->getId(), count);

    ui->suggestions->setRowCount(suggestions.size());
    //fill the table with suggested friends
    int row = 0;
    for(int id : suggestions){
        User *suggest = network->getUser(id);
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(suggest->getName()));
        ui->suggestions->setItem(row,0,item);
        row++;
    }
}

//allows you to click on the suggested friend and add them
void MainWindow::suggestionsTable(int row, int column){
    QString friendName = ui->suggestions->item(row,column)->text();
    //adds this to users.txt
    network->addConnection(LoggedIn->getName(),friendName.toStdString());
    network->writeUsers("users.txt");
    display();
}

//Allows you to add a friend when you are viewing their profile
void MainWindow::addFriend(){
    network->addConnection(LoggedIn->getName(), lookingAt->getName());
    //adds this to users.txt
    network->writeUsers("users.txt");
    display();
}

void MainWindow::addPost(){
    //gets the post text from the text box
    QString postText = ui->post->toPlainText();
    //checks to see if the post is empty
    if(postText.isEmpty()){
        return;
    }
    //creates a new post object
    Post *newP = new Post(LoggedIn->getId(),lookingAt->getId(), postText.toStdString(),false);

    //add posts to the network
    network->addPost(newP);

    char fname[] = "posts.txt";
    network->writePosts(fname);
    //clears the textbox
    ui->post->clear();
    display();

}

void MainWindow::displayProfile(){
    if(lookingAt != nullptr){
        QString path = QString::fromStdString(lookingAt->getProfilePic());
        QPixmap profilePic(path);

        if(!profilePic.isNull()){
            ui->profilePictureLabel->setPixmap(profilePic);
        }

        QString bio1 = QString::fromStdString(lookingAt->getBio());
        ui->bio->setText(bio1);
        ui->bio->show();
    }
}

void MainWindow::onProfileClicked(){
    if(lookingAt != nullptr){
        ProfileWindow* profile = new ProfileWindow(lookingAt,LoggedIn,network, nullptr);
        //found the WA_DeleteOnClose online , what it does is it automatically deletes the profile window widget
        profile->setAttribute(Qt::WA_DeleteOnClose);
        //found the destroyed online, when you exit out of the profile window it changes the mainwindow
        connect(profile, &QWidget::destroyed, this, &MainWindow::displayProfile);
        profile->show();
    }
}

MainWindow::~MainWindow()
{
    delete network;
    delete ui;
}
