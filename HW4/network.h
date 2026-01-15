#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <set>
#include <vector>
#include "user.h"
#include "post.h"

class Post;

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

    // pre: none
    // post: return a vector containing a shortest path of vertices
    std::vector<int> shortestPath(int from, int to);

    // pre: none
    // post: finds a user whose relational distance from the given user exactly matches the distance given
    std::vector<int> distanceUser(int from, int &to, int distance);

    // pre: none
    // post: suggest users with new friends
    std::vector<int> suggestFriends(int who, int &score);

    // pre: none
    // post: connects people into groups
    std::vector<std::vector<int> > groups();

    // pre: parameters needed for the post
    // post: adds a post
    void addPost(Post *post);

    // pre:id of user
    // post: returns post for user with the id
    std::vector<Post *> getPosts(int id);

    // pre: parameters needed for the post
    // post: returns a string for the post
    std::string postDisplayString(Post *post);

    // pre: the profile id and how many posts
    // post: returns a string that holds the most recent howMany posts for user
    std::string getPostsString(int profileId, int howMany);

    // pre: file
    // post: read the posts from a file
    int readPosts(char *fname);

    // pre: file
    // post: write the posts to a file
    int writePosts(char *fname);

private:
    std::vector<User *> users_;
    std::vector<std::vector<Post *> > posts_;
};
#endif