#include "Connect.hpp"
#include <assert.h>
#include "Include.hpp"
#include "UserRelated.hpp"
#include "TrainRelated.hpp"
#include "OrderRelated.hpp"
#include "BasicElements.hpp"

using namespace std;

//argv:×Ö·û´®”µ½M
//argc:…¢”µ”µÁ¿

//basic functions:

//void Connector::init() {
//    cuser.init();
//    ctrain.init();
//    corder.init();
//}


void Connector::run() {

    string str;
    char ans[8200];

    int no=0;
    while (1){
        getline(cin,str);
        no++;
        cout<<"---------------No. "<<no<<"----------------"<<endl;

//        cout<<str<<endl;

        if( str.substr(0, 8) == "add_user" ) {
//            puts("{add_user}");
            strcpy(ans, add_user(str));
        }
        else if( str.substr(0, 5) == "login" ) {
//            puts("{login}");
            strcpy(ans, login(str));
        }
        else if( str.substr(0, 6) == "logout" ){
//            puts("{logout}");
            strcpy(ans, logout(str));
        }
        else if( str.substr(0, 13) == "query_profile" ) {
//            puts("{query_profile}");
            strcpy(ans, query_profile(str));
        }
        else if( str.substr(0, 14) == "modify_profile" ){
//            puts("{modify_profile}");
            strcpy(ans, modify_profile(str));

        }
        else if( str.substr(0, 9) == "add_train" ){
//            puts("{add_train}");
            strcpy(ans, add_train(str));
        }
        else if( str.substr(0, 13) == "release_train" ){
//            cout<<"{release}"<<endl;
            strcpy(ans, release_train(str));
        }
        else if( str.substr(0, 11) == "query_train" ) {
//            puts("{query_train}");
            strcpy(ans, query_train(str));
//            printf("{%s}",ans);
        }
        else if( str.substr(0, 12) == "delete_train" ) {
//            puts("{delete_train}");
            strcpy(ans, delete_train(str));
        }
        else if( str.substr(0, 12) == "query_ticket" ) {
//            puts("{query_ticket}");
            strcpy(ans, query_ticket(str));
        }
        else if( str.substr(0, 14) == "query_transfer" ) {
//            puts("{query_transfer}");
            strcpy(ans, query_transfer(str));
        }
        else if( str.substr(0, 10) == "buy_ticket" ) {
//            puts("{buy_ticket}");
            strcpy(ans, buy_ticket(str));
        }
        else if( str.substr(0, 11) == "query_order" ) {
//            puts("{query_order}");
            strcpy(ans, query_order(str));
        }
        else if( str.substr(0, 13) == "refund_ticket" ) {
//            puts("{refund_ticket}");
            strcpy(ans, refund_ticket(str));
        }
        else if( str.substr(0, 5) == "clean" ) {
//            puts("{clean}");
            strcpy(ans, clean());
        }
        else if( str.substr(0, 4) == "exit" ){
//            puts("{exit}");
            strcpy(ans, Exit());
            break;
        }
        printf("%s\n", ans);
    }
}

const char* Connector::clean(){
    cuser.bpt_user.Clean();
    ctrain.bpt_station.Clean();
    ctrain.bpt_seat.Clean();
    ctrain.bpt_station.Clean();
    ctrain.bpt_sta_train.Clean();
    ctrain.bpt_train.Clean();
    corder.use_order.Clean();
    corder.bpt_order.Clean();
    return "0";
}

const char* Connector::Exit() {
    printf("%s\n", "bye");
    exit(0);
    return "0";
}

int Connector::split(string str, char c, string *&ans) {
    if(c==' '){
        int cnt=0;
        stringstream ss(str);//??????????????????????
        string tmp;
        while(ss>>tmp)cnt++;
        ans=new string [cnt];
        ss=stringstream(str);
        for(int i=0;i<cnt;++i)ss>>ans[i];
        return cnt;
    }else {
        int cnt=1;
        for(int i=0;i<(int)str.length();++i){
            if(str[i]==c)cnt++;
        }
        ans=new string[cnt];
        int tmp=0;
        for(int i=0;i<(int)str.length();++i){
            if(str[i] == c) tmp++;
            else ans[tmp].push_back( str[i] );
        }
        return cnt;
    }
}

