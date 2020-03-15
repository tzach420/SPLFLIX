#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
class Watchable;
class Session;

class LengthRecommenderUser;

class User{
public:
    User(const std::string& name);//constructor.
    virtual~User();//destructor.
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual User* clone()=0;//deep copy
    void addToHistory(Watchable* toAdd);//add Watchable to the history of the user.
    void printHistory();//prints user history.
    void setName(const std::string& name);
    std::vector<Watchable*> get_history() ;
    std::vector<Watchable*> &getHistoryByRef() ;//returns the user history.
    std::string getName() const;


protected:
    std::vector<Watchable*> history;
    bool userDidntWatch(Watchable* & content);//return true if the user didnt watch content, false otherwise.
private:
     std::string name;

};

class LengthRecommenderUser : public User {//prefer to watch content whose length is closest to the avg length of all the things he watched so far.
public:
    LengthRecommenderUser(const std::string& name);//constructor.
    virtual Watchable* getRecommendation(Session& s);// return the next recommendation.
    virtual User* clone();
private:
};

class RerunRecommenderUser : public User {//recommned by cycle: first recommend the first WATCHABLE, then second, third..... n.(saves the index of the last recomandation) 
public:
    RerunRecommenderUser(const std::string& name);//constructor.
    virtual Watchable* getRecommendation(Session& s);// return the next recommendation.
    virtual User* clone();
private:
    int index_of_next_recommendation;//holds the id of the next recommendation.
};

class GenreRecommenderUser : public User {//recommend content based on the most popular tag in the user WATCH HISTORY. if a set of tags has the same value, it will recommand by lex order. recommand on content that the user didnt watch already. If no such content exists, it will try with the second most popular tag, and so on.
public:
    GenreRecommenderUser(const std::string& name);//constructor.
    virtual Watchable* getRecommendation(Session& s);// return the next recommendation.
    virtual User* clone();
    std::map<std::string,int>* initTagMap();//return map of [tag name][number_of_appearance] of the user current history.
    std::string findTagToSearch(std::map<std::string,int>*& mymap);//find the tag with the highest number of appearance in user history.
private:

};
#endif
