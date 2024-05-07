#include "account.h"
#include <iostream>
#include<cmath>
#include <utility>
using namespace std;
using namespace std::rel_ops;
AccountRecord::AccountRecord(const Date &date, const Account *account, double amount, double balance,
                             std::string desc) : date(date),account(account),amount(amount),
                             balance(balance),desc(std::move(desc)){}
void AccountRecord::show() const {
    date.show();
    cout<<"\t#"<<account->getId()<<"\t"<<amount<<"\t"<<balance<<"\t"<<desc<<endl;
}
double Account::total=0;
RecordMap Account::recordMap;
void Account::query(const Date &begin, const Date &end) {
    if(begin<=end){
        auto iter1=recordMap.lower_bound(begin);
        auto iter2=recordMap.upper_bound(end);
        for(auto iter=iter1;iter!=iter2;++iter){
            iter->second.show();
        }
    }
}
Account::Account(const Date &date, const std::string &id) :id(id),balance(0){
    date.show();cout<<"\t#"<<id<<" is created."<<endl;
}

void Account::record(Activity activity,const Date &date,double amount,const std::string &desc) {
    if(activity==Activity::DEPOSIT){
        amount= floor(amount*100+0.5)/100;
        balance+=amount;
        total+=amount;
        date.show();
        cout<<"\t#"<<id<<"\t"<<amount<<"\t"<<balance<<"\t"<<desc<<endl;
    } else{
        if(balance-amount<0){
            cout<<"Error!Balance is under zero!!!"<<endl;
        }
        else{
            amount= floor(amount*100+0.5)/100;
            balance-=amount;
            total-=amount;
            date.show();
            cout<<"\t#"<<id<<"\t"<<amount<<"\t"<<balance<<"\t"<<desc<<endl;
        }
    }
    recordMap.insert(make_pair(date, AccountRecord(date,this,amount,balance,desc)));//对每笔记录
}
void Account::error(const string &msg) const {
    cout<<"error:  "<<msg<<endl;
}
void Account::show() const {
    cout<<"#"<<id<<"   have "<<balance<<endl;
}
//SavingAccount实现
void SavingAccount::deposit(const Date &date, double amount,const std::string &desc) {
    record(Activity::DEPOSIT,date,amount,desc);
    acc.change(date,getBalance());
}

void SavingAccount::withdraw(const Date &date, double amount,const std::string &desc) {
    record(Activity::WITHDRAW,date,amount,desc);
    acc.change(date,getBalance());
}

void SavingAccount::settle(const Date& date) {
    double interest = acc.getSum(date)*rate/(date-Date(date.getYear()-1,1,1));//计算年息
    if(interest!=0){
        record(Activity::DEPOSIT,date,interest,"interest");
    }
    acc.reset(date,getBalance());
}


SavingAccount::SavingAccount(const Date &date, const string &id, double rate) : Account(date,id),rate(rate),acc(date,0){}
//CreditAccount实现
CreditAccount::CreditAccount(const Date &date, const std::string &id, double credit, double rate, double fee) : Account(date,id),credit(credit),rate(rate),fee(fee),acc(date,0){}
void CreditAccount::deposit(const Date &date, double amount, const std::string &desc) {
    record(Activity::DEPOSIT,date,amount,desc);
    acc.change(date,getDebt());
}
void CreditAccount::withdraw(const Date &date, double amount, const std::string &desc) {
    record(Activity::DEPOSIT,date,amount,desc);
    acc.change(date,getDebt());
}
void CreditAccount::settle(const Date &date) {
    double interest=acc.getSum(date)*rate;
    if(interest!=0){ record(Activity::DEPOSIT,date,interest,"interest");}
    if(date.getMonth()==1){
        record(Activity::WITHDRAW,date,fee,"annual fee");
    }
    acc.reset(date,getDebt());
}
void CreditAccount::show() const {
    Account::show();
    cout<<"\tAvailable credit: "<<getAvailableCredit();
}