int Connector::read_int(string str) {
    int ans=0;
    for(int i=0;i<(int)str.length();++i)ans=ans*10+(str[i]-'0');
    return ans;
}

Time Connector::read_time(string str) {
    int h=(str[0]-'0')*10+str[1]-'0';
    int m=(str[3]-'0')*10+str[4]-'0';
    return Time(Date(),h,m);
}

Date Connector::read_date(string str) {
    int m=(str[0]-'0')*10+str[1]-'0';
    int d=(str[3]-'0')*10+str[4]-'0';
    return Date(m,d);
}

//normal functions:
const char* Connector::add_user(string str) {
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string c, u, p, n, m;
    int g;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-c") c = argv[i + 1];
        else if(argv[i] == "-u") u = argv[i + 1];
        else if(argv[i] == "-p") p = argv[i + 1];
        else if(argv[i] == "-n") n = argv[i + 1];
        else if(argv[i] == "-m") m = argv[i + 1];
        else if(argv[i] == "-g") g = read_int(argv[i + 1]);
        else assert(0);

    }
    delete []argv;
    return cuser.add_user(c.c_str(), u.c_str(), p.c_str(), n.c_str(), m.c_str(), g);
}

const char* Connector::login(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string u,p;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-u")u=argv[i+1];
        else if(argv[i]=="-p")p=argv[i+1];
        else assert(0);
    }
    delete []argv;
    return Connector::cuser.login(u.c_str(),p.c_str());
}

const char* Connector::logout(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string u;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-u")u=argv[i+1];
        else assert(0);
    }
    delete []argv;
    return Connector::cuser.logout(u.c_str());
}

const char* Connector::query_profile(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string u,c;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-c")c=argv[i+1];
        else if(argv[i]=="-u")u=argv[i+1];
        else assert(0);
    }
    delete []argv;
//    puts("there!");
    return Connector::cuser.query_profile(c.c_str(), u.c_str());
}

const char* Connector::modify_profile(string str) {
    string* argv = nullptr;
    int argc = split(str, ' ', argv);
    string c, u, p, n, m;
    int g = -1;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-c") c = argv[i + 1];
        else if(argv[i] == "-u") u = argv[i + 1];
        else if(argv[i] == "-p") p = argv[i + 1];
        else if(argv[i] == "-n") n = argv[i + 1];
        else if(argv[i] == "-m") m = argv[i + 1];
        else if(argv[i] == "-g") g = read_int(argv[i + 1]);
        else assert(0);
    }
    delete []argv;
    return Connector::cuser.modify_profile(c.c_str(), u.c_str(), p.c_str(), n.c_str(), m.c_str(), g);
}

const char* Connector::add_train(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string i_,s,p,t,o,d,y;
    int m,n;
    Time tim;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-i")i_=argv[i+1];
        else if(argv[i] == "-s") s = argv[i + 1];
        else if(argv[i] == "-p") p = argv[i + 1];
        else if(argv[i] == "-t") t = argv[i + 1];
        else if(argv[i] == "-o") o = argv[i + 1];
        else if(argv[i] == "-d") d = argv[i + 1];
        else if(argv[i] == "-y") y = argv[i + 1];
        else if(argv[i] == "-n") n = read_int(argv[i + 1]);
        else if(argv[i] == "-m") m = read_int(argv[i + 1]);
        else if(argv[i] == "-x") tim = read_time(argv[i + 1]);
        else assert(0);
    }

    Date begin=read_date(d.substr(0,5));//½ØÈ¡dÄ0-5
    Date end=read_date(d.substr(6,5));
    char stations[105][41];
    int price[105],traveltime[105],stopovertime[105];

    string* tmp=nullptr;
    int cnt=split(s,'|',tmp);
    for(int i=0;i<cnt;++i)strcpy(stations[i],tmp[i].c_str());
    delete []tmp;

    cnt=split(p,'|',tmp);
    for(int i=0;i<cnt;i++)price[i]=read_int(tmp[i]);
    delete []tmp;

    cnt=split(t,'|',tmp);
    for(int i=0;i<cnt;i++)traveltime[i]=read_int(tmp[i]);
    delete []tmp;

    if(n>2){
        cnt=split(o,'|',tmp);
        for(int i=0;i<cnt;i++)stopovertime[i]=read_int(tmp[i]);
        delete []tmp;
    }

    delete []argv;
    return Connector::ctrain.add_train(i_.c_str(),n,m,stations,price,tim,
                            traveltime,stopovertime,begin,end,y.c_str()[0]);
}

