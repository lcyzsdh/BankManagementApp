#include <iostream>

using namespace std;

enum class Activity{DEPOSIT,WITHDRAW};

class Date{
private:
    int day,month,year;
public:
    Date(int y,int m,int d):year(y),month(m),day(d){}
};

//储存账户类
class SavingAccount{
private:
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
    SavingAccount(int date,int id,double rate):lastDate(date),id(id),rate(rate){
        cout<<"#"<<id<<"was created successfully."<<endl;
    }
    int getId(){return id;}
    double getBalance(){return balance;}
    double getRate(){return rate;}
    void deposit(int date,double amount);
    void withdraw(int date,double amount);
    void settle(int date);
    void show();

};

void SavingAccount::record(Activity activity, int date, double amount) {
    if(activity==Activity::DEPOSIT){
        lastDate=date;
        balance+=amount;
    } else{
        lastDate=date;
        if(balance-amount<0){
            cout<<"Error!Balance is under zero!!!"<<endl;
        }
        else{
            balance-=amount;
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

}

void SavingAccount::show() {
    cout<<"#"<<balance
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
    return 0;
}
