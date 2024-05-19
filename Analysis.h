//
// Created by 陆辰暘 on 2024/5/16.
//

#ifndef BANKAPP_ANALYSIS_H
#define BANKAPP_ANALYSIS_H
#include "account.h"
#include <cstring>
enum Classified{
    DAILY,MONTHLY,ANNUALLY
};

class Analysis {
private:
    const Account* acc;
    std::string name;
public:
    Analysis(Account* acc);
    void show(Classified cmd);
};


#endif //BANKAPP_ANALYSIS_H
