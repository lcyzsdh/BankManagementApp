//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_ACCOUNT_H
#define BANKAPP_ACCOUNT_H
#include "accumulator.h"
#include "date.h"
#include <string>
using namespace std;
enum class Activity{DEPOSIT,WITHDRAW};

class Account{
private:
    string id;
    double balance;
    static double total;
protected:
    Account(const Date &date, const string &id);
    void record(Activity activity,const Date &date,double amount,const std::string &desc);
    void error(const std::string &msg) const;
public:
    const string &getId()const{return id;}
    double getBalance() const {
        return balance;
    }
    static double getTotal() {
        return total;
    }
    void show()const;
};

//储存账户类
class SavingAccount:public Account{
private:
    Accumulator acc;
    double rate;//定期存款利率
public:
    SavingAccount(const Date &date, const string &id, double rate);
    double getRate()const{return rate;}
    void deposit(const Date &date,double amount,const std::string &desc);
    void withdraw(const Date &date,double amount,const std::string &desc);
    void settle(const Date &date);

};

class CreditAccount:public Account{
private:
    Accumulator acc;
    double credit;
    double fee;
    double rate;
    double getDebt() const{
        double balance=getBalance();
        return (balance<0?balance:0);
    }
public:
    CreditAccount(const Date &date,const string &id,double credit,double rate,double fee);
    double getCredit()const{
        return credit;
    }
    double getRate()const{return rate;}
    double getFee() const {
        return fee;
    }
    double getAvailableCredit()const{
        if(getBalance()<0){
            return credit+getBalance();
        }
        else{
            return credit;
        }
    }
    void deposit(const Date &date,double amount,const std::string &desc);
    void withdraw(const Date &date,double amount,const std::string &desc);
    void settle(const Date &date);
    void show()const;
};
#endif //BANKAPP_ACCOUNT_H
