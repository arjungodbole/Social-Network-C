#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QWidget>
#include "network.h"
#include "user.h"

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWindow(User* user,User *user2,Network *net, QWidget *parent = nullptr);
    ~ProfileWindow();

    //pre: none
    //post: lets you edit the bio
    void onbioEditClicked();

    //pre: none
    //post: lets you save the bio
    void onSaveBioClicked();

    //pre: none
    //post: user should be allowed to go back to the main window
    void onBackClicked();

private:
    Ui::ProfileWindow *ui;
    User* currentUser;
    User* LoggedIn;
    Network* network;
};

#endif // PROFILEWINDOW_H
