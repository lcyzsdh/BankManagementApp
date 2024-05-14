//
// Created by 陆辰暘 on 2024/5/10.
//

#ifndef BANKAPP_USER_H
#define BANKAPP_USER_H
#include<string>
#include <vector>
class User{
private:

    std::string name;
public:
    User(const std::string &name="");
    virtual std::vector<std::string>& getInfo()=0;
    static std::string encryption(const std::string &password);
    static std::string decryption(const std::string &password);
    std::string getName()const{
        return name;
    }
};

class NormalUser:public User{
public:
    NormalUser(const std::string &name);
    std::vector<std::string>& getInfo() override;
};

class Administrator:public User{
public:
    Administrator(const std::string &name);
    std::vector<std::string>& getInfo() override;
};
#endif //BANKAPP_USER_H
