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
    vector< int > user_list;
    bpt_user.Find_(make_pair(-1,-1),make_pair(19260817,19491001),user_list);
    if(user_list.empty()){//先判u，首次insert时c不存在！
        User newuser(u,p,n,m,10);
        int pos=USER.Newpos();
        USER.Write(newuser,pos);
        bpt_user.Insert(make_pair(hash_u,pos));
        return "0";
    }
    else {//非首次Insert
        if(!bpt_user.Exist(hash_c))return "-1";
        int pos=bpt_user.Find(hash_c);
        User curuser;
        USER.Read(curuser,pos);
        if(!curuser.logstate||curuser.privilege<=g)return "-1";
        else {//success Insert
            User newuser(u,p,n,m,g);
            int newpos=USER.Newpos();
            USER.Write(newuser,newpos);
            bpt_user.Insert(make_pair(hash_u,newpos));
            return "0";
        }
    }
//    return "-1";
}

const char* Users::login(const char* u,const char* p){
    pair<int,int> hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u)){return "-1";}
    int curpos=bpt_user.Find(hash_u);
    User curuser;
    USER.Read(curuser,curpos);
    cout << curpos << " " << curuser.username << endl;
//    cout << "loginstate = " << curuser.logstate << " " << curuser.privilege << endl;
    if(curuser.logstate||strcmp(curuser.password,p))return "-1";
    else {//success login
        curuser.logstate = 1;
        USER.Write(curuser,curpos);
        int aaa=bpt_user.Find(hash_u);
        User bbb;
        USER.Read(bbb,aaa);
//        cout<<bbb.username<<" status: "<<bbb.logstate<<endl;
        return "0";
    }
}

const char* Users::logout(const char *u){
    pair<int,int> hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u))return "-1";
    int curpos=bpt_user.Find(hash_u);
    User curuser;
    USER.Read(curuser,curpos);
    if(!curuser.logstate)return "-1";
    else {//success logout
        curuser.logstate=0;
        USER.Write(curuser,curpos);
        return "0";
    }
}

const char* Users::query_profile(const char* c,const char* u){
    pair<int,int> hash_c=Hash().hash_it(c),hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u)||!bpt_user.Exist(hash_c)){cout<<"query_profile---sb not exist!  ";return "-1";}
    int curpos=bpt_user.Find(hash_c),findpos=bpt_user.Find(hash_u);
    User curuser,finduser;
    USER.Read(curuser,curpos),USER.Read(finduser,findpos);
    if(curuser.logstate==0) { cout<<"logstate=0"<<endl; return "-1";}// 486 not this reason ???
    if(curuser.privilege<finduser.privilege)return "-1";
    if(curuser.privilege==finduser.privilege&&strcmp(curuser.username,finduser.username))return "-1";
    else {//success check
//        puts("---[query_profile]success---");
        return print_user(finduser);
    }
}

const char* Users::modify_profile(const char* c,const char* u,const char* p,const char* n,const char* m,const int g){
    pair<int,int> hash_c=Hash().hash_it(c),hash_u=Hash().hash_it(u);
    if(!bpt_user.Exist(hash_u)||!bpt_user.Exist(hash_c))return "-1";
    int curpos=bpt_user.Find(hash_c),findpos=bpt_user.Find(hash_u);
    User curuser,finduser;
    USER.Read(curuser,curpos),USER.Read(finduser,findpos);
    if(curuser.logstate&&g<curuser.privilege&&(curuser.privilege>finduser.privilege||!strcmp(curuser.username,finduser.username))){
        if(strcmp(n,""))strcpy(finduser.name,n);
        if(strcmp(p,""))strcpy(finduser.password,p);
        if(strcmp(m,""))strcpy(finduser.mailAddr,m);
        if(g!=-1)finduser.privilege=g;
        USER.Write(finduser,findpos);
        int aaa=bpt_user.Find(hash_u);
        User bbb;
        USER.Read(bbb,aaa);
//        cout<<"!!!!!!!!!!: "<<bbb.username<<' '<<bbb.mailAddr<<endl;

//        puts("---[modify_profile]success---");
        return print_user(finduser);
    }
    else return "-1";
}