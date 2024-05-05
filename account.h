//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_ACCOUNT_H
#define BANKAPP_ACCOUNT_H
#include "date.h"
#include <string>
using namespace std;
enum class Activity{DEPOSIT,WITHDRAW};

//储存账户类
class SavingAccount{
private:
    static double total;//所有账号的存款之和
    std::string id;//账号
    double balance{};//余额
    double rate;//定期存款利率
    Date lastDate;//最后更新日期
    double accumulation{};//余额累积存款之和
    void record(Activity activity,const Date &date,double amount,const std::string &desc);//操作
    double accumulate(Date date) const{
        return accumulation+balance*date.distance(lastDate);
    }
    void error(const std::string &msg) const;
public:
    SavingAccount(const Date &date, const string &id, double rate);
    const std::string &getId ()const{return id;}
    double getBalance ()const{return balance;}
    double getRate()const{return rate;}
    static double getTotal(){return total;}
    void deposit(const Date &date,double amount,const std::string &desc);
    void withdraw(const Date &date,double amount,const std::string &desc);
    void settle(const Date &date);
    void show() const;

};
#endif //BANKAPP_ACCOUNT_H
