//
// Created by 陆辰暘 on 2024/4/23.
//
#include "account.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "user.h"
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace std::rel_ops;
struct deleter{
    template<class T>void operator()(T* p){delete p;}
};

class Controller{
private:
    Date date;
    vector<Account *> accounts;
    vector<FinancialManagement*> fmList;
    bool end;
public:
    Controller(const Date &date): date(date),end(false){
        ifstream fileIn("fmList.txt");string info;FinancialManagement* fm;
        if(fileIn){
            while(getline(fileIn,info)){
                istringstream ss(info);
                string name,y1,m1,d1,y2,m2,d2,rate,la,risk;
                ss>>name>>y1>>m1>>d1>>y2>>m2>>d2>>rate>>la>>risk;
                Date begin(stoi(y1),stoi(m1),stoi(d1));Date end(stoi(y2),stoi(m2),stoi(d2));
                fm=new FinancialManagement(name,begin,end, stod(rate), stod(la), stod(risk));
                fmList.push_back(fm);
            }
            fileIn.close();
        }
    }
    ~Controller();
    const Date &getDate()const{return date;}
    bool isEnd()const{return end;}
    bool runCommand(const string &cmdLine);
};
Controller::~Controller() {
    for_each(accounts.begin(),accounts.end(),deleter());
    for_each(fmList.begin(),fmList.end(),deleter());
}
bool Controller::runCommand(const string &cmdLine) {
    istringstream str(cmdLine);
    char cmd,type;
    int index,day,bk=0,bnum,destID,fromID;
    double amount,credit,rate,fee,realAmount;
    string id,desc,cms;
    Account *account;
    Date date1,date2;
    str>>cmd;
    switch (cmd) {
        case 'a':
            str>>type>>id;
            if(type=='s'){
                str>>rate;
                account=new SavingAccount(date,id,rate);
            }
            else{
                str>>credit>>rate>>fee;
                account=new CreditAccount(date,id,credit,rate,fee);
            }
            accounts.push_back(account);
            return true;
        case 'd':
            str>>index>>amount;
            getline(str,desc);
            accounts[index]->deposit(date,amount,desc);
            return true;
        case 'w':
            str>>index>>amount;
            getline(str,desc);
            accounts[index]->withdraw(date,amount,desc);
            return true;
        case 's':
            for(int i=0;i<accounts.size();i++){
                cout<<"["<<i<<']';
                accounts[i]->show(cout);
                cout<<endl;
            }
            return false;
        case 'b':
            cout<<"These are the financial management list you can buy:"<<endl;
            for (int i = 0; i < fmList.size(); ++i) {
                if(fmList[i]->getBeginDate()>date){
                    cout<<"("<<i+1<<") ";fmList[i]->show();bk++;
                }
            }
            cout<<"buy(enter the number) or return r>";cin>>cms;
            if(cms=="r"){
                return false;
            }else{
                bnum= stoi(cms);
                if(bnum>bk){
                    cout<<"The fm isn't existing."<<endl;
                    return false;
                }
                else{
                    account[0].withdraw(date,100,"buy fm");//TODO:correct the real command,using withdraw(desc)to buy a fm
                }
            }
        case 'c':
            str>>day;
            if(day<date.getDay()){
                cout<<"You cannot specify a previous day";
                return false;
            } else if(day>date.getMonthDay()){
                cout<<"Invalid day";
                return false;
            }
            else{
                date=Date(date.getYear(),date.getMonth(),day);
            }
            return true;
        case 'n':
            if(date.getMonth()==12){
                date=Date(date.getYear()+1,1,1);
            }
            else{
                date=Date(date.getYear(),date.getMonth()+1,1);
            }
            for(auto &paccount:accounts){
                paccount->settle(date);
            }
            return true;
        case 'q':
            str>>date1>>date2;
            Account::query(date1,date2);
            return false;
        case 'z':
            str>>fromID>>destID>>amount;
            realAmount=amount+amount*0.05;//手续费
            if(accounts[fromID]->getBalance()<amount){
                cout<<"Unable to exchange!"<<endl;
                return false;
            }
            else{
                accounts[fromID]->withdraw(date,amount,"exchange to another account");
                accounts[destID]->deposit(date,amount,"get exchange");
                return true;
            }
        case 'e':
            end=true;
            return false;
    }
    cout<<"Invalid command: "<<cmdLine<<endl;
    return false;
}

