#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;
using namespace std;
enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

///////////////////////BASE ACTION/////////////////////////////////////////
class BaseAction{
public:
	BaseAction();//empty constructor
    virtual~BaseAction()=0;//virtual distructor.
	virtual void act(Session& sess)=0;// perform a specific action (not implimented here, implimented on each act)
	virtual std::string toString() const=0;
    virtual BaseAction* clone()=0;//virtual method, return deep copy of the action.
    void setErrorMsg(const string & msg);
    void setStatus(const ActionStatus e);
    ActionStatus getStatus() const;
protected:
	void complete();// called in order to change to status to COMPLETED.
	void error(const std::string& errorMsg);//display to the screen the error messege and change the status to ERROR.
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;// pending, completed, error
};
///////////////////////CREATE USER/////////////////////////////////////////
class CreateUser  : public BaseAction {
public:
    CreateUser(string userName,string recommend_algo);//contructor.
    virtual~CreateUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
private: //our addition
    string userName;
    string recommend_algo;// len,rer,gen.
};
///////////////////////CHANGE USER/////////////////////////////////////////
class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(string name);
    virtual ~ChangeActiveUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();

private:
    string nameActiveUser;//name of the new active user.
};
///////////////////////DELETE USER/////////////////////////////////////////
class DeleteUser : public BaseAction {
public:
    DeleteUser(string name);
    virtual ~DeleteUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
private:
    string name;
};
///////////////////////DUPLICATE USER/////////////////////////////////////////
class DuplicateUser : public BaseAction {
public:
    DuplicateUser(string originName, string newName);//create duplicate of originname with name newname.
    virtual ~DuplicateUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();

private:
    string originName;
    string newName;
};
///////////////////////PRINT CONTENT LIST/////////////////////////////////////////
class PrintContentList : public BaseAction {
public:
    PrintContentList();
    virtual ~PrintContentList();
    virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
///////////////////////PRINT WATCH HISTORY/////////////////////////////////////////
class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    virtual ~PrintWatchHistory();
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();

};
///////////////////////WATCH/////////////////////////////////////////
class Watch : public BaseAction {
public:
    Watch(long contentId);
    virtual ~Watch();
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();

private:
   long unsigned contentId;
};
///////////////////////PRINT ACTION LOG/////////////////////////////////////////
class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual ~PrintActionsLog();
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
///////////////////////EXIT/////////////////////////////////////////
class Exit : public BaseAction {
public:
    Exit();
    virtual ~Exit();
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
#endif
