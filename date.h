//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_DATE_H
#define BANKAPP_DATE_H
#include <iostream>
class Date{
private:
    int day,month,year;
    int totalDays;//从公元元年1月1日开始的第几天
public:
    Date(int y=1,int m=1,int d=1);
    int getDay() const{return day;}
    int getMonth() const{return month;}
    int getYear() const{return year;}
    int getMonthDay()const;
    bool isLeapYear() const{
        return year%4==0&&year%100!=0||year%400==0&&year%3200!=0;
    }
    void show() const;
    int operator- (const Date& date)const {
        return totalDays-date.totalDays;
    }
    bool operator< (const Date& date)const{
        return totalDays<date.totalDays;
    }
};
std::istream & operator>> (std::istream &in,Date &date);
std::ostream & operator<< (std::ostream &out,const Date &date);

#endif //BANKAPP_DATE_H
