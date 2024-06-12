//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_ACCOUNT_H
#define BANKAPP_ACCOUNT_H
#include "accumulator.h"
#include "date.h"
#include <string>
#include <map>
#include <istream>
#include "FinancialManagement.h"
#include<utility>
using namespace std;
enum class Activity{DEPOSIT,WITHDRAW};
class Account;
class AccountRecord{
private:
    Date date;
    const Account *account;
    double amount;
    double balance;
    string desc;
public:
    AccountRecord(const Date &date,const Account *account,double amount,double balance,std::string desc);
    void show()const;
};
typedef multimap<Date,AccountRecord> RecordMap;
class Account{
private:
    string id;
    double balance;
    static double total;
    static RecordMap recordMap;
protected:
    Account(const Date &date, const string &id);
    void record(Activity activity,const Date &date,double amount,const std::string &desc);
public:
    const string &getId()const{return id;}
    double getBalance() const {
        return balance;
    }
    static double getTotal() {
        return total;
    }
    virtual void deposit(const Date &date,double amount,const std::string &desc)=0;
    virtual bool withdraw(const Date &date, double amount, const std::string &desc)=0;
    virtual void settle(const Date &date)=0;
    virtual void show(std::ostream &out)const;
    static void query(const Date& begin,const Date& end);//查询时间
    virtual bool buyFM(FinancialManagement fm, double t)=0;
    virtual bool isFMAccount()=0;
};
inline ostream & operator<< (ostream &out,const Account &account){
    account.show(out);
    return out;
}

//储存账户类
class SavingAccount:public Account{
private:
    Accumulator acc;
    double rate;//定期存款利率
public:
    SavingAccount(const Date &date, const string &id, double rate);
    double getRate()const{return rate;}
    void deposit(const Date &date,double amount,const std::string &desc) override ;
    bool withdraw(const Date &date, double amount, const std::string &desc) override;
    void settle(const Date &date) override;
    bool buyFM(FinancialManagement fm, double t) override;
    bool isFMAccount() override;

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
    void deposit(const Date &date,double amount,const std::string &desc) override;
    bool withdraw(const Date &date, double amount, const std::string &desc)override;
    void settle(const Date &date)override;
    void show(ostream &out)const override;
    bool buyFM(FinancialManagement fm, double t) override;
    bool isFMAccount() override;
};

class FinancialAccount:public Account{
private:
    vector<pair<FinancialManagement,double>> FinancialList;
    double highRisk;
    int buy(FinancialManagement &item,double am);
public:
    FinancialAccount(const Date &date,const string &id,double highRisk);
    bool buyFM(FinancialManagement fm, double t) override;
    bool isFMAccount() override;
    void deposit(const Date &date,double amount,const std::string &desc) override;
    bool withdraw(const Date &date, double amount, const std::string &desc)override;
    void settle(const Date &date)override;
    void show(ostream &out)const override;
};
class AccountException:public std::runtime_error{
private:
    const Account *account;
public:
    AccountException(const Account *account,const std::string &msg): std::runtime_error(msg),account(account){}
    const Account *getAccount() const{return account;}
};
#endif //BANKAPP_ACCOUNT_H
