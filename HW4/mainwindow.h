#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //pre: none
    //post: lets you click the button
    void myButtonClick();

    //pre:none
    //post: show the display of each ui
    void display();
    //pre: none
    //post: show your friends
    void showFriends();

    //pre: none
    //post: shows the 5 most recent posts
    void showPosts();

    //pre: none
    //post: show the friend suggestion table
    void onFriend(int row,int column);

    //pre: none
    //post: the back to profile button works
    void BackToProfile();

    //pre:
    //post: shows the suggestion table with all of the friend suggestions
    void suggestionsTable(int row, int column);

    //pre: none
    //post: allows you to add friends in the suggestions table
    void addFriend();

    //pre: none
    //post: allows you to add a post
    void addPost();

    //pre: none
    //post: shows friends suggestions
    void friendSuggestions();

private:
    Ui::MainWindow *ui;
    //stores all of the users a connections posts
    Network *network;
    //who the logged in user is
    User* LoggedIn;
    //who the user you are viewing
    User* lookingAt;
};
#endif // MAINWINDOW_H
