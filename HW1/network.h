#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <set>
#include <vector>
#include "user.h"

class Network
{
public:
    // pre: none
    // post: Initialize empty network
    Network();

    // pre: none
    // post: returns pointer to corresponding User
    User *getUser(int id);

    // pre: none
    // post: adds user to the database
    void addUser(User *);

    // pre: none
    // post: adds a friend connection
    int addConnection(std::string s1, std::string s2);

    // pre: none
    // post: deletes the friend connection
    int deleteConnection(std::string s1, std::string s2);

    // pre: none
    // post: returns the id of the user
    int getId(std::string name);

    // pre: none
    // post: returns the number of users
    int numUsers();

    // pre: none
    // post: initializes all of the network’s information from a file.
    void readUsers(const char *fname);

    // pre: none
    // post: hat writes all of the network’s information to a file
    void writeUsers(const char *fname);

private:
    std::vector<User *> users_;
};
#endif