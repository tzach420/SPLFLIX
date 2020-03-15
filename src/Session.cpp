//
// Created by tzachu@wincs.cs.bgu.ac.il on 22/11/2019.
//
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include <iostream>
#include "../include/json.hpp"
#include <fstream>
using json = nlohmann::json;


///////constructors///////
Session::Session(const std::string &configFilePath): content(), actionsLog(), userMap(), activeUser(), path(configFilePath){
   content=Session::buildDataBase(path);
   activeUser = new LengthRecommenderUser("default");
   userMap.insert({"default",activeUser});

}
Session::Session(const Session & other):content(), actionsLog(), userMap(),activeUser(),path("") {//copy constructor, deep copy
    this->copy(other);
}
Session& Session::operator=(const Session& other) {//copy Assignment operator
    if(this != &other){
        clear(); //deep delete
        this->copy(other);
    }
    return *this;
}
Session::Session(Session&& other):content(other.content),actionsLog(other.actionsLog),userMap(other.userMap),activeUser(other.activeUser),path(){//move constructor
    other.clearPointersObjects();
}
Session& Session::operator=(Session &&other){//move assignment operator.
    if(this!=&other){
        clear();
        content=other.content;
        actionsLog=other.actionsLog;
        userMap=other.userMap;
        activeUser=other.activeUser;
        other.clearPointersObjects();
    }
    return *this;
}
Session::~Session() {//destructor
    clear();
}
void Session::copy(const Session & other){
    for(int unsigned i=0;i<other.content.size();i++)
        this->content.push_back(other.content[i]->clone()); //deep copy of content
    for(int unsigned i=0;i<other.actionsLog.size();i++)
        this->actionsLog.push_back(other.actionsLog[i]->clone());//deep copy for action log
    for(auto &it : other.userMap) { //loop threw all the users
        int i=0;
        this->userMap.insert({it.first, it.second->clone()}); //deep copy of users.
        for(auto & hist: it.second->getHistoryByRef()) { //change all watchable of history per user, to point to the new content, very important!!!
            userMap.find(it.first)->second->getHistoryByRef()[i] = (content[hist->getId()]);
            i++;
        }
    }
    string s=other.activeUser->getName();
    activeUser=userMap.find(s)->second; //s is the name of current active user
}
void Session::start(){
    string s1;
    bool flag=true;
    cout<<"SPLFLIX is now on!"<<endl;
    while (flag) { //flag change during exit command
        getline (std::cin,s1);
        vector<string> input = Session::getCommand(s1, " ");

        ////////check type of action and act/////
        if(input[0]=="createuser"){
            BaseAction *execute=new CreateUser(input[1],input[2]);
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="changeuser"){
            BaseAction *execute=new ChangeActiveUser(input[1]);
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="deleteuser"){
            BaseAction *execute=new DeleteUser(input[1]);
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="dupuser"){
            BaseAction *execute=new DuplicateUser(input[1],input[2]);
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="content"){
            BaseAction *execute=new PrintContentList();
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="watchhist"){
            BaseAction *execute=new PrintWatchHistory();
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="watch"){
            BaseAction *execute=new Watch(stoi(input[1]));
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="log"){
            BaseAction *execute=new PrintActionsLog();
            execute->act(*this);
            actionsLog.push_back(execute);
        }
        else if(input[0]=="exit"){
            BaseAction *execute=new Exit();
            execute->act(*this);
            actionsLog.push_back(execute);
            flag=false;
        }
    }
}
void Session::clear() { //deep delete
    for (int unsigned i = 0; i < content.size(); i++)
        delete content[i];
    for (int unsigned i = 0; i < actionsLog.size(); i++)
        delete actionsLog[i];
    for (auto &it: userMap)
        delete it.second;
    activeUser = nullptr;
}
void Session::clearPointersObjects(){ //change pointers to null
    for(unsigned int i =0;i<content.size();i++)
        content[i]= nullptr;
    for(unsigned int i =0;i<actionsLog.size();i++)
        actionsLog[i]= nullptr;
    for(auto &x: userMap)
        x.second= nullptr;
    activeUser=nullptr;
}
void Session::updateUserMap(pair<string,User*> p){
    userMap.insert(p);//insert user p to the map.
}
bool Session::changeActiveUser(string name){  //return true if change user succeed
    if (userMap.count(name)==0)  return false;//if there is no user under that name.
    else activeUser=userMap.find(name)->second;
    return true;
}
bool Session::deleteUser(string name){
    if(userMap.count(name)==0) return false;//if there is no user under that name.
    delete userMap.find(name)->second;
    userMap.erase(name);
    return true;

}
bool Session::dupUser(string originName,string newName){
    if(userMap.count(originName)==0||userMap.count(newName)>0) return false;//if there is no user with the name "originname" or there is already a user with the name "newname".
    User *toInsert=userMap.find(originName)->second->clone();//deep copy
    toInsert->setName(newName);//change the name of the copy to the new name.
    userMap.insert({newName,toInsert});//insert the new user to the map.
    return true;
}
bool Session::printContentList() const{ //prints all the content.
    int j=1;
    for (auto i = this->content.begin(); i != this->content.end(); ++i) {
        cout << to_string(j) + ". " + (*i)->fullToString() << endl;
        j++;
    }

    return true;
}
const  std::vector<Watchable*> & Session::getContent() const{
    return content;
}
User & Session::getActiveUser()  const{//check how to return const- we have problem in getNextWatchable in watchable class.
    return *activeUser;
}
unordered_map<string, User *> Session::getUserMap() const{
    return userMap;
}
std::vector<BaseAction*> & Session::getActionLog(){
    return  actionsLog;
}

/////////////////////////////////////ADDITIONAL FUNCTIONS//////////////////////////////////////////


vector<Watchable*> Session::buildDataBase(const string& path) {//create the vector content from the json file.
    ifstream i(path);
    if(!i){//in case the file is failed to load.
        std::cout <<"Failed to open Json file. Please check the file path.";
    }
    json j;
    i>>j;
    i.close();
    vector<Watchable*> content;
    ///finish reading file here
    long index = 0;
    for (auto it:j["movies"]) {//insert all the movies in the json file to content.
        string movie_name = it["name"].get<std::string>();
        int movie_length = it["length"];
        vector<string> tags;
        for (auto tag:it["tags"])
            tags.push_back(tag.get<std::string>());
        Watchable *movie = new Movie(index, movie_name, movie_length, tags);//check if we need to free this.
        index++;
        content.push_back(movie);
    }
    for (auto it:j["tv_series"]) {//insert all the tv_series in the json file to content.
        string series_name = it["name"].get<std::string>();
        int series_length = it["episode_length"];
        vector<string> tags;
        for (auto tag:it["tags"])
            tags.push_back(tag.get<std::string>());
        int season_number = 0;
        for (auto season:it["seasons"]) {
            season_number++;
            int episode_number = 1;
            int season_episode_number =season.get<std::int8_t>();
            while (season_episode_number != 0) {
                Watchable *episode = new Episode(index, series_name, series_length, season_number, episode_number,
                                                 tags);
                index++;
                episode->setNextId(index);
                content.push_back(episode);
                episode_number++;
                season_episode_number--;
            }
        }
        content[index-1]->setNextId(-1);

    }
    return content;
}

vector<string> Session::getCommand(string s, string delimiter){//split the command typed by the user into words. return a vector of words.(each position is a word)
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}

