//
// Created by 陆辰暘 on 2024/5/10.
//

#ifndef BANKAPP_USER_H
#define BANKAPP_USER_H
#include<string>
#include <vector>
enum State{
    NORMAL,ICED
};
class User{
private:
    std::string name;
    State nState;
public:
    User(const std::string &name="");
    virtual std::vector<std::string>& getInfo(std::vector<std::string>& in)=0;
    static std::string encryption(const std::string &password);
    static std::string decryption(const std::string &password);
    std::string getName()const{
        return name;
    }
    virtual bool isAdmin()=0;
    virtual bool isVIP()=0;
};

class NormalUser:public User{
public:
    NormalUser(const std::string &name);
    std::vector<std::string>& getInfo(std::vector<std::string>& in) override;
    bool isAdmin() override;
    bool isVIP() override;
};

class Administrator:public User{
private:
    bool isAd;
public:
    Administrator(const std::string &name);
    std::vector<std::string>& getInfo(std::vector<std::string>& in) override;
    bool isAdmin() override;
    bool isVIP() override;
};
class VIPUser:public User{
private:
    int star;
public:
    VIPUser(const std::string &name);
    std::vector<std::string>& getInfo(std::vector<std::string>& in) override;
    bool isAdmin() override;
    bool isVIP() override;
};
#endif //BANKAPP_USER_H