const char* Connector::release_train(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string i_;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-i")i_=argv[i+1];
        else assert(0);
    }
    delete [] argv;
//    cout<<"here!"<<endl;
    return ctrain.release_train(i_.c_str());
}

const char* Connector::query_train(string str) {
    string* argv=nullptr;
    int argc=split(str,' ',argv);
    string i_;
    Date d;
    for(int i=1;i<argc;i+=2){
        if(argv[i]=="-i")i_=argv[i+1];
        else if(argv[i]=="-d")d=read_date(argv[i+1]);
        else assert(0);
    }
    delete []argv;
    return Connector::ctrain.query_train(i_.c_str(),d);
}

const char* Connector::delete_train(string str) {
    string *argv=nullptr;
    int argc=split(str,' ',argv);
    string i_;
    for(int i= 1; i < argc; i += 2 ) {
        if(argv[i] == "-i") i_ = argv[i + 1];
        else assert(0);
    }
    delete []argv;
    return Connector::ctrain.delete_train(i_.c_str());
}

const char* Connector :: query_ticket(string str){
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string s, t, p = "time";
    Date d;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-s") s = argv[i + 1];
        else if(argv[i] == "-t") t = argv[i + 1];
        else if(argv[i] == "-p") p = argv[i + 1];
        else if(argv[i] == "-d") d = read_date(argv[i + 1]);
        else assert(0);
    }
    delete []argv;
    return Connector::ctrain.query_ticket(d,s.c_str(), t.c_str(), p.c_str());
}

const char* Connector :: query_transfer(string str){
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string s, t, p = "time";
    Date d;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-s") s = argv[i + 1];
        else if(argv[i] == "-t") t = argv[i + 1];
        else if(argv[i] == "-p") p = argv[i + 1];
        else if(argv[i] == "-d") d = read_date(argv[i + 1]);
        else assert(0);
    }
    delete []argv;
    return Connector::ctrain.query_transfer(d,s.c_str(), t.c_str(), p.c_str());
}

const char* Connector :: buy_ticket(string str){
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string u, ii, f, t, q = "false";
    Date d;
    int n;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-u") u = argv[i + 1];
        else if(argv[i] == "-i") ii = argv[i + 1];
        else if(argv[i] == "-f") f = argv[i + 1];
        else if(argv[i] == "-t") t = argv[i + 1];
        else if(argv[i] == "-q") q = argv[i + 1];
        else if(argv[i] == "-d") d = read_date(argv[i + 1]);
        else if(argv[i] == "-n") n = read_int(argv[i + 1]);
        else assert(0);
    }
    delete []argv;
    return Connector::corder.buy_ticket(u.c_str(), ii.c_str(), d, n, f.c_str(), t.c_str(), q.c_str());
}

const char* Connector :: query_order(string str){
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string u;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-u") u = argv[i + 1];
        else assert(0);
    }
    delete []argv;
    return Connector::corder.query_order(u.c_str());
}

const char* Connector :: refund_ticket(string str){
    string *argv = nullptr;
    int argc = split(str, ' ', argv);
    string u;
    int n = 1;
    for(int i = 1; i < argc; i += 2 ) {
        if(argv[i] == "-u") u = argv[i + 1];
        else if(argv[i] == "-n") n = read_int(argv[i + 1]);
        else assert(0);
    }
    delete []argv;
    return Connector::corder.refund_ticket(u.c_str(), n);
}