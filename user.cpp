//
// Created by 陆辰暘 on 2024/5/10.
//
#include "user.h"
#include <fstream>
#include <cmath>
using namespace std;

User::User(const std::string &name, const std::string &password) :name(name),decryptedPassword(password){

}

string trAscii(char c){
    int a=(int)c;
    int ans=0,k=1;
    while(a>0){
        ans+=(a%2)*k;
        k*=10;
        a/=2;
    }
    string s= to_string(ans);int l=s.length();
    for(int i=l;i<=8;i++){
        s="0"+s;
    }
    return s;
}
char trStr(string s){
    int ans=0;
    for(int i=0;i<6;i++){
        ans+=((int)s[i])*pow(2,5-i);
    }
    char c=(char)ans;
    return c;
}
string User::encryption(const std::string &password) {
    string pass(password);string ans="";
    int len=pass.length();
    for(int i=0;i<len%3;i++){
        pass.append("=");
    }
    len=pass.length();
    for(int i=0;i<len;i+=3){
        char a=pass[i],b=pass[i+1],c=pass[i+2];
        string a1=trAscii(a),b1= trAscii(b),c1= trAscii(c);
        string s1=a1.substr(0,6),s2=a1.substr(6,2)+b1.substr(0,4),s3=b1.substr(4,4)+c1.substr(0,2),s4=c1.substr(2,6);
        ans=ans+ trStr(s1)+ trStr(s2)+ trStr(s3)+ trStr(s4);
    }
    return ans;
}