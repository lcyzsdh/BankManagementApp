//
// Created by 陆辰暘 on 2024/5/10.
//
#include "user.h"
#include <fstream>
#include <cmath>
using namespace std;
static const std::string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
User::User(const std::string &name, const std::string &password) :name(name),decryptedPassword(password){
}

string User::encryption(const std::string &password) {
    string encode;
    int b3[3]={},k=0;
    int b4[4]={},m=0;
    for(int i=0;i<password.length();i++){
        b3[k++]=password[i];
        if(k==3){
            b4[0] = (b3[0] & 0xfc) >> 2;
            b4[1] = ((b3[0] & 0x03) << 4) | ((b3[1] & 0xf0) >> 4);
            b4[2] = ((b3[1] & 0x0f) << 2) | ((b3[2] & 0xc0) >> 6);
            b4[3] = b3[2] & 0x3f;
            for(int j=0;j<4;j++){
                encode+=base64_chars[b4[j]];
            }
            k=0;
        }
    }
    if(k!=0){
        for(int j=k;j<3;j++){
            b3[j]=0;
        }
        b4[0] = (b3[0] & 0xfc) >> 2;
        b4[1] = ((b3[0] & 0x03) << 4) | ((b3[1] & 0xf0) >> 4);
        b4[2] = ((b3[1] & 0x0f) << 2) | ((b3[2] & 0xc0) >> 6);
        for (int j = 0; j < k + 1; j++) {
            encode += base64_chars[b4[j]];
        }
        while (k++ < 3) {
            encode += '=';
        }
    }
    return encode;
}

string User::decryption(const std::string &password) {
    int len=password.length();
    string decode;
    int k=0,i=0,j=0;
    char c4[4],c3[3];
    while(len--&&password[k]!='='){
        c4[i++]=password[k];
        k++;
        if(i==4){
            for(i=0;i<4;i++){
                c4[i]=base64_chars.find(c4[i]);
            }
            c3[0] =(c4[0] << 2) + ((c4[1] & 0x30) >> 4);
            c3[1] =((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
            c3[2] = ((c4[2] & 0x3) << 6) + c4[3];
            for(i=0;i<3;i++){
                decode+=c3[i];
            }
            i=0;
        }
    }
    if(i){
        for(j=i;j<4;j++){
            c4[j]=0;
        }
        for(j=0;j<4;j++){
            c4[j]=base64_chars.find(c4[j]);
        }
        c3[0] = (c4[0] << 2) + ((c4[1] & 0x30) >> 4);
        c3[1] =((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
        c3[2] = ((c4[2] & 0x3) << 6) + c4[3];
        for(j=0;j<i-1;j++){
            decode+=c3[j];
        }
    }
    return decode;
}