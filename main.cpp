//
// Created by 陆辰暘 on 2024/4/23.
//
#include "account.h"
#include <iostream>
using namespace std;

int main(){
    Date beginDate(2024,1,1);
    SavingAccount accounts[]={SavingAccount(beginDate,"131313",0.015),
                              SavingAccount(beginDate,"13122",0.015)};

    const int n=sizeof (accounts)/ sizeof(SavingAccount);
    //模拟
    accounts[0].deposit(Date(2024,11,5),5000,"salary");
    accounts[1].deposit(Date(2024,11,15),10000,"ok");
    accounts[0].deposit(Date(2024,12,12),100,"www");
    accounts[1].withdraw(Date(2024,12,15),200,"wat");
    cout<<endl;
    for(int i=0;i<n;i++){
        accounts[i].settle(Date(2009,1,1));
        accounts[i].show();
        cout<<endl;
    }
    cout<<"Total: "<<SavingAccount::getTotal()<<endl;
    return 0;
}