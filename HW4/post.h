#ifndef POST_H
#define POST_H

#include "user.h"
#include "network.h"

class Post
{

public:
    // pre: none
    // post: creates a Post object with default values
    Post();

    // pre: arguments
    // post: creates a Post object with values
    Post(int profileId, int authorId, std::string message, int likes);

    // pre: none
    // post: return messageId_
    int getMessageId();

    // pre: needs a int of messageId
    // post: sets messageId_
    void setMessageId(int messageId);

    // pre: none
    // post: return profileId_
    int getProfileId();

    // pre: none
    // post: return authorId_
    int getAuthorId();

    // pre: none
    // post: return message_
    std::string getMessage();

    // pre: none
    // post: return likes
    int getLikes();

    // pre: none
    // post: return an empty string
    std::string virtual getURL();

    // pre: none
    // post: message and likes are replaced by their values
    std::string virtual toString();

private:
    int messageId_;
    int profileId_;
    int authorId_;
    int likes_;
    std::string message_;
};

class LinkPost : public Post
{
public:
    // pre: none
    // post: creates a LinkPost object with default values
    LinkPost();
    // pre: args
    // post: creates a LinkPost object with values
    LinkPost(int profileId, int authorId, std::string message, int likes, std::string url);

    // pre: none
    // post: return url_
    std::string getURL();

    // pre: none
    // post: message, likes, and url are replaced by their values
    std::string toString();

private:
    std::string url_;
};

#endif