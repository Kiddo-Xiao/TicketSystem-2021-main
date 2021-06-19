#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <iomanip>
using namespace std;

#ifndef TICKETSYSTEM_2021_MAIN_BASICELEMENTS_HPP
#define TICKETSYSTEM_2021_MAIN_BASICELEMENTS_HPP

//classes included : Hash & Date & Time & Tickrt & Seat & Station

class Hash{
public:
    const int p1=233,p2=2333,mod1 = 19260817, mod2 = 19491001;
    Hash(){}
    pair<int,int> hash_it(const char* c){
        int ans1=0,ans2=0,len=strlen(c);
        for(int i=0;i<len;++i){
            ans1=(int)(((long long)p1*ans1+c[i])%mod1);
            ans2=(int)(((long long)p2*ans2+c[i])%mod2);
        }
        return make_pair(ans1,ans2);
    }
};

const int monthday[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

class Date{
public:
    int month,day;
    Date(const int month_=1,const int day_=1):month(month_),day(day_){}


    Date operator+(const int& x){
        Date tmp=*this;
        tmp.day+=x;
        while (tmp.day>monthday[tmp.month]){
            tmp.day-=monthday[tmp.month];
            tmp.month++;
        }
        return tmp;
    }

    Date operator-(const int& x){
        Date tmp=*this;
        tmp.day-=x;
        while (tmp.day<1){
            tmp.month--;
            tmp.day+=monthday[tmp.month];
        }
        return tmp;
    }

    int operator-(Date other){
        int delta=0;
        if(month<other.month||(month==other.month&&day<other.day)){
            delta=other-(*this);
            return delta;
        }else {
            if(month==other.month) delta=day-other.day;
            else {
                for(int i=other.month;i<=month;++i){
                    if(i==other.month)delta+=monthday[other.month]-other.day;
                    else if(i==month)delta+=day;
                    else delta+=monthday[i];
                }
            }
        }
        return delta;
    }

    bool operator==(const Date& other){
        if(month==other.month&&day==other.day)return 1;
        return 0;
    }

    bool operator<(Date other){
        if(month!=other.month)return month<other.month ? 1:0;
        return day<other.day ? 1:0;
    }

    friend ostream&operator<<(ostream& os,Date date){
        os<<setw(2)<<setfill('0')<<date.month<<'-'<<setw(2)<<setfill('0')<<date.day;
        return os;
    }
};

class Time{
public:
    Date date;
    int hour,minute;

    Time(Date date_=Date(),int hour_=0,int minute_=0):date(date_),hour(hour_),minute(minute_){}

    Time operator+(const int& x){
        Time tmp=*this;
        tmp.minute+=x;
        tmp.hour+=tmp.minute/60,tmp.minute%=60;
        tmp.date=tmp.date+tmp.hour/24,tmp.hour%=24;
        return tmp;
    }

    Time operator-(const int& x){
        Time tmp=*this;
        if (minute>=x)tmp.minute-=x;
        else {
            int del=x-tmp.minute;
            tmp.minute=60-del%60;
            if(tmp.minute == 60) tmp.minute = 0;
            tmp.hour-=(1+ (del - 1)/60);
            if(tmp.hour<0){
                tmp.date=tmp.date-(1+(-tmp.hour - 1)/24);
                tmp.hour=24-((-tmp.hour)%24);
                if(tmp.hour == 24) tmp.hour = 0;
            }
        }
        return tmp;
    }

    bool operator==(const Time& other){
        if(date==other.date&&hour==other.hour&&minute==other.minute)return 1;
        return 0;
    }

    int operator-(const Time& other){
        return (date-other.date)*1440+(minute + 60 * hour) - (other.minute + 60 * other.hour);
    }

    bool operator<(Time other){
        if(!(other.date==date))return date<other.date ? 1:0;
        if(hour!=other.hour)return hour<other.hour ? 1:0;
        return minute<other.minute ? 1:0;
    }

    friend ostream & operator<<(ostream &os,Time time){
        os<<time.date<<' '<<setw(2)<<setfill('0')<<time.hour<<'-'<<setw(2)<<setfill('0')<<time.minute;
        return os;
    }
};

class Ticket{
public:
    char train[25],station_from[41],station_to[41];
    int price,seat;
    Time depart,arrive;

    Ticket(){}

    Ticket(const char* train_,const char* from,const char* to,
           const int price_,const int seat_,Time depart_,Time arrive_){
        strcpy(train,train_),strcpy(station_from,from),strcpy(station_to,to);
        price=price_,seat=seat_;
        depart=depart_,arrive=arrive_;
    }
};

class Seat{//以每辆列车为单位
public:
    int ordernum[920];//每日order数量
    int seat[95][101];//该车每日每站的余量
    Seat(int daynum=0,int stationnum=0,int seatnum=0){
        for(int i=0;i<daynum;++i){//第i天结束时
            ordernum[i]=0;
            for(int j=0;j<stationnum;++j){//第j站结束时
                seat[i][j]=seatnum;
            }
        }
//        cout<<daynum<<" " <<stationnum<<" "<< seatnum<<endl;
    }
//    bool operator < (const Seat & other) const{
//        int l = 95;
//        for(int i = 0; i < l; i++)
//            if(ordernum[i] != other.ordernum[i]) return ordernum[i] < other.ordernum[i];
//        for(int i = 0; i < l; i++)
//            for(int j = 0; j < l; j++)
//                if(seat[i][j] != other.seat[i][j]) return seat[i][j] < other.seat[i][j];
//
//    }
    int* operator[](const int &day) {return seat[day];}
};

class Station{
public:
    char stationname[41];
    int trainnum;
    Station(){trainnum=0;}
    Station(const char* sta_name){
        trainnum=0;
        strcpy(stationname,sta_name);
    }
};

#endif //TICKETSYSTEM_2021_MAIN_BASICELEMENTS_HPP
