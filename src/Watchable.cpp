
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
using namespace std;


///////////////////////constructor//////////////////////
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(tags){} //constructor
Watchable::~Watchable() {} //distructor
const int Watchable::getLength() const{
    return length;
}
const std::vector<std::string> Watchable::getTags() const{
    return tags;
}
const long Watchable::getId() const {
        return id;
}
bool Watchable::containsTag(const  std::string& s){//check if tags contain s
    for(auto it: tags)
        if(it==s) return true;
    return false;
}

////////////////////////////////static function/////////////////////////////

string Watchable::vector_to_string(std::vector<std::string> &vec) const { //return long string of the string in the vector
    string Tags="[";
    for(auto i = vec.begin(); i!=vec.end();++i)//create string from the tags vector
        Tags+= *i+", ";
    Tags=Tags.substr(0, Tags.size()-1);
    Tags.at(Tags.size()-1)=']';
    return Tags;
}

////////////////////////////////////MOVIE CLASS///////////////////////////////



/////////////////////constructor////////////////////////
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags): Watchable(id,length,tags), name(name) {}

std::string Movie::getName() const {return name;}
Watchable* Movie::getNextWatchable(Session & s) const{
    return s.getActiveUser().getRecommendation(s);
}
std::string Movie::toString() const{
    return name;
}
std::string Movie::fullToString() const{    //string include: name, seasion/episode(if episode),leangth' tags
    vector<string> vector_of_tags=this->getTags();
    string Tags=Watchable::vector_to_string(vector_of_tags);
    return name+" "+to_string(this->getLength())+" minutes "+Tags;
}
Watchable* Movie::clone(){
    Watchable *movie=new Movie(this->getId(),this->name,this->getLength(),this->getTags());
    return movie;
}
void Movie::setNextId( long id ){};//our addition


////////////////////////////EPISODE CLASS///////////////////////

/////////////////////////////constructor////////////////////////
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):
                 Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(-1) {}

std::string Episode::getName() const {return seriesName;}
Watchable* Episode::getNextWatchable(Session & s) const {
    if(nextEpisodeId!=-1){//the last episode of each series is -1, mean no have next episode
        return s.getContent()[nextEpisodeId]; //return next episode
    }
    else{//return the recomm.
        return s.getActiveUser().getRecommendation(s);
    }
}
std::string Episode::toString() const{
    return seriesName+" S"+ to_string(season)+"E"+to_string(episode);
}
string Episode::fullToString() const {
    vector<string> vector_of_tags=this->getTags();
    string Tags=Watchable::vector_to_string(vector_of_tags); //string of tags: [tag1, tag2, ...]
    return seriesName+" S"+ to_string(season)+"E"+to_string(episode)+" "+to_string(this->getLength())+" minutes"+" " +Tags;
}
void Episode::setNextId( long id ){
   nextEpisodeId=id;
}
Watchable* Episode::clone(){ ////deep copy
    Watchable *episode=new Episode(this->getId(),this->seriesName,this->getLength(),this->season,this->episode,this->getTags());
    return episode;
}

