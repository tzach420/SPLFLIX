//
// Created by tzachu@wincs.cs.bgu.ac.il on 22/11/2019.
//
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <map>
using namespace std;
User::User(const string& name):history(), name(name){}
User::~User() {}//empty dis. the content of the history will not be deleted here. (will be deleted in Session destructor).
std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history()  {
    return history;
}
std::vector<Watchable*> &User::getHistoryByRef(){
    return history;
}//returns the history of the user.
void User::printHistory() {
    int j = 1;
    for (auto i = history.begin(); i != history.end(); ++i){ //create string from the tags vector
        cout << to_string(j) + ". " + (*i)->toString() << endl;
        j++;
    }
}
void User::setName(const std::string& name){
    this->name=name;
}
void User::addToHistory(Watchable* toAdd){
    history.push_back(toAdd);
}
Watchable* User::getRecommendation(Session& s){
    return nullptr;
}







///CLASS LengthRecommenderUser
LengthRecommenderUser::LengthRecommenderUser(const std::string& name):User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    if(history.size()==0) return nullptr;//the history is empty.
    double avg=0;
    int id_closest_to_avg=-1;
    long long min_distance_from_avg= INTMAX_MAX;
    for(auto & it: history)
        avg+=it->getLength();//for the average.
    avg/=history.size();//calculate average.
    for( auto & cont: s.getContent()){
        bool in_history=false;//if cont already been watched, dont recommend.
        for(auto i = history.begin(); i!=history.end() && !(in_history);++i)
            if((*i)->getId()==cont->getId()) in_history=true;
        if(!in_history){//if the user didnt  watched cont in the past.
            int distance= abs(avg-cont->getLength());//calculate distance from avg.
            if(distance<min_distance_from_avg) {
                min_distance_from_avg = distance;//update new closest.
                id_closest_to_avg=cont->getId();
            }
        }

    }
    if(id_closest_to_avg==-1) return nullptr;
    return s.getContent()[id_closest_to_avg];
}
User* LengthRecommenderUser::clone(){
    LengthRecommenderUser *user= new LengthRecommenderUser(this->getName());
    for(auto & it: this->history)
        user->history.push_back(it);
    return user;
}


///CLASS RerunRecommenderUser
RerunRecommenderUser::RerunRecommenderUser (const std::string& name):User(name), index_of_next_recommendation(0){
}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if(history.size()==0) return nullptr;//if the history is empty.
    int to_return=index_of_next_recommendation;
    index_of_next_recommendation=(index_of_next_recommendation+1) % history.size();//module history size.
    return history[to_return];
}
User* RerunRecommenderUser::clone(){
    RerunRecommenderUser *user= new RerunRecommenderUser(this->getName());
    user->index_of_next_recommendation=this->index_of_next_recommendation;
    for(auto & it: this->history)
        user->history.push_back(it);
    return user;
}

///CLASS GenreRecommenderUser
GenreRecommenderUser::GenreRecommenderUser (const std::string& name):User(name){}
std::map<std::string,int>* GenreRecommenderUser::initTagMap(){
    map <string, int>* mymap=new map<string,int>;
    for(auto it : history){//add all the user watch history to the map.
        vector<string> tags= it->getTags();
        for( auto tag: tags){
            if(mymap->count(tag)>0)
                (*mymap)[tag]++;
            else mymap->insert(pair<string,int>(tag,1));
        }
    }
    return mymap;
}
string GenreRecommenderUser::findTagToSearch(std::map<string,int>*& mymap){
    int max=-1;
    for(auto pair: *mymap)
        if(pair.second>max)
            max = pair.second;
    for(auto pair : *mymap)
        if(pair.second==max)
            return  pair.first;
    return nullptr;
}
bool User::userDidntWatch(Watchable* & content){
        for(auto & it:history)
            if(content->toString()==it->toString())
                return false;
        return true;
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if (history.size() == 0) return nullptr;//if the history is empty.
    else {
        map<string, int> *mymap = initTagMap(); //init map- free the map on this function(ordered by lex order)
        //try to find content to recommend
        while (!mymap->empty()) {
            string tag = findTagToSearch(mymap);//find the tag with the highest number of appearance in user history(already order by lex)
            for (auto cont: s.getContent())
                if (cont->containsTag(tag) && userDidntWatch(cont)) {//if you found content, return it.
                    mymap->clear();
                    delete mymap;
                    return cont;
                }
            mymap->erase(tag);
            }
        mymap->clear();
        delete mymap;
        return nullptr;//didnt find content to recommend.
    }
}
User* GenreRecommenderUser::clone(){
    GenreRecommenderUser *user= new GenreRecommenderUser(this->getName());
    for(auto & it: this->history)
        user->history.push_back(it);
    return user;
}



