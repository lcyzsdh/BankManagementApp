#include "account.h"
#include <iostream>

using namespace std;

class Date{
private:
    int day,month,year;
public:
    Date(int y,int m,int d):year(y),month(m),day(d){}
};

double SavingAccount::total=0;

void SavingAccount::record(Activity activity, int date, double amount) {
    if(activity==Activity::DEPOSIT){
        lastDate=date;
        balance+=amount;
        total+=amount;
    } else{
        if(balance-amount<0){
            cout<<"Error!Balance is under zero!!!"<<endl;
        }
        else{
            lastDate=date;
            balance-=amount;
            total-=amount;
        }
    }
}

void SavingAccount::deposit(int date, double amount) {
    record(Activity::DEPOSIT,date,amount);
}

void SavingAccount::withdraw(int date, double amount) {
    record(Activity::WITHDRAW,date,amount);
}

void SavingAccount::settle(int date) {
    double interest = accumulate(date)*rate/365;
    if(interest!=0){
        record(Activity::DEPOSIT,date,interest);
    }
    accumulation=0;
}

void SavingAccount::show() const {
    cout<<"#"<<id<<"   have "<<balance<<endl;
}

SavingAccount::SavingAccount(int date, int id, double rate) :lastDate(date),id(id),rate(rate),accumulation(0){
    cout<<date<<"\t#"<<id<<" is created."<<endl;
}

int main() {
    SavingAccount s1(1,136,0.08);
    SavingAccount s2(1,137,0.05);
    s1.deposit(2,500);
    s2.deposit(3,1000);
    s2.withdraw(7,200);
    s1.settle(50);
    s2.settle(50);
    s1.show();
    s2.show();
    cout<<"Total::"<<SavingAccount::getTotal()<<endl;
    return 0;
}
