//
// Created by 陆辰暘 on 2024/5/5.
//

#ifndef BANKAPP_ACCUMULATOR_H
#define BANKAPP_ACCUMULATOR_H
#include "date.h"
class Accumulator{
private:
    Date lastDate;
    double value;
    double sum;
public:
    Accumulator(const Date &date,double value): lastDate(date),value(value),sum(0){}
    double getSum(const Date &date) const{
        return sum+value*(date-lastDate);
    }
    void change(const Date &date,double nValue){
        sum= getSum(date);
        lastDate=date;value=nValue;
    }
    void reset(const Date &date,double nValue){
        lastDate=date;value=nValue;sum=0;
    }
};
#endif //BANKAPP_ACCUMULATOR_H
