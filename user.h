//
// Created by 陆辰暘 on 2024/5/10.
//

#ifndef BANKAPP_USER_H
#define BANKAPP_USER_H
#include<string>
class User{
private:
    std::string name;
    std::string decryptedPassword;
    std::string encryptedPassword;
public:
    User(const std::string &name,const std::string &password);
    static std::string encryption(const std::string &password);
    static std::string decryption(const std::string &password);
};

class NormalUser:public User{
private:
    int number;
public:
    NormalUser(const std::string &name,const std::string &password);
};

class Administrator:public User{
public:
    Administrator(const std::string &name,const std::string &password);
};

class InvalidUserException:public std::runtime_error{
public:
    InvalidUserException(int place): std::runtime_error(place==0?
    "User name length must be between 4 to 8 characters!":
    "User password length must be between 4 to 8 characters and includes letter and number!"){
    }
};
#endif //BANKAPP_USER_H
