#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
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

std::vector<int> Network::shortestPath(int from, int to)
{
    // Simple base case check to see if they are the same verticies
    if (from == to)
    {
        return {from};
    }
    // BFS Implementation
    std::queue<int> q;
    std::vector<int> dist(users_.size(), -1);
    std::vector<int> prev(users_.size(), -1);
    std::vector<bool> visited(users_.size(), 0);

    dist[from] = 0;
    visited[from] = true;
    q.push(from);

    while (q.size() > 0)
    {
        int cur = q.front();
        q.pop();

        for (int neighbor : users_[cur]->getFriends())
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                prev[neighbor] = cur;
                dist[neighbor] = 1 + dist[cur];
                q.push(neighbor);
            }
        }
    }
    // Implementation to get the actual shortest path
    std::vector<int> shortestPath;
    // Check to see if this is true
    if (!visited[to])
    {
        return shortestPath;
    }
    // Left the second condition blank because you need to take a step back after each iteration
    for (int i = to;; i = prev[i])
    {
        shortestPath.push_back(i);
        // If i reaches the end then stop
        if (i == from)
        {
            break;
        }
        // If i is negative one then it doesn't have a path
        if (i == -1)
        {
            break;
        }
    }
    std::reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

std::vector<int> Network::distanceUser(int from, int &to, int distance)
{
    // simple BFS implementation with shortespath
    std::queue<int> q;
    std::vector<int> dist(users_.size(), -1);
    std::vector<bool> visited(users_.size(), 0);

    visited[from] = true;
    dist[from] = 0;
    q.push(from);
    while (q.size() > 0)
    {
        int n = q.front();
        q.pop();

        // check to see if the distance is equal to the distance we are looking for
        if (dist[n] == distance)
        {
            to = n;
            // return the function we just implemented
            return shortestPath(from, to);
        }

        for (int j : users_[n]->getFriends())
        {
            if (!visited[j])
            {
                visited[j] = true;
                dist[j] = dist[n] + 1;
                q.push(j);
            }
        }
    }
    // if there is no user at that distance return -1
    to = -1;
    return {};
}

std::vector<int> Network::suggestFriends(int who, int &score)
{
    std::vector<int> suggestions;
    std::vector<int> mutualFriends(users_.size(), 0);

    score = 0;

    int n = users_.size();
    // set to store whos friends, doesn't work with a vector
    std::set<int> whosFriends = users_[who]->getFriends();
    for (int i = 0; i < n; i++)
    {
        // checks to user id isn't already a friend of who, and also checks if i isnt who
        if (i != who && !whosFriends.count(users_[i]->getId()))
        {
            int mutualFriends = 0;
            std::set<int> totalFriends = users_[i]->getFriends();

            for (int Id : whosFriends)
            {
                if (totalFriends.count(Id))
                {
                    mutualFriends++;
                }
            }
            // check to see if it is higher than the actual score
            if (mutualFriends > score)
            {
                // set score to the new amount
                score = mutualFriends;
                // remove all users with lower scores
                suggestions.clear();
                suggestions.push_back(i);
            }
            // push backs when the score is the same
            else if (mutualFriends == score && mutualFriends > 0)
            {
                suggestions.push_back(i);
            }
        }
    }
    return suggestions;
}

std::vector<std::vector<int>> Network::groups()
{

    std::vector<std::vector<int>> connected;
    int n = users_.size();
    // Check and see the size
    if (n == 0)
    {
        return connected;
    }

    // Track the visited users
    std::vector<bool> visited(n, 0);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            std::stack<int> stck;
            // This vector stores all of the users
            std::vector<int> component;
            stck.push(i);

            // DFS Implementation
            while (stck.size() > 0)
            {
                int cur = stck.top();
                stck.pop();

                if (!visited[cur])
                {
                    visited[cur] = true;
                    component.push_back(cur);

                    // Add all unvisited neighbors to the stack
                    for (int neighbor : users_[cur]->getFriends())
                    {
                        if (!visited[neighbor])
                        {
                            stck.push(neighbor);
                        }
                    }
                }
            }
            connected.push_back(component);
        }
    }
    return connected;
}