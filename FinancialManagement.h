//
// Created by 陆辰暘 on 2024/5/19.
//

#ifndef BANKAPP_FINANCIALMANAGEMENT_H
#define BANKAPP_FINANCIALMANAGEMENT_H
#include "date.h"
#include <cstring>
#include <cstdlib>
class FinancialManagement {
private:
    Date beginDate,endDate;
    double rate;
    double risk;
    std::string desc;
    double leastAmount;
    //double expectedAmount;
public:
    FinancialManagement(const std::string &desc,Date begin,Date end,double rate,double leastAmount,double risk=1):desc(desc), beginDate(begin),
                                                                                                                     endDate(end),rate(rate),leastAmount(leastAmount),risk(risk){
    }
    double settle(double amount)const{
        int day=endDate-beginDate;
        double am=0;
        for(int i=0;i<day;i++){
            double r=rand()%2;
            if(r>=risk){
                am+=amount*rate/365;
            }
        }
        return am;
    }
    double getRate() const {
        return rate;
    }
    double getRisk() const {
        return risk;
    }
    const Date &getBeginDate() const {
        return beginDate;
    }
    const Date &getEndDate() const {
        return endDate;
    }
    const std::string &getDesc() const {
        return desc;
    }

    double getLeastAmount() const {
        return leastAmount;
    }

    void show() const{
        std::cout<<"#"+desc+"# "<<"from ";getBeginDate().show();std::cout<<" to ";getEndDate().show();std::cout<<" leastAmount:"<<getLeastAmount()<<" risk:"<<getRisk()<<std::endl;
    }
};


#endif //BANKAPP_FINANCIALMANAGEMENT_H
