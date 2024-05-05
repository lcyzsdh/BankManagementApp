//
// Created by 陆辰暘 on 2024/4/9.
//
#include "date.h"
#include <iostream>

using namespace std;
namespace {
    const int DAYS_BEFORE_MONTH[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
}
Date::Date(int y, int m, int d):year(y),month(m),day(d) {
    if(day<=0||day>getMonthDay()){
        cout<<"Invalid date!!"<<endl;
    }
    else{
        int years=year-1;
        totalDays=years*365+years/4-years/100+years/400-years/3200+DAYS_BEFORE_MONTH[month-1]+day;
        if(isLeapYear()&&month>2){
            totalDays++;
        }
    }
}
int Date::getMonthDay() const {
    if(isLeapYear()&&month==2){
        return 29;
    }
    else{
        return DAYS_BEFORE_MONTH[month]-DAYS_BEFORE_MONTH[month-1];
    }
}

void Date::show() const {
    cout<<getYear()<<'-'<<getMonth()<<'-'<<getDay()<<endl;
}

