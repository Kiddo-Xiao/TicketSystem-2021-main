#include "Include.hpp"
#include "Bpt.hpp"


#ifndef TICKETSYSTEM_2021_MAIN_TRAINRELATED_HPP
#define TICKETSYSTEM_2021_MAIN_TRAINRELATED_HPP
using namespace std;

class Connector;
class Train{
//    friend class Trains;
public:
    char type,trainID[21],stations[101][41];
    int stationNum=0,seatNum=0;
    int prices[101],travelTimes[101],stopoverTimes[100];
    Time startTime;
    Date Date1,Date2;
    bool release=0;

public:

//    bool operator < (const Train & other) const{
//        int l = min(strlen(trainID), strlen(other.trainID));
//        for(int i = 0; i < l; i++)
//            if(trainID[i] != other.trainID[i]) return trainID[i] < other.trainID[i];
//        return strlen(trainID) < strlen(other.trainID);
//    }

    Train(){}
    Train(const char* trainid, int stationnum, int seatnum,
          const char stations_[][41], const int p[], Time starttime,
          const int traveltimes[], const int stopovertimes[],
          Date date1, Date date2, const char typ){
        strcpy(trainID,trainid);
        stationNum=stationnum,seatNum=seatnum,type=typ,startTime=starttime,Date1=date1,Date2=date2;
        release=prices[0]=travelTimes[0]=stopoverTimes[0]=stopoverTimes[stationNum-1]=0;
        for(int i=0;i<stationNum;++i){
            strcpy(stations[i],stations_[i]);
            if(i>0&&i<stationNum-1)stopoverTimes[i]=stopovertimes[i - 1];
            if(i + 1 < stationnum){
                prices[i + 1] = p[i] + prices[i];
                travelTimes[i + 1] = traveltimes[i] + stopoverTimes[i] + travelTimes[i];
            }
//            printf("%d  %d  %d\n", i, travelTimes[i], stopoverTimes[i]);
        }
    }
};


class Trains{
public:

    BPlusTree<pair<int,int>,int> bpt_train{"Bpt_Train.dat"};
    BPlusTree<pair<int,int>,int> bpt_seat{"Bpt_Seat.dat"};
    BPlusTree<pair<int,int>,int> bpt_station{"Bpt_Station.dat"};
    BPlusTree<pair<pair<int,int>,int>,int>bpt_sta_train{"Bpt_Sta_Train.dat"};

    File<Train> TRAIN{"TRAIN.dat"};
    File<Seat> SEAT{"SEAT.dat"};
    File<Station> STATION{"STATION.dat"};
    File<pair<pair<int,int>,int>>STA_TRAIN{"STA_TRAIN.dat"};

    Hash hash;
    Connector *connect;


    Trains(Connector* connect_){connect=connect_;}

//    void init(Connector *_connect);

    const char* add_train(const char* trainid, int stationnum, int seatnum,
                          const char stations_[][41], const int p[], Time starttime,
                          const int traveltimes[], const int stopovertimes[],
                          Date date1, Date date2, const char typ);

    const char* release_train(const char* trainid);

//    void print_train(Train t);
//
//    void print_station(Station s);

    const char* query_train(const char* trainid,Date date);

    const char* delete_train(const char* trainid);

    Ticket find_ticket(Date date,const char* from,const char* to,const char* op,const char *id,int hour=0,int minute=0);

    const char* query_ticket(Date date,const char* from,const char* to,const char* op);

    const char* query_transfer(Date date,const char* from,const char* to,const char* op);
};



#endif //TICKETSYSTEM_2021_MAIN_TRAINRELATED_HPP
