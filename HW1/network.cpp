#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "user.h"
#include "network.h"

Network::Network()
{
}

// Return the User* at id if in range; otherwise nullptr.
User *Network::getUser(int id)
{
    if (id >= 0 && id < users_.size())
    {
        return users_[id];
    }

    return nullptr;
}

// Add a user pointer if its id matches the next index (enforces users_[i]->getId() == i).
void Network::addUser(User *user)
{
    int Id = user->getId();
    if (Id == users_.size())
    {
        users_.push_back(user);
    }
}

// Add an undirected friend edge by names; return -1 on invalid, 0 on success.
int Network::addConnection(std::string s1, std::string s2)
{
    int id1 = getId(s1);
    int id2 = getId(s2);

    if (id1 == -1 || id2 == -1)
    {
        return -1;
    }

    User *u1 = getUser(id1);
    User *u2 = getUser(id2);

    u1->addFriend(id2);
    u2->addFriend(id1);

    return 0;
}

// Delete an undirected friend edge by names; return -1 on invalid, 0 on success.
int Network::deleteConnection(std::string s1, std::string s2)
{
    int id1 = getId(s1);
    int id2 = getId(s2);

    if (id1 == -1 || id2 == -1)
    {
        return -1;
    }

    User *u1 = getUser(id1);
    User *u2 = getUser(id2);

    u1->deleteFriend(id2);
    u2->deleteFriend(id1);

    return 0;
}

// Search by full name ("First Last"); returns id or -1.
int Network::getId(std::string name)
{
    for (int i = 0; i < users_.size(); i++)
    {
        if (users_[i]->getName() == name)
        {
            return users_[i]->getId();
        }
    }
    return -1;
}

int Network::numUsers()
{
    return users_.size();
}

// Load the network from a file in the specified format (two-pass to add edges).
void Network::readUsers(const char *fname)
{
    for (User *u : users_)
    {
        delete u;
    }
    users_.clear();

    std::ifstream in(fname);
    if (!in)
    {
        return; // silently fail per common course conventions
    }

    auto strip_tab = [](std::string &s)
    {
        if (!s.empty() && s[0] == '\t')
            s.erase(0, 1);
    };

    std::string line;

    // Line 1: number of users
    if (!std::getline(in, line))
        return;
    std::stringstream ssn(line);
    int n = 0;
    ssn >> n;
    if (n < 0)
        return;

    // We'll store pending friend lists to add edges in a second pass
    std::vector<std::vector<int>> pending(n);

    for (int i = 0; i < n; ++i)
    {
        // id line
        if (!std::getline(in, line))
            return;
        std::stringstream ssid(line);
        int id = -1;
        ssid >> id;
        if (id != i)
            return; // enforce ordered, contiguous ids

        // name line
        if (!std::getline(in, line))
            return;
        strip_tab(line);
        std::string name = line; // full name (may contain spaces)

        // year line
        if (!std::getline(in, line))
            return;
        strip_tab(line);
        std::stringstream ssy(line);
        int year = 0;
        ssy >> year;

        // zip line
        if (!std::getline(in, line))
            return;
        strip_tab(line);
        std::stringstream ssz(line);
        int zip = 0;
        ssz >> zip;

        // friends line
        if (!std::getline(in, line))
            return;
        strip_tab(line);
        std::stringstream ssf(line);
        int fid;
        while (ssf >> fid)
        {
            pending[i].push_back(fid);
        }

        // Create user with empty friends first; add later
        std::set<int> emptyFriends;
        User *u = new User(id, name, year, zip, emptyFriends);
        addUser(u); // enforces id == users_.size() - 1
    }

    // Second pass: add undirected edges
    for (int i = 0; i < n; ++i)
    {
        for (int fid : pending[i])
        {
            if (fid < 0 || fid >= n)
                continue;
            if (i < fid)
            { // add each edge once
                User *ui = getUser(i);
                User *uf = getUser(fid);
                if (ui && uf)
                {
                    ui->addFriend(fid);
                    uf->addFriend(i);
                }
            }
        }
    }
}

// Write the network to a file in the required format (tabs, order, space-separated IDs).
void Network::writeUsers(const char *fname)
{
    std::ofstream out(fname);
    if (!out)
    {
        return;
    }

    int n = numUsers();
    out << n << "\n";

    for (int i = 0; i < n; ++i)
    {
        User *u = users_[i];
        out << i << "\n";
        out << "\t" << u->getName() << "\n";
        out << "\t" << u->getYear() << "\n";
        out << "\t" << u->getZip() << "\n";

        out << "\t";
        const std::set<int> &fr = u->getFriends();
        bool first = true;
        for (int fid : fr)
        {
            if (!first)
                out << ' ';
            out << fid;
            first = false;
        }
        out << "\n";
    }
}
