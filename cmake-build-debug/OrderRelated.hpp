#include "TrainRelated.hpp"
#include "UserRelated.hpp"

using namespace std;

#ifndef TICKETSYSTEM_2021_MAIN_ORDERRELATED_HPP
#define TICKETSYSTEM_2021_MAIN_ORDERRELATED_HPP

class Connector;

class Order{
public:
    char status[10],username[25],train_ID[25];
    int id_user,id_train,d_day;
    Ticket ticket;
    Order(){}
    Order(const char* status_,const char* username_,int id_user_,const char* train_ID_,int d_day_,int id_train_,Ticket ticket_){
        strcpy(status,status_),strcpy(username,username_),strcpy(train_ID,train_ID_);
        id_train=id_train_,id_user=id_user_,d_day=d_day_;
        ticket=ticket_;
    }
};

class Orders{
public:
    Connector *connect;

    BPlusTree<pair<pair<int,int>,int>,int> use_order{"Use_Order.dat"};
    BPlusTree<pair<pair<pair<int,int>,int>,int>,int> bpt_order{"Bpt_Order.dat"};

    File<Order> USE_ORDER{"USE_ORDER_.dat"};
    File<Order> ORDER{"ORDER.dat"};
    File<Train> TRAIN{"TRAIN.dat"};
    File<Seat> SEAT{"SEAT.dat"};
    File<Station> STATION{"STATION.dat"};
    File<pair<pair<int,int>,int>>STA_TRAIN{"STA_TRAIN.dat"};
    File<User> USER{"USER.dat"};

    Hash hash;




//    void init(Connector* other);

    Orders(Connector* connect_){connect=connect_;}

    const char* buy_ticket(const char* u,const char* id,Date date,int n,const char* from,const char* to,const char* q);

    const char* query_order(const char *u);

    const char* refund_ticket(const char* u,int n);

};

#endif //TICKETSYSTEM_2021_MAIN_ORDERRELATED_HPP
