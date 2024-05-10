//
// Created by 陆辰暘 on 2024/4/23.
//
#include "account.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct deleter{
    template<class T>void operator()(T* p){delete p;}
};

class Controller{
private:
    Date date;
    vector<Account *> accounts;
    bool end;
public:
    Controller(const Date &date): date(date),end(false){}
    ~Controller();
    const Date &getDate()const{return date;}
    bool isEnd()const{return end;}
    bool runCommand(const string &cmdLine);
};
Controller::~Controller() {
    for_each(accounts.begin(),accounts.end(),deleter());
}
bool Controller::runCommand(const string &cmdLine) {
    istringstream str(cmdLine);
    char cmd,type;
    int index,day;
    double amount,credit,rate,fee;
    string id,desc;
    Account *account;
    Date date1,date2;
    str>>cmd;
    switch (cmd) {
        case 'a':
            str>>type>>id;
            if(type=='s'){
                str>>rate;
                account=new SavingAccount(date,id,rate);
            }
            else{
                str>>credit>>rate>>fee;
                account=new CreditAccount(date,id,credit,rate,fee);
            }
            accounts.push_back(account);
            return true;
        case 'd':
            str>>index>>amount;
            getline(str,desc);
            accounts[index]->deposit(date,amount,desc);
            return true;
        case 'w':
            str>>index>>amount;
            getline(str,desc);
            accounts[index]->withdraw(date,amount,desc);
            return true;
        case 's':
            for(int i=0;i<accounts.size();i++){
                cout<<"["<<i<<']';
                accounts[i]->show(cout);
                cout<<endl;
            }
            return false;
        case 'c':
            str>>day;
            if(day<date.getDay()){
                cout<<"You cannot specify a previous day";
                return false;
            } else if(day>date.getMonthDay()){
                cout<<"Invalid day";
                return false;
            }
            else{
                date=Date(date.getYear(),date.getMonth(),day);
            }
            return true;
        case 'n':
            if(date.getMonth()==12){
                date=Date(date.getYear()+1,1,1);
            }
            else{
                date=Date(date.getYear(),date.getMonth()+1,1);
            }
            for(auto &paccount:accounts){
                paccount->settle(date);
            }
            return true;
        case 'q':
            str>>date1>>date2;
            Account::query(date1,date2);
            return false;
        case 'e':
            end=true;
            return false;
    }
    cout<<"Invalid command: "<<cmdLine<<endl;
    return false;
}
void test(){
    Date beginDate(2020,1,1);
    cout<<"(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit"<<endl;

    Controller controller(beginDate);
    string cmdLine;
    const char *FILE_NAME="commands.txt";
    ifstream fileIn(FILE_NAME);
    if(fileIn){
        while(getline(fileIn,cmdLine)){
            controller.runCommand(cmdLine);
        }
        fileIn.close();
    }
    ofstream fileOut(FILE_NAME,ios_base::app);
    cout<<"(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit"<<endl;
    while(!controller.isEnd()){
        cout<<controller.getDate()<<"\tTotal: "<<Account::getTotal()<<"\tcommand>";
        string cmd;
        getline(cin,cmd);
        if(controller.runCommand(cmd)){
            fileOut<<cmd<<endl;
        }
    }
}

int main(){
    test();
    return 0;
}