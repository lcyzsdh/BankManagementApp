//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_ACCOUNT_H
#define BANKAPP_ACCOUNT_H
enum class Activity{DEPOSIT,WITHDRAW};

//储存账户类
class SavingAccount{
private:
    static double total;//所有账号的存款之和
    int id;//账号
    double balance{};//余额
    double rate;//定期存款利率
    int lastDate;//最后更新日期
    double accumulation{};//余额累积存款之和
    void record(Activity activity,int date,double amount);//操作
    double accumulate(int date) const{
        return accumulation+balance*(date-lastDate);
    }
public:
    SavingAccount(int date,int id,double rate);
    int getId ()const{return id;}
    double getBalance ()const{return balance;}
    double getRate()const{return rate;}
    static double getTotal(){return total;}
    void deposit(int date,double amount);
    void withdraw(int date,double amount);
    void settle(int date);
    void show() const;

};
#endif //BANKAPP_ACCOUNT_H
