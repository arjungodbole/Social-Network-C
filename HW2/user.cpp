#include <string>
#include <set>
#include "user.h"

// Default constructor
User::User()
{
    id_ = 0;
    name_ = "";
    year_ = 1934;
    zip_ = 89591;
}

// Primary constructor
User::User(int id, std::string name, int year, int zip, std::set<int> friends)
{
    id_ = id;
    name_ = name;
    year_ = year;
    zip_ = zip;
    friends_ = friends;
}

// Accessors
int User::getId() const
{
    return id_;
}

std::string User::getName() const
{
    return name_;
}

int User::getYear() const
{
    return year_;
}

int User::getZip() const
{
    return zip_;
}
// Friends accessors
// Non-const overload allows mutation via reference
std::set<int> &User::getFriends()
{
    return friends_;
}

// Const overload for read-only access on const User objects
const std::set<int> &User::getFriends() const
{
    return friends_;
}

// Friend mutations
void User::addFriend(int id)
{
    // doesn't add if there is a duplicate
    friends_.insert(id);
}

void User::deleteFriend(int id)
{
    // doesnt erase if id isnt present
    friends_.erase(id);
}
