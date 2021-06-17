#include "BasicElements.hpp"
#include "Bpt.hpp"
#include "File_oi.hpp"
#include <cstring>
using namespace std;

#ifndef TICKETSYSTEM_2021_MAIN_USERRELATED_HPP
#define TICKETSYSTEM_2021_MAIN_USERRELATED_HPP

class Connector;
class User{
public:
    char username[21],password[31],name[21],mailAddr[31];
    int privilege;//权限
    int ordernum;//指令总数

    User(){privilege=ordernum=0;}

    User(const char* u,const char* p,const char* n,const char* m,const int g=0,const int o=0):
            privilege(g),ordernum(o){
        strcpy(username,u),strcpy(password,p),strcpy(name,n),strcpy(mailAddr,m);
    }

    ~User(){}
};

class Users{
public:
    BPlusTree<pair<int,int>,int> bpt_user{"Bpt_User.dat"};

    File<User> USER{"USER.dat"};

    Connector *connect;

    Users(Connector* connect_){connect=connect_;}
    char* print_user(User user);

    const char* add_user(const char* c,const char* u,const char* p,const char* n,const char* m,const int g);

    const char* login(const char* u,const char* p);

    const char* logout(const char* u);

    const char* query_profile(const char* c,const char* u);

    const char* modify_profile(const char* c,const char* u,const char* p,const char* n,const char* m,const int g);

    const char* clear();

};


#endif //TICKETSYSTEM_2021_MAIN_USERRELATED_HPP