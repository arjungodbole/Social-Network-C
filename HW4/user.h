#ifndef USER_H
#define USER_H

#include <string>
#include <set>

class User
{

public:
    // pre: none
    // post:Initialize values to arbitrary values
    User();

    // pre: arguments provided
    // post:creates a user
    User(int id, std::string name, int year, int zip, std::set<int> friends);

    // pre: none
    // post: Adds a user as a friend
    void addFriend(int id);

    // pre: none
    // post: Deletes a user as a friend
    void deleteFriend(int id);

    // pre: none
    // post: returns id_
    int getId() const;

    // pre: none
    // post: returns name_
    std::string getName() const;

    // pre: none
    // post: returns year_
    int getYear() const;

    // pre: none
    // post: returns zip_
    int getZip() const;

    // pre: none
    // post: returns friends
    std::set<int> &getFriends();             // non-const ref (allows insert/erase)
    const std::set<int> &getFriends() const; // const ref (read-only)

private:
    int id_;
    std::string name_;
    int year_;
    int zip_;
    std::set<int> friends_;
};
#endif