//
// Created by 陆辰暘 on 2024/4/9.
//

#ifndef BANKAPP_DATE_H
#define BANKAPP_DATE_H
class Date{
private:
    int day,month,year;
    int totalDays;//从公元元年1月1日开始的第几天
public:
    Date(int y,int m,int d);
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
};
#endif //BANKAPP_DATE_H
