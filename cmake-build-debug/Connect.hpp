#include "BasicElements.hpp"
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <vector>
#include "UserRelated.hpp"
#include "TrainRelated.hpp"
#include "OrderRelated.hpp"
using namespace std;

#ifndef TICKETSYSTEM_2021_MAIN_CONNECT_HPP
#define TICKETSYSTEM_2021_MAIN_CONNECT_HPP

class Connector{
public:
    Users cuser{this};
    Trains ctrain{this};
    Orders corder{this};
//    File<User> USER;


    Connector(){}
    void run();
    const char* clean();
    const char* exit();
    int split(string str, char c, string *&ans);
	int read_int(string str);
	Time read_time(string str);
	Date read_date(string str);


    const char* add_user(string str);
    const char* login(string str);
    const char* logout(string str);
    const char* query_profile(string str);
    const char * modify_profile(string str);
    const char * add_train(string str);
    const char * release_train(string str);
    const char * query_train(string str);
    const char * delete_train(string str);
    const char * query_ticket(string str);
    const char * query_transfer(string str);
    const char * buy_ticket(string str);
    const char * query_order(string str);
    const char * refund_ticket(string str);

};




#endif //TICKETSYSTEM_2021_MAIN_CONNECT_HPP
