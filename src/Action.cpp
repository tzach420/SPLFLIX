#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <unordered_map>
using namespace std;

///////////////////////BASE ACTION/////////////////////////////////////////
BaseAction::BaseAction():errorMsg(""), status(PENDING){}//contructor.
BaseAction::~BaseAction(){}//destructor
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(const std::string &errorMsg)  {
    status = ActionStatus::ERROR;
    this->errorMsg=errorMsg;
    cout<<"Error - "+errorMsg<<endl; //check if we need it here
}
void BaseAction::setErrorMsg(const string & msg){
    errorMsg=msg;
}
void BaseAction::setStatus(const ActionStatus e){
    status=e;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

///////////////////////CREATE USER/////////////////////////////////////////
CreateUser::CreateUser(string userName, string recommend_algo):BaseAction(),userName(userName), recommend_algo(recommend_algo) {}
CreateUser::~CreateUser(){}//destructor
void CreateUser::act(Session& sess){
    if(recommend_algo!="len" && recommend_algo!="rer" && recommend_algo!="gen")//input check of the algo name.
        this->error("no legal algorithm");
    else if( sess.getUserMap().count(userName)>0)//check if user with that name already exists.
        this->error("name already exist"); ///need to check if name is legal
     else { //input legal
        User *u;
        if (recommend_algo == "len") {
            u = new LengthRecommenderUser(userName);
            sess.updateUserMap({userName, u});
        } else if (recommend_algo == "rer") {
            u = new RerunRecommenderUser(userName);
            sess.updateUserMap({userName, u});
        } else if (recommend_algo == "gen") {
            u = new GenreRecommenderUser(userName);
            sess.updateUserMap({userName, u});
        }
        u= nullptr;
        this->complete();
    }
}
std::string CreateUser::toString() const{
       if(getStatus()==COMPLETED)
           return "CreateUser COMPLETED";
       else if(getStatus()==ERROR) return "CreateUser ERROR: "+getErrorMsg();
       return "";
}
BaseAction* CreateUser::clone(){
    CreateUser* toReturn=new CreateUser(this->userName,this->recommend_algo);
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////CHANGE ACTIVE USER/////////////////////////////////////////
ChangeActiveUser::ChangeActiveUser(string nameActiveUser):nameActiveUser(nameActiveUser) {}//constructor
ChangeActiveUser::~ChangeActiveUser() {}//destructor
void ChangeActiveUser::act(Session& sess){
    if(sess.changeActiveUser(nameActiveUser)) this->complete();//changeActiveUser in Session class return true if change succed.
    else this->error("input name is invalid");//if change user failed.
}
std::string ChangeActiveUser::toString() const{
    if(getStatus()==COMPLETED)
        return "ChangeActiveUser COMPLETED";
    else if(getStatus()==ERROR) return "ChangeActiveUse ERROR: "+getErrorMsg();
    return "";
}
BaseAction* ChangeActiveUser::clone(){
    ChangeActiveUser* toReturn=new ChangeActiveUser(this->nameActiveUser);
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////DELETE USER/////////////////////////////////////////
DeleteUser::DeleteUser(string name): name(name) {}//constructor.
DeleteUser::~DeleteUser() {}//destructor
void DeleteUser::act(Session& sess){
    if(sess.deleteUser(name)) this->complete();//methood deleteUser implement in Session, return true if delete succeed
    else this->error("input name is invalid"); //don't have user with that name
}
std::string DeleteUser::toString() const{
    if(getStatus()==COMPLETED)
        return "DeleteUser COMPLETED";
    else if(getStatus()==ERROR) return "DeleteUser ERROR: "+getErrorMsg();
    return "";
}
BaseAction* DeleteUser::clone(){
    DeleteUser* toReturn=new DeleteUser(this->name);
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////DUPLICATE USER/////////////////////////////////////////
DuplicateUser::DuplicateUser(string originName, string newName):originName(originName),newName(newName) {}//constructor.
DuplicateUser::~DuplicateUser() {}//destructor
void DuplicateUser::act(Session& sess){
    if(sess.dupUser(originName,newName)) this->complete(); //dupUser implement in Session, return true if duplicate succeed
    else this->error("input name is invalid"); ////don't have user with that name
}
std::string DuplicateUser::toString() const{
    if(getStatus()==COMPLETED)
        return "DuplicateUser COMPLETED";
    else if(getStatus()==ERROR) return "DuplicateUser ERROR: "+getErrorMsg();
    return "";
}
BaseAction* DuplicateUser::clone(){
    DuplicateUser *toReturn=new DuplicateUser(this->newName,this->originName);
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////PRINT CONTENT LIST/////////////////////////////////////////
PrintContentList::PrintContentList() {} //constructor.
PrintContentList::~PrintContentList() {}
void PrintContentList::act (Session& sess){
   if(sess.printContentList()) this->complete(); ////printContentList implement in Session, return true if duplicate succeed, should always return true
}
std::string PrintContentList::toString() const{
    if(getStatus()==COMPLETED)
        return "PrintContentList COMPLETED";
    else if(getStatus()==ERROR) return "PrintContentList ERROR: "+getErrorMsg();
    return  "";
}
BaseAction* PrintContentList::clone(){
    PrintContentList *toReturn=new PrintContentList();
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////PRINT WATCH HISTORY/////////////////////////////////////////
PrintWatchHistory::PrintWatchHistory() {}//constructor.
PrintWatchHistory::~PrintWatchHistory() {}//destructor
void PrintWatchHistory::act(Session& sess) {
    cout<<"Watch history for "+sess.getActiveUser().getName()<<endl;
    sess.getActiveUser().printHistory();
    this->complete();//this act should always succeed
}
std::string PrintWatchHistory::toString() const{
    if(getStatus()==COMPLETED)
        return "PrintWatchHistory COMPLETED";
    else if(getStatus()==ERROR) return "PrintWatchHistory ERROR: "+getErrorMsg();
    return "";
}
BaseAction* PrintWatchHistory::clone(){
    PrintWatchHistory *toReturn=new PrintWatchHistory();
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}



///////////////////////WATCH/////////////////////////////////////////
Watch::Watch(long contentId):contentId(contentId) {}//constructor.
Watch::~Watch() {}//destructor
string Watch::toString() const {
    if(getStatus()==COMPLETED)
        return "Watch COMPLETED";
    else if(getStatus()==ERROR) return "Watch ERROR: "+getErrorMsg();
    return "";
}
void Watch::act(Session &sess) {
    vector<Watchable *> currContent = sess.getContent();
    if (contentId <= currContent.size() && contentId > 0) { //check if content id is valid
        cout << "Watching " + currContent[contentId - 1]->toString() << endl; //vector start from 0, content id from 1
        sess.getActiveUser().addToHistory(currContent[contentId-1]); //add to history of active user
        complete();
        Watchable *rec=currContent[contentId-1]->getNextWatchable(sess); //get the next episode or recomendition
        if(rec!=nullptr){ //there is something to recommend
            string answer;
            cout<<"We recommend watching "+rec->toString()+", continue watching? [y/n]"<<endl;
            cin>>answer;
            if(answer=="y"){
                BaseAction *execute=new Watch(rec->getId()+1);
                sess.getActionLog().push_back(execute);
                execute->act(sess);
            }
            else if(answer=="n"){} //is user doesn't want to watch recommendation, do nothing
        }
    }
    else { //content id not valid
        error("Content id not valid.");
    }

}
BaseAction* Watch::clone(){
   Watch *toReturn=new Watch(this->contentId);
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////PRINT ACTION LOG/////////////////////////////////////////
PrintActionsLog::PrintActionsLog(){}//constructor.
PrintActionsLog::~PrintActionsLog() {}
void PrintActionsLog::act(Session& sess){
    for(int i=sess.getActionLog().size()-1;i>=0;--i)
        cout<<sess.getActionLog()[i]->toString()<<endl;
    complete(); //this methood should succeed every time

}
std::string PrintActionsLog::toString() const{
    if(getStatus()==COMPLETED)
        return "PrintActionLog COMPLETED";
    else if(getStatus()==ERROR) return "PrintActionLog ERROR: "+getErrorMsg();
    return "";
}
BaseAction* PrintActionsLog::clone(){
    PrintActionsLog *toReturn=new PrintActionsLog();
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}


///////////////////////EXIT/////////////////////////////////////////
Exit::Exit(){}//constructor.
Exit::~Exit() {}
void Exit::act(Session& sess){
    complete(); //always succeed
}
std::string Exit::toString() const{
    if(getStatus()==COMPLETED)
        return "Exit COMPLETED";
    else if(getStatus()==ERROR) return "Exit ERROR: "+getErrorMsg();
    return "";
}
BaseAction* Exit::clone(){
    Exit* toReturn=new Exit();
    toReturn->setErrorMsg(this->getErrorMsg());
    toReturn->setStatus(this->getStatus());
    return toReturn;
}

