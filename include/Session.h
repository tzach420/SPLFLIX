#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{//this is the main class of the program.
public:
    Session(const std::string &configFilePath);//constructor.
    Session(const Session & other);//copy constructor
    ~Session();//destructor
    Session& operator=(const Session& other);//copy Assignment operator
    Session(Session&& other);//move constructor
    Session& operator=(Session &&other);//move assignment operator.
    void clearPointersObjects(); //move pointers to null
    void copy(const Session & other); //deep copy
    void start();//main loop
    void updateUserMap(pair<string,User*>);// insert new user to map
    void clear(); // deep delete, help to destructor
    bool changeActiveUser(string name);  //return true if change user succeed
    bool deleteUser(string name);
    bool dupUser(string originName,string newName); // duplicate user
    bool printContentList() const;
    const  std::vector<Watchable*> & getContent() const;//return content list by reference.
    User & getActiveUser() const;//return active user by reference
    std::vector<BaseAction*> & getActionLog(); //get action log by ref
    unordered_map<string, User *> getUserMap() const; //
    vector<Watchable*> buildDataBase(const string& path);//build content vector from json file
    vector<string> getCommand(string s, string delimiter);
private:
    std::vector<Watchable*> content;//available watching content (read the json text to this vector).
    std::vector<BaseAction*> actionsLog;//history of all actions.
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;//currently active user (only 1 user can be active at the same time).
    std::string path;
};
#endif
