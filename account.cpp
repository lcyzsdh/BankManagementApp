#include "account.h"
#include <iostream>
#include<cmath>
using namespace std;

double SavingAccount::total=0;

void SavingAccount::record(Activity activity,const Date &date,double amount,const std::string &desc) {
    if(activity==Activity::DEPOSIT){
        accumulation= accumulate(date);
        lastDate=date;
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
            accumulation= accumulate(date);
            lastDate=date;
            amount= floor(amount*100+0.5)/100;
            balance+=amount;
            total+=amount;
            date.show();
            cout<<"\t#"<<id<<"\t"<<amount<<"\t"<<balance<<"\t"<<desc<<endl;
        }
    }
}

void SavingAccount::deposit(const Date &date, double amount,const std::string &desc) {
    record(Activity::DEPOSIT,date,amount,desc);
}

void SavingAccount::withdraw(const Date &date, double amount,const std::string &desc) {
    record(Activity::WITHDRAW,date,amount,desc);
}

void SavingAccount::settle(const Date& date) {
    double interest = accumulate(date)*rate/365;
    if(interest!=0){
        record(Activity::DEPOSIT,date,interest,"settle");
    }
    accumulation=0;
}

void SavingAccount::show() const {
    cout<<"#"<<id<<"   have "<<balance<<endl;
}

SavingAccount::SavingAccount(const Date &date, const string &id, double rate) : lastDate(date), id(id), rate(rate), accumulation(0){
    lastDate.show();
    cout<<"\t#"<<id<<" is created."<<endl;
}

void SavingAccount::error(const string &msg) const {
cout<<"error:  "<<msg<<endl;
}