string load(int num){
    if(num==3){
        return "-2";
    }
    cout<<"Welcome to bank management app!!!\nchoose (1)sign in(2)sign up(e)exit >";
    string USER_FILE_NAME="user_list.txt";
    string c;cin>>c;
    while(c!="1"&&c!="2"&&c!="e"){
        cout<<"Invalid command!"<<endl<<"Welcome to bank management app!!!\nchoose (1)sign in(2)sign up(e)exit >";
        cin>>c;
    }
    string info;
    ifstream userIn(USER_FILE_NAME);
    if(c=="1"){//登录功能
        if(userIn){
            bool flag= false;
            string iName,iPassword;
            cout<<"Please enter your name>";cin>>iName;
            cout<<"Please enter your password>";cin>>iPassword;
            string name,enPassword,identity;
            while(getline(userIn,info)){
                istringstream str(info);
                str>>name>>enPassword>>identity;
                string dePassword=User::decryption(enPassword);
                if(dePassword==iPassword&&iName==name){
                    flag=true;
                    break;
                }
            }
            if(flag){
                if(identity=="normal"){
                    return "N"+name;
                }
                else{//administrator
                    return "A"+name;
                }
            }
            else{
                cout<<"Wrong user name or passsword!!"<<endl;
                userIn.close();
                return load(++num);
            }
        }else{
            cout<<"Can't find user file!!"<<endl;
            return "-1";
        }
    }
    else if(c=="2"){//注册功能
        string iName,iPassword;
        cout<<"Please enter your name>";cin>>iName;
        cout<<"Please enter your password>";cin>>iPassword;
        int flag=1;
        if(userIn){
            while(getline(userIn,info)){
                istringstream str(info);
                string name,enPassword;
                str>>name>>enPassword;
                if(iName==name){
                    flag=0;
                    break;
                }
            }
        }
        else{
            cout<<"Can't find user file!!"<<endl;
            return "-1";
        }
        if(flag==1){
            cout<<"Create "<<iName<<" successfully."<<endl;
            ofstream fileOut(USER_FILE_NAME,ios_base::app);
            fileOut<<iName<<' '<<User::encryption(iPassword)<<" normal"<<endl;
            return "N"+iName;
        }
        else{
            cout<<"This user has been created!"<<endl;
            return load(num);
        }
    }
    else{
        return "0";
    }
}

void mainWork(User* iUser){
    vector<string> iInfo;
    cout << "Welcome back: "<<iUser->getInfo(iInfo)[1] << endl;

    Date beginDate(2020,1,1);
    cout<<"(a)add account (d)deposit (w)withdraw (s)show (b)buy financial management (c)change day (n)next month (q)query (z)exchange (e)exit"<<endl;

    Controller controller(beginDate);
    string cmdLine;

    string FILE_NAME=  iInfo[1] + "_commands.txt";
    ifstream fileIn(FILE_NAME);
    if(fileIn){
        while(getline(fileIn,cmdLine)){
            try {
                if(!cmdLine.empty()){
                    controller.runCommand(cmdLine);
                }
            }catch (exception &e){
                cout<<"Bad line in"<<FILE_NAME<<":"<<cmdLine<<endl;
                cout<<"Error: "<<e.what()<<endl;
                //return 1;
            }
        }
        fileIn.close();
    }
    ofstream fileOut(FILE_NAME,ios_base::app);
    //cout<<"(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit"<<endl;
    bool flag=true;
    while(!controller.isEnd()){
        if(flag){
            cout<<controller.getDate()<<"\tTotal: "<<Account::getTotal()<<"\tcommand>";
        }
        string cmd;
        getline(cin,cmd);
        try{
            if(!cmd.empty()){
                flag= true;
                if(controller.runCommand(cmd)){
                    fileOut<<cmd<<endl;
                }
            }
            else{
                flag= false;
            }
        } catch (AccountException &e) {
            cout<<"Error(#"<<e.getAccount()->getId()<<"):"<<e.what()<<endl;
        } catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    fileOut.close();
}

int main(){
    srand(time(nullptr));
    string u=load(0);User* us;
    if(u=="-2"){
        cout<<"You have tried over three times!!To protect your safety,this program exits.";
        return 0;
    }
    else if(u=="0"||u=="-1"){
        return 0;
    }
    else if(u.find('A')==0){
        us=new Administrator(u.substr(1));
    }
    else{
        us=new NormalUser(u.substr(1));
    }
    mainWork(us);
    return 0;
}