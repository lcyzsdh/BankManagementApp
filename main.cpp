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
#include "db/sqlite3.h"
#include<cstdio>
#include "OutputUtils.h"
using namespace std;
using namespace std::rel_ops;
extern void welcome();
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// 查询数据
string QueryData(string s1,string s2,sqlite3* db) {
    const char* queryDataSQL = "SELECT * FROM USER;";
    sqlite3_stmt* statement;
    if (sqlite3_prepare_v2(db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            string t1(reinterpret_cast<const char*>(sqlite3_column_text(statement,0)));
            string t2(reinterpret_cast<const char*>(sqlite3_column_text(statement,1)));
            if(t1==s1&&
                    t2==s2){
                string t3(reinterpret_cast<const char*>(sqlite3_column_text(statement,2)));
                return t3;
            }
        }
        sqlite3_finalize(statement);
        return "-1";
    }
}

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

string load(int num,sqlite3* db){

    if(num==3){
        return "-2";
    }
    welcome();
    string USER_FILE_NAME="user_list.txt";
    string c;cin>>c;
    while(c!="1"&&c!="2"&&c!="e"){
        cout<<"非法的操作符，请重新输入！！"<<endl;
        welcome();
        cin>>c;
    }
    string info;

    if(c=="1"){//登录功能
            string iName,iPassword;
            cout<<"请输入账号名称>>";cin>>iName;
            cout<<"请输入账号密码>>";cin>>iPassword;
            string re= QueryData(iName,iPassword,db);
            if(re!="-1"){
                return re[0]+iName;
            }
            else{
                cout<<"账号或密码有误！"<<endl;
                return load(++num,db);
            }
    }
    else if(c=="2"){//注册功能
        string iName,iPassword;
        cout<<"请输入您要注册的账号名称>>";cin>>iName;
        cout<<"请输入您的密码>>";cin>>iPassword;

        if(QueryData(iName,iPassword,db)=="-1"){
            cout<<"创建 "<<iName<<" 账户成功！"<<endl;
            char* msg= nullptr;
            string sql="INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('"+iName+"','"+iPassword+"','Normal');";
            sqlite3_exec(db,sql.c_str(), callback, nullptr, &msg);
            return "N"+iName;
        }
        else{
            cout<<"当前账户已经被注册，请重试！"<<endl;
            return load(num,db);
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

    //数据库连接操作
    sqlite3 *db;char* msg= nullptr;
    int rc;
    rc = sqlite3_open("user_list.sqlite", &db);
    char *user_table= "CREATE TABLE USER("  \
         "NAME TEXT PRIMARY KEY  NOT NULL," \
         "PASSWORD       TEXT     NOT NULL," \
         "IDENTITY         TEXT   NOT NULL,"\
         "STARS INT);";
    //创建示例用户
    rc=sqlite3_exec(db,user_table, callback, nullptr, &msg);

    rc=sqlite3_exec(db,"INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('lcyzsdh',2613,'Normal');", callback, nullptr, &msg);
    rc=sqlite3_exec(db,"INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('ad',333,'Administrator');", callback, nullptr, &msg);

    srand(time(nullptr));
    string u=load(0,db);User* us;
    if(u=="-2"){
        cout<<"您尝试3次账号或密码有误，为保护用户安全，程序将自动退出。";
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
    sqlite3_close(db);
    return 0;
}