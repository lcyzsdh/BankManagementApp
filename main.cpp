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
#include<map>
using namespace std;
using namespace std::rel_ops;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int fmlen=0;
static int searchFM(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        fmlen++;
        printf("|  %s  |",  argv[i] ? argv[i] : "NULL");
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
FinancialManagement QueryFM(int k,sqlite3* db) {
    const char* queryDataSQL = "SELECT * FROM FM;";int kt=0;
    sqlite3_stmt* statement;
    if (sqlite3_prepare_v2(db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            kt++;
            if(kt==k){
                string id(reinterpret_cast<const char*>(sqlite3_column_text(statement,0)));
                string begin(reinterpret_cast<const char*>(sqlite3_column_text(statement,1)));
                string end(reinterpret_cast<const char*>(sqlite3_column_text(statement,2)));
                double rate=sqlite3_column_double(statement,3);
                double risk=sqlite3_column_double(statement,4);
                double least=sqlite3_column_double(statement,5);
                stringstream ss(begin);int y,m,d;ss>>y>>m>>d;Date dbe(y,m,d);
                stringstream s2(end);s2>>y>>m>>d;Date den(y,m,d);
                FinancialManagement fm(id,dbe,den,rate,least,risk);
                sqlite3_finalize(statement);
                return fm;
            }

        }

    }
}
class Controller{
private:
    Date date;

    map<string,Account*> accounts;
    bool end;
public:
    Controller(const Date &date): date(date),end(false){
    }
    ~Controller();
    const Date &getDate()const{return date;}
    bool isEnd()const{return end;}
    bool runCommand(const string &cmdLine,sqlite3* db,User* iUser);
    void buyFM(const string &cmdLine,sqlite3* db,User* iUser,ofstream& fileout);
};
Controller::~Controller() {
}
void Controller::buyFM(const std::string &cmdLine, sqlite3 *db, User *iUser,ofstream& fileout) {
    string cms;int bnum;double amount;
    cout<<"-------------以下列出可以购买的理财：------------"<<endl;char* msg;
    cout<<"| 名称 ｜ 开始时间 ｜ 结束时间 ｜ 利率 ｜ 最低购买额 ｜ 风险度 ｜"<<endl;
    sqlite3_exec(db,"SELECT * FROM FM",searchFM, nullptr,&msg);
    cout<<"请输入要购买的理财编号(输入 -1 退出)>>";cin>>cms;
    if(cms=="-1"){
        return;
    }else{
        cout<<"请输入购买此理财的金额>>";
        cin>>amount;
        bnum= stoi(cms);
        if(bnum>(fmlen/6)){
            cout<<"当前编号不存在！"<<endl;
            return;
        }
        else{
            bool flag= false;
            for(const auto &i:accounts){
                if(i.second->isFMAccount()){
                    if(i.second->buyFM(QueryFM(bnum, db), amount)){
                        i.second->withdraw(date,amount,"buy");
                        cout<<"购买理财成功！"<<endl;
                        fileout<<"k "<<i.second->getId()<<" "<<amount<<" fm"+cms<<endl;

                    }else{
                        cout<<"购买失败，请确认您的购买金额超过最低购买金额，并且账户最高可承受风险高于理财风险!"<<endl;
                    }
                    flag=true;
                    return;
                }
            }
            if(!flag){cout<<"请先建立一个理财账户！"<<endl;}
            return;
        }
    }
}
bool Controller::runCommand(const string &cmdLine,sqlite3* db,User* iUser) {
    istringstream str(cmdLine);
    bool flag= false;map<string ,Account*>::iterator iter;
    char cmd,type;
    int index,day,bk=0,bnum,kk=0;
    double amount,credit,rate,fee,realAmount,risk;
    string id,desc,destID,fromID,cms;
    Account *account;
    Date date1,date2;
    str>>cmd;
    switch (cmd) {
        case 'a':
            str>>type>>id;
            for(iter=accounts.begin();iter!=accounts.end();iter++){
                if(iter->first==id){
                    cout<<"您要创建的账户已存在！"<<endl;
                    return false;
                }
            }
            if(type=='s'){
                str>>rate;
                account=new SavingAccount(date,id,rate);
            }
            else if(type=='c'){
                str>>credit>>rate>>fee;
                account=new CreditAccount(date,id,credit,rate,fee);
            }else{
                str>>risk;
                account=new FinancialAccount(date,id,risk);
            }
            accounts.insert(make_pair(account->getId(),account));
            return true;
        case 'd':
            str>>id>>amount;
            getline(str,desc);
            accounts[id]->deposit(date,amount,desc);
            return true;
        case 'k':
            str>>id>>amount;
            getline(str,desc);
            bnum=stoi(desc.substr(3));
            accounts[id]->withdraw(date,amount,desc);
            accounts[id]->buyFM(QueryFM(bnum,db),amount);
            return false;
        case 'w':
            str>>id>>amount;
            getline(str,desc);
            if(accounts[id]->withdraw(date,amount,desc))
            return true;
            else return false;
        case 's':
            cout<<"--------"<<"下面是您目前账号的所有资产状况：--------------"<<endl;
            for(const auto &i:accounts){
                cout<<"["<<++kk<<']';i.second->show(cout);cout<<endl;
            }
            return false;
        case 'c':
            str>>day;
            if(day<date.getDay()){
                cout<<"你不能指定之前的日期！";
                return false;
            } else if(day>date.getMonthDay()){
                cout<<"非法的日期！";
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
                paccount.second->settle(date);
            }
            return true;
        case 'q':
            str>>date1>>date2;
            Account::query(date1,date2);
            return false;
        case 'z':
            str>>fromID>>destID>>amount;
            if(iUser->isVIP()){
                realAmount=amount;
            }else{
                realAmount=amount+amount*0.05;//手续费
            }
            if(accounts[fromID]->getBalance()<realAmount){
                cout<<"无法转账！"<<endl;
                return false;
            }
            else{
                accounts[fromID]->withdraw(date,realAmount,"exchange to another account");
                accounts[destID]->deposit(date,amount,"get exchange");
                return true;
            }
        case 'e':
            end=true;
            return false;
    }
    cout<<"非法的操作符: "<<cmdLine<<endl;
    return false;
}

string load(int num,sqlite3* db){

    if(num==3){
        return "-2";
    }//三次尝试错误退出
    welcome();
    string USER_FILE_NAME="user_list.txt";
    string c;cin>>c;
    while(c!="1"&&c!="2"&&c!="e"){
        cout<<"非法的操作符，请重新输入！！"<<endl;
        welcome();
        cin>>c;
    }

    if(c=="1"){//登录功能
            string iName,iPassword;
            cout<<"请输入账号名称>>";cin>>iName;
            cout<<"请输入账号密码>>";cin>>iPassword;
            string re= QueryData(iName,User::encryption(iPassword),db);
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

        if(QueryData(iName,User::encryption(iPassword),db)=="-1"){
            cout<<"创建 "<<iName<<" 账户成功！"<<endl;
            char* msg= nullptr;
            string sql="INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('"+iName+"','"+User::encryption(iPassword)+"','Normal');";
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

void mainWork(User* iUser,sqlite3* db){
    vector<string> iInfo;
    cout << "------------欢迎回来: "<<iUser->getInfo(iInfo)[1] <<' '<<iInfo[0]<<"---------------"<< endl;

    Date beginDate(2020,1,1);
    UserScreen();

    Controller controller(beginDate);
    string cmdLine;

    string FILE_NAME=  iInfo[1] + "_commands.txt";
    ifstream fileIn(FILE_NAME);
    if(fileIn){
        while(getline(fileIn,cmdLine)){
            try {
                if(!cmdLine.empty()){
                    controller.runCommand(cmdLine,db,iUser);
                }
            }catch (exception &e){
                cout<<"Bad line in"<<FILE_NAME<<":"<<cmdLine<<endl;
                cout<<"Error: "<<e.what()<<endl;
                exit(1);
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
                if(cmd=="b"){
                    controller.buyFM(cmd,db,iUser,fileOut);
                }
                else if(controller.runCommand(cmd,db,iUser)){
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
void administration(User* iUser,sqlite3* db){
    vector<string> iInfo;
    cout << "--------欢迎回来: "<<iUser->getInfo(iInfo)[1] <<' '<<iInfo[0]<<"-------------"<< endl;
    AdminScreen();cout<<">>";
    string c;
    while(cin>>c){
        if(c=="e"){
            break;
        }
        if(c=="a"){
            string info;
            getline(cin,info);
            istringstream ss(info);
            string name,y1,m1,d1,y2,m2,d2,rate,la,risk;
            ss>>name>>y1>>m1>>d1>>y2>>m2>>d2>>rate>>la>>risk;
            Date begin(stoi(y1),stoi(m1),stoi(d1));Date end(stoi(y2),stoi(m2),stoi(d2));
            FinancialManagement fm1(name,begin,end, stod(rate), stod(la), stod(risk));
            fm1.addtoFM(db);
        }
        else if(c=="i"){
            string id;int n;char* ms;
            cin>>id>>n;
            string sql = "UPDATE USER set STARS = "+ to_string(n)+" where NAME='"+id+"'; "\
            "SELECT * FROM USER ";
            sqlite3_exec(db,sql.c_str(), callback, nullptr,&ms);
            cout<<"更新用户数据成功！"<<endl;
        }
        AdminScreen();cout<<">>";
    }
    return;
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
         "STARS INT);";//创建用户数据表
    char *fm_table="CREATE TABLE FM("  \
         "NAME TEXT PRIMARY KEY  NOT NULL," \
         "BEGIN       TEXT     NOT NULL," \
         "END         TEXT   NOT NULL,"\
         "RATE         REAL   NOT NULL,"\
         "RISK         REAL   NOT NULL,"\
         "LEAST        REAL NOT NULL);";//创建理财数据表
    //创建示例用户,在测试中使用
    rc=sqlite3_exec(db,user_table, callback, nullptr, &msg);
    rc=sqlite3_exec(db,"INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('lcyzsdh',2613,'Normal');", callback, nullptr, &msg);
    rc=sqlite3_exec(db,"INSERT INTO USER (NAME,PASSWORD,IDENTITY)"\
    "VALUES('ad',333,'Administrator');", callback, nullptr, &msg);

    rc= sqlite3_exec(db,fm_table,callback, nullptr,&msg);
    rc= sqlite3_exec(db,"INSERT INTO FM (NAME,BEGIN,END,RATE,RISK,LEAST)"\
    "VALUES('NEW1','2025-1-1','2025-3-1',0.05,0.5,10000);",callback, nullptr,&msg);

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
        administration(us,db);
    }
    else{
        us=new NormalUser(u.substr(1));
        mainWork(us,db);
    }

    sqlite3_close(db);
    return 0;
}