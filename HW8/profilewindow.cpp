#include "profilewindow.h"
#include "ui_profilewindow.h"
#include "user.h"

ProfileWindow::ProfileWindow(User *user,User* user2,Network* net, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfileWindow)
    , currentUser(user)
    , LoggedIn(user2)
    , network(net)
{
    ui->setupUi(this);
    ui->bio->setText(QString::fromStdString(user->getBio()));
    ui->bioEdit->hide();
    ui->saveBio->hide();

    if(currentUser->getId() == LoggedIn->getId()){
        ui->editBio->show();
    } else {
        ui->editBio->hide();
    }
    //all of the different connect buttons that are needed
    connect(ui->editBio,
            &QPushButton::clicked,
            this,
            &ProfileWindow::onbioEditClicked);

    connect(ui->saveBio,
            &QPushButton::clicked,
            this,
            &ProfileWindow::onSaveBioClicked);

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            &ProfileWindow::onBackClicked);
}

void ProfileWindow::onbioEditClicked(){
    if(currentUser->getId() != LoggedIn->getId()){
        return;
    }
    ui->bioEdit->setText(QString::fromStdString(currentUser->getBio()));
    ui->bioEdit->show();
    ui->saveBio->show();
    ui->editBio->hide();
    ui->bio->hide();

}

void ProfileWindow::onSaveBioClicked(){
    if(currentUser->getId() != LoggedIn->getId()){
        return;
    }
    QString bio = ui->bioEdit->text();
    currentUser->setBio(bio.toStdString());


    ui->bio->setText(bio);
    ui->bioEdit->hide();
    ui->saveBio->hide();
    ui->editBio->show();
    ui->bio->show();
}

void ProfileWindow::onBackClicked(){
    this->close();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}
