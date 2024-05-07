//
// Created by 陆辰暘 on 2024/4/23.
//
#include "account.h"
#include <iostream>
using namespace std;

void test(){
    Date beginDate(2024,1,1);
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
    accounts[0]->show();cout<<endl;
    accounts[1]->show();cout<<endl;
    ca.show();cout<<endl;
    cout<<"Total: "<<SavingAccount::getTotal()<<endl;
}

int main(){
    test();
    return 0;
}