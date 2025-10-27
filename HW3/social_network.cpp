// social_network.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <fstream> // only used to sanity-check writability
#include "user.h"
#include "network.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << (argc > 0 ? argv[0] : "program")
             << " <users.txt>\n";
        return 1;
    }

    const char *inputFile = argv[1];
    const char *postsFile = argv[2];

    Network net;
    net.readUsers(inputFile);
    net.readPosts((char *)postsFile);

    // Menu loop
    while (true)
    {
        cout
            << "\nMenu:\n"
            << " 1 <First Last> <Year> <Zip>      : Add user\n"
            << " 2 <First Last> <First Last>      : Add friend connection\n"
            << " 3 <First Last> <First Last>      : Delete friend connection\n"
            << " 4 <filename>                     : Write to file\n"
            << " 5 <First name> <Last name>       : View Recent posts\n "
            << " Any other number                 : Exit\n"
            << "Enter option and arguments on one line:\n";

        string line;
        if (!getline(cin, line))
        {
            // EOF or input error -> exit
            break;
        }
        if (line.empty())
            continue;

        istringstream iss(line);
        int option;
        if (!(iss >> option))
        {
            // Not a number -> exit (per spec: ANY invalid command number exits)
            break;
        }

        if (option == 1)
        {
            // 1 First Last Year Zip
            string first, last;
            int year, zip;
            if (!(iss >> first >> last >> year >> zip))
            {
                cerr << "Invalid input for option 1. Example: 1 Jason Chen 2001 95053\n";
                continue;
            }
            string name = first + " " + last;
            int newId = net.numUsers();
            set<int> empty;
            User *u = new User(newId, name, year, zip, empty);
            net.addUser(u);
            cout << "Added user: " << name << " with id " << newId << "\n";
        }
        else if (option == 2)
        {
            // 2 First1 Last1 First2 Last2
            string f1, l1, f2, l2;
            if (!(iss >> f1 >> l1 >> f2 >> l2))
            {
                cerr << "Invalid input for option 2. Example: 2 Aled Montes Sandhya Krish\n";
                continue;
            }
            string name1 = f1 + " " + l1;
            string name2 = f2 + " " + l2;

            int rc = net.addConnection(name1, name2);
            if (rc == -1)
            {
                cerr << "Error: one or both users do not exist (\"" << name1 << "\", \"" << name2 << "\").\n";
            }
            else
            {
                cout << "Connected: " << name1 << " <-> " << name2 << "\n";
            }
        }
        else if (option == 3)
        {
            // 3 First1 Last1 First2 Last2
            string f1, l1, f2, l2;
            if (!(iss >> f1 >> l1 >> f2 >> l2))
            {
                cerr << "Invalid input for option 3. Example: 3 Aled Montes Leo Griffin\n";
                continue;
            }
            string name1 = f1 + " " + l1;
            string name2 = f2 + " " + l2;

            // Per prompt: print error if users not found OR users are not friends
            int id1 = net.getId(name1);
            int id2 = net.getId(name2);
            if (id1 == -1 || id2 == -1)
            {
                cerr << "Error: one or both users do not exist (\"" << name1 << "\", \"" << name2 << "\").\n";
                continue;
            }
            User *u1 = net.getUser(id1);
            User *u2 = net.getUser(id2);
            if (!u1 || !u2)
            {
                cerr << "Error: internal lookup failed.\n";
                continue;
            }
            const set<int> &fset1 = u1->getFriends();
            if (fset1.find(id2) == fset1.end())
            {
                cerr << "Error: users are not friends; nothing to delete.\n";
                continue;
            }

            int rc = net.deleteConnection(name1, name2);
            if (rc == -1)
            {
                cerr << "Error deleting connection.\n";
            }
            else
            {
                cout << "Deleted connection: " << name1 << " X " << name2 << "\n";
            }
        }
        else if (option == 4)
        {
            // 4 filename
            string outName;
            if (!(iss >> outName))
            {
                cerr << "Invalid input for option 4. Example: 4 users_new.txt\n";
                continue;
            }

            // Quick sanity check: can we open for writing?
            {
                ofstream test(outName.c_str());
                if (!test)
                {
                    cerr << "Error: cannot open \"" << outName << "\" for writing.\n";
                    continue;
                }
                // close immediately; Network::writeUsers will write the actual data
            }

            net.writeUsers(outName.c_str());
            cout << "Wrote " << net.numUsers() << " users to \"" << outName << "\"\n";
        }
        else if (option == 5)
        {
            string firstName;
            string lastName;
            int howMany;
            if (!(iss >> firstName >> lastName >> howMany))
            {
                cerr << "Invalid input for option 5. Example: 5 Aled Montes 4" << endl;
                continue;
            }

            string name = firstName + " " + lastName;
            int Id = net.getId(name);

            if (Id == -1)
            {
                cerr << "User doesnt exist" << endl;
                continue;
            }

            string posts = net.getPostsString(Id, howMany);
            cout << posts << endl;
        }
        else
        {
            // Any other command number => exit (per spec)
            break;
        }
    }

    return 0;
}
