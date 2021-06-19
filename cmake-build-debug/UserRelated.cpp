#include "UserRelated.hpp"
#include "Connect.hpp"

using namespace std;

char * Users :: print_user(User user){
    char* ans;
    char head[256];
    snprintf(head,256,"%s %s %s %d",user.username,user.name,user.mailAddr,user.privilege);
    ans=head;
    return ans;
}

const char* Users::add_user(const char* c,const char* u,const char* p,const char* n,const char* m, const int g){
    pair<int,int> hash_c=Hash().hash_it(c),hash_u=Hash().hash_it(u);
    if(bpt_user.Exist(hash_u))return "-1";
    if(bpt_user.Empty()){//先判u，首次insert时c不存在！
        User newuser(u,p,n,m,10);
        int pos=USER.Newpos();
        USER.Write(newuser,pos);
        bpt_user.Insert(make_pair(hash_u,pos));
        return "0";
    }
    else {//非首次Insert
//        cout<<"what fuck?"<<endl;
        if(!bpt_user.Exist(hash_c))return "-1";
        if(!connect->loguser.count(c))return "-1";
        if(connect->loguser.find(c)->second<=g)return "-1";
        //success Insert
        User newuser(u,p,n,m,g);
        int newpos=USER.Newpos();
        USER.Write(newuser,newpos);
        bpt_user.Insert(make_pair(hash_u,newpos));
        return "0";
    }
}

const char* Users::login(const char* u,const char* p){
    pair<int,int> hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u)){return "-1";}

    int curpos=bpt_user.Find(hash_u);
//    cerr<<"position : "<<curpos<<endl;

    User curuser;
    USER.Read(curuser,curpos);

//    //debug
//    string aa="NoirCorne";
//    int nnpos=connect->cuser.bpt_user.Find(connect->corder.hash.hash_it(aa.c_str()));
//    cout<<"position : "<<nnpos<<endl;
//    User nn;
//    USER.Read(nn,nnpos);
//    cout<<"NoirCorne now password : "<<nn.password<<endl;


    if(connect->loguser.count(u)){return "-1";}
    if(strcmp(curuser.password,p)){return "-1";}
    else {//success login
        connect->loguser.insert(make_pair(u,curuser.privilege));
        return "0";
    }
}

const char* Users::logout(const char *u){
    if(!connect->loguser.count(u))return "-1";
    else {//success logout
        connect->loguser.erase(u);
        return "0";
    }
}

const char* Users::query_profile(const char* c,const char* u){
    pair<int,int> hash_c=Hash().hash_it(c),hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u)||!bpt_user.Exist(hash_c)){return "-1";}
//    int curpos=bpt_user.Find(hash_c),findpos=bpt_user.Find(hash_u);
//    User curuser,finduser;
//    USER.Read(curuser,curpos),USER.Read(finduser,findpos);
    if(!connect->loguser.count(c)) { return "-1";}// 486 not this reason ???
    auto cpri=connect->loguser.find(c);
    int findpos=bpt_user.Find(hash_u);
    User finduser;
    USER.Read(finduser,findpos);
    if(cpri->second<finduser.privilege){return "-1";}
    if(cpri->second==finduser.privilege&&strcmp(cpri->first.c_str(),finduser.username)){return "-1";}

    else {//success check
//        puts("---[query_profile]success---");
        return print_user(finduser);
    }
}

const char* Users::modify_profile(const char* c,const char* u,const char* p,const char* n,const char* m,const int g) {
    pair<int, int> hash_c = Hash().hash_it(c), hash_u = Hash().hash_it(u);
    if (!bpt_user.Exist(hash_u) || !bpt_user.Exist(hash_c))return "-1";
    if (!connect->loguser.count(c))return "-1";
    auto cuser = connect->loguser.find(c);
    if(g>=cuser->second)return "-1";

    int findpos=bpt_user.Find(hash_u);
    User finduser;
    USER.Read(finduser,findpos);
    if (cuser->second > finduser.privilege || !strcmp(cuser->first.c_str(),finduser.username)){
        if (strcmp(n, "")) strcpy(finduser.name, n);
        if (strcmp(p, ""))strcpy(finduser.password, p);
        if (strcmp(m, ""))strcpy(finduser.mailAddr, m);
        if (g != -1)if(connect->loguser.count(u))connect->loguser.erase(u),connect->loguser.insert(make_pair(u,g));
        USER.Write(finduser, findpos);

        //debug:
        int aaa=bpt_user.Find(hash_u);
        User bbb;
        USER.Read(bbb,aaa);
//        cerr<<"NoirCorne password : "<<bbb.password<<endl;

//        puts("---[modify_profile]success---");
        return print_user(bbb);
    }
    else return "-1";
}