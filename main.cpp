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
bool Controller::runCommand(const std::string &cmdLine) {
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
            cin>>type>>id;
            if(type=='s'){
                cin>>rate;
                account=new SavingAccount(date,id,rate);
            }
            else{
                cin>>credit>>rate>>fee;
                account=new CreditAccount(date,id,credit,rate,fee);
            }
            accounts.push_back(account);
            return true;
        case 'd':
            cin>>index>>amount;
            getline(cin,desc);
            accounts[index]->deposit(date,amount,desc);
            return true;
        case 'w':
            cin>>index>>amount;
            getline(cin,desc);
            accounts[index]->withdraw(date,amount,desc);
            return true;
        case 's':
            for(const auto &paccount:accounts){
                cout<<"["<<i<<"]";
                paccount->show();
                cout<<endl;
            }
            return false;
        case 'c':
            cin>>day;
            if(day<date.getDay()){
                cout<<"You cannot specify a previous day";
            } else if(day>date.getMonthDay()){
                cout<<"Invalid day;"
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
    vector<Account *> accounts;
    cout<<"Welcome!"<<endl;

    Controller controller(beginDate);
    string cmdLine;
    //const
    CreditAccount ca(beginDate,"c1111",1000000,0.0005,50);
    SavingAccount s1(beginDate,"131313",0.015);
    SavingAccount s2(beginDate,"13122",0.015);
    Account *accounts[]={&s1,&s2,&ca};
    //模拟
    accounts[0]->deposit(Date(2024,11,5),5000,"salary");
    ca.withdraw(Date(2024,11,15),200,"withdrew1");
    accounts[1]->deposit(Date(2024,11,15),10000,"ok");
    accounts[0]->deposit(Date(2024,12,12),100,"www");
    accounts[1]->withdraw(Date(2024,12,15),200,"wat");
    cout<<endl;
    accounts[0]->settle(Date(2025,1,1));
    accounts[1]->settle(Date(2025,1,1));
    ca.settle(Date(2025,1,1));
    //accounts[0]->show();cout<<endl;
    //accounts[1]->show();cout<<endl;
    //ca.show();cout<<endl;
    cout<<"Total: "<<SavingAccount::getTotal()<<endl;


    //Date d1=Date::read();
    //Date d2=Date::read();
    Account::query(d1,d2);
}

int main(){
    test();
    return 0;
}