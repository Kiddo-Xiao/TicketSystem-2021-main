#include "TrainRelated.hpp"
#include "Connect.hpp"
#include "Include.hpp"



//void Trains :: init(){
//    bpt_train.init("bptrain_file", "bptrain_disk");
//    bpt_seat.init("bpseat_file", "bpseat_disk");
//    bpt_station.init("bpstation_file", "bpstation_disk");
//    bpt_sta_train.init("bpstrain_file", "bpstrain_disk");
//}



bool cmp_str(char *a, char *b){
    int la = strlen(a), lb = strlen(b);
    for(int i = 0; i < la && i < lb; i++)
        if(a[i] != b[i]) return a[i] < b[i];
    return la < lb;
}

bool cmp_time1(Ticket a,Ticket b){
    if((a.arrive-a.depart)!=(b.arrive-b.depart))return (a.arrive - a.depart) < (b.arrive - b.depart);
    else return cmp_str(a.train,b.train);
}

bool cmp_time2(Ticket a, Ticket b){
    if(!(a.arrive == b.arrive))return a.arrive < b.arrive;
    else return cmp_str(a.train, b.train);
}

bool cmp_cost(Ticket a, Ticket b){
    if(a.price != b.price) return a.price < b.price;
    else return strcmp(a.train, b.train) < 0;
}

const char* Trains::add_train(const char* trainid, int stationnum, int seatnum,
                      const char stations_[][41], const int p[], Time starttime,
                      const int traveltimes[], const int stopovertimes[],
                      Date date1, Date date2, const char typ) {
    pair<int,int> hash_id=hash.hash_it(trainid);
    if(bpt_train.Exist(hash_id))return "-1";
    else {//succeed
//        printf("1111111111111111111111 ::::::::: %d  %d\n", traveltimes[1], stopovertimes[1]);
        Train newtrain(trainid, stationnum, seatnum, stations_, p, starttime, traveltimes,
                       stopovertimes, date1, date2, typ);
        int newtrain_pos=TRAIN.Newpos();
        TRAIN.Write(newtrain,newtrain_pos);
        bpt_train.Insert(make_pair(hash_id,newtrain_pos));
        return "0";
    }
}
class Seat;
const char* Trains::release_train(const char *trainid) {
    bool jd = 0;
    if (trainid[0] == '!'){jd = 1; trainid++;}
    pair<int,int> hash_id=hash.hash_it(trainid);
    if(!bpt_train.Exist(hash_id))return "-1";
    int curtrain_pos=bpt_train.Find(hash_id);
    Train curtrain;
    TRAIN.Read(curtrain,curtrain_pos);
    if(curtrain.release)return "-1";
    if (jd) {return "not";}
    else {//succeed
        curtrain.release=1;
        TRAIN.Write(curtrain,curtrain_pos);
        int tmpt_pos=bpt_train.Find(hash_id);
        Train tmpt;
        TRAIN.Read(tmpt,tmpt_pos);
        Seat ss(curtrain.Date2-curtrain.Date1+1,curtrain.stationNum,curtrain.seatNum);
        int ss_pos=SEAT.Newpos();
        SEAT.Write(ss,ss_pos);
        bpt_seat.Insert(make_pair(hash_id,ss_pos));

        for(int i=0;i<curtrain.stationNum;++i){
            pair<int,int> hash_sta=hash.hash_it(curtrain.stations[i]);
            if(bpt_station.Exist(hash_sta)==0){
                int tmp_pos=STATION.Newpos();
                Station tmp(curtrain.stations[i]);
                STATION.Write(tmp,tmp_pos);
                bpt_station.Insert(make_pair(hash_sta,tmp_pos));
            }
            int sta_pos=bpt_station.Find(hash_sta);
            Station sta;
            STATION.Read(sta,sta_pos);
            sta.trainnum++;
//            cout<<sta.stationname<<" trainnum= "<<sta.trainnum<<endl;
            STATION.Write(sta,sta_pos);
            int sta_train_pos=STA_TRAIN.Newpos();
            pair<pair<int,int>,int> sta_train=make_pair(hash_id,i);
            STA_TRAIN.Write(sta_train,sta_train_pos);
            bpt_sta_train.Insert(make_pair(make_pair(hash_sta,sta.trainnum),sta_train_pos));
        }
        return "0";
    }
}

const char* Trains::query_train(const char *trainid, Date date) {
    pair<int,int> hash_id=hash.hash_it(trainid);
    if(!bpt_train.Exist(hash_id))return "-1";
    int tmp_pos=bpt_train.Find(hash_id);
    Train tmp;
    TRAIN.Read(tmp,tmp_pos);
    if(date<tmp.Date1||tmp.Date2<date)return "-1";
    int daycnt=date-tmp.Date1;
    int restseat_pos;
    Seat restseat;
    if(tmp.release){
        restseat_pos=bpt_seat.Find(hash_id);
        SEAT.Read(restseat,restseat_pos);
    }
    char head[8200];
    char body[130];
    snprintf(head,sizeof(head),"%s %c\n",tmp.trainID,tmp.type);
    snprintf(body,sizeof(body),"%s xx-xx xx:xx -> %02d-%02d %02d:%02d 0 %d\n",
             tmp.stations[0],date.month,date.day,tmp.startTime.hour,tmp.startTime.minute,tmp.release ? restseat.seat[daycnt][0]:tmp.seatNum);
    strcat(head,body);//connect body after head's last char
    Time t=Time(date,tmp.startTime.hour,tmp.startTime.minute);
    Time tmpt;
    for(int i=1;i<tmp.stationNum-1;++i){
        tmpt=t+tmp.travelTimes[i];
//        cout<<"month????????????????  "<<tmpt.date.month<<endl;
        snprintf(body,sizeof(body),"%s %02d-%02d %02d:%02d -> ",
                 tmp.stations[i],tmpt.date.month,tmpt.date.day,tmpt.hour,tmpt.minute);
        tmpt=tmpt+tmp.stopoverTimes[i];
        strcat(head,body);
        snprintf(body,sizeof(body),"%02d-%02d %02d:%02d %d %d\n",
                 tmpt.date.month,tmpt.date.day,tmpt.hour,tmpt.minute,tmp.prices[i],tmp.release ? restseat[daycnt][i] : tmp.seatNum);
        strcat(head,body);
    }
    tmpt=t+tmp.travelTimes[tmp.stationNum-1];
    snprintf(body,sizeof(body),"%s %02d-%02d %02d:%02d -> xx-xx xx:xx %d x",
             tmp.stations[tmp.stationNum-1],tmpt.date.month,tmpt.date.day,tmpt.hour,tmpt.minute,tmp.prices[tmp.stationNum-1]);
    strcat(head,body);
    char* ans=head;
    return ans;
}

const char* Trains::delete_train(const char *trainid) {
    pair<int,int> hash_id=hash.hash_it(trainid);
    if(!bpt_train.Exist(hash_id))return "-1";
    Train tmpt;
    int tmp_pos=bpt_train.Find(hash_id);
    TRAIN.Read(tmpt,tmp_pos);
    if(tmpt.release)return "-1";
    else {//succeed
        TRAIN.Delete(tmp_pos);
        bpt_train.Delete(make_pair(hash_id,tmp_pos));
        return "0";
    }
}

Ticket Trains::find_ticket(Date date, const char *from, const char *to, const char *op, const char *id, int hour, int minute) {
    pair<int,int> hash_f=hash.hash_it(from),hash_t=hash.hash_it(to),hash_id=hash.hash_it(id);
    Ticket noticket;
    noticket.price=-1;
    if(!bpt_station.Exist(hash_f)||!bpt_station.Exist(hash_t)||!bpt_train.Exist(hash_id)
    ||hash_f==hash_t||(strcmp(op,"time")&&strcmp(op,"cost")))return noticket;

    int sta_f_pos=bpt_station.Find(hash_f),sta_t_pos=bpt_station.Find(hash_t);
    Station sta_f,sta_t;
    STATION.Read(sta_f,sta_f_pos),STATION.Read(sta_t,sta_t_pos);
    vector<Ticket> tickets;
    int sta_f_trainnum=sta_f.trainnum;
    for(int i=1;i<=sta_f_trainnum;++i){
        int pos1=bpt_sta_train.Find(make_pair(hash_f,i));
        pair<pair<int,int>,int> sta1;
        STA_TRAIN.Read(sta1,pos1);
        if(sta1.first==hash_id)continue;

        int pos2=bpt_sta_train.Find(make_pair(hash_f,i));
        pair<pair<int,int>,int> sta2;
        STA_TRAIN.Read(sta2,pos2);

        int tmp_train_pos=bpt_train.Find(sta2.first);
        Train tmp_train;
        TRAIN.Read(tmp_train,tmp_train_pos);

        int pos3=bpt_sta_train.Find(make_pair(hash_f,i));
        pair<pair<int,int>,int> sta3;
        STA_TRAIN.Read(sta3,pos3);

        int pos=sta3.second;
        Time t1=Time(date,tmp_train.startTime.hour,tmp_train.startTime.minute)
                +tmp_train.travelTimes[pos]+tmp_train.stopoverTimes[pos];
        t1.date=date;
        if(t1.hour<hour||(t1.hour==hour&&t1.minute<minute))t1.date=t1.date+1;

        Time delt_time=t1-(tmp_train.travelTimes[pos]+tmp_train.stopoverTimes[pos]);
        int delt_day=delt_time.date-tmp_train.Date1;

        int seat_pos=bpt_seat.Find(hash.hash_it(tmp_train.trainID));;
        Seat restseat;
        SEAT.Read(restseat,seat_pos);

        int min_seat=restseat.seat[delt_day][pos];
        for(int j=pos+1;j<tmp_train.stationNum;j++){
            if(hash.hash_it(tmp_train.stations[j])==hash_t){
                Time t2=t1+(tmp_train.travelTimes[j]-(tmp_train.travelTimes[pos]+tmp_train.stopoverTimes[pos]));
                tickets.push_back(Ticket(tmp_train.trainID,from,to,tmp_train.prices[j]-tmp_train.prices[pos],min_seat,t1,t2));
                break;
            }
            if(restseat.seat[delt_day][j]<min_seat)min_seat=restseat.seat[delt_day][j];
        }
    }
    if(op[0]=='t'){
        int sz=tickets.size();
        Ticket *ti=new Ticket[sz];
        for(int i=0;i<sz;i++)ti[i]=tickets[i];
        tickets.clear();
        sort(ti,ti+sz,cmp_time2);
        for(int i=0;i<sz;i++)tickets.push_back(ti[i]);
        delete [] ti;
    }else {
        int sz=tickets.size();
        Ticket *ti=new Ticket[sz];
        for(int i=0;i<sz;++i)ti[i]=tickets[i];
        tickets.clear();
        sort(ti,ti+sz,cmp_cost);
        for(int i=0;i<sz;++i)tickets.push_back(ti[i]);
        delete []ti;
    }
    sta_f_trainnum=tickets.size();
    if(sta_f_trainnum==0)return noticket;
    else return tickets[0];
}

const char* Trains::query_ticket(Date date,const char* from,const char* to,const char* op) {
    pair<int,int> hash_f=hash.hash_it(from),hash_t=hash.hash_it(to);
    if(!bpt_station.Exist(hash_f)||!bpt_station.Exist(hash_t)){return "-1";}
    if(hash_f==hash_t){return "-1";}
    if(strcmp(op,"time")&&strcmp(op,"cost")){return "-1";}
    int sta_f_pos=bpt_station.Find(hash_f),sta_t_pos=bpt_station.Find(hash_t);
    Station sta_f,sta_t;
    STATION.Read(sta_f,sta_f_pos),STATION.Read(sta_t,sta_t_pos);
    vector<Ticket>tickets;
    int num=sta_f.trainnum;
    for(int i=1;i<=num;++i) {
        if (!bpt_sta_train.Exist(make_pair(hash_f, i))){return "-1";}//不存在pair<sta_from,1>
        int pos1=bpt_sta_train.Find(make_pair(hash_f, i));
        pair<pair<int,int>,int> sta1;
        STA_TRAIN.Read(sta1,pos1);
        if (!bpt_train.Exist(sta1.first)){return "-1";}

        int pos2=bpt_sta_train.Find(make_pair(hash_f, i));
        pair<pair<int,int>,int> sta2;
        STA_TRAIN.Read(sta2,pos2);
        int tmpt_pos=bpt_train.Find(sta2.first);
        Train tmpt;
        TRAIN.Read(tmpt,tmpt_pos);

        int pos3=bpt_sta_train.Find(make_pair(hash_f, i));
        pair<pair<int,int>,int> sta3;
        STA_TRAIN.Read(sta3,pos3);
        int pos = sta3.second;
        Time t1 = Time(date, tmpt.startTime.hour, tmpt.startTime.minute) +
                  tmpt.travelTimes[pos] + tmpt.stopoverTimes[pos];
        t1.date = date;
        Time t2 = t1 - (tmpt.travelTimes[pos] + tmpt.stopoverTimes[pos]);

        if (tmpt.Date2 < t2.date)continue;
        if (t2.date < tmpt.Date1)continue;
        int daynum = t2.date - tmpt.Date1;
        int seat_pos=bpt_seat.Find(hash.hash_it(tmpt.trainID));
        Seat restseat ;
        SEAT.Read(restseat,seat_pos);
        int seat_min = restseat.seat[daynum][pos];

//        cout << "pos = " << pos << endl;

        for (int j = pos + 1; j < tmpt.stationNum; ++j) {
//            cout << "j = " << j << " seat = " << seat_min << " rest_seat = " << restseat[daynum][j] << endl;
            if (hash.hash_it(tmpt.stations[j]) == hash_t) {
                Time jt = t1 + (tmpt.travelTimes[j] - (tmpt.travelTimes[pos] + tmpt.stopoverTimes[pos]));
//                printf("j = %d  pos = %d  %d  %d  %d\n",j, pos, tmpt.travelTimes[j], tmpt.travelTimes[pos], tmpt.stopoverTimes[pos]);
                tickets.push_back(Ticket(tmpt.trainID, from, to, tmpt.prices[j] - tmpt.prices[pos], seat_min,t1, jt));
                break;
            }
            if (restseat.seat[daynum][j] < seat_min)seat_min = restseat[daynum][j];
        }
    }
    if(op[0]=='t'){//TIME priority sort-1
//        cout<<"---sort bt time---"<<endl;
        int sz=tickets.size();
        Ticket *ti=new Ticket[sz];
        for(int i=0;i<sz;++i)ti[i]=tickets[i];
        tickets.clear();
        sort(ti,ti+sz,cmp_time1);
        for(int i=0;i<sz;++i)tickets.push_back(ti[i]);
        delete []ti;

    }else {//COST priority sort
//        cout<<"---sort bt cost---"<<endl;
        int sz=tickets.size();
        Ticket *ti=new Ticket[sz];
        for(int i=0;i<sz;++i)ti[i]=tickets[i];
        tickets.clear();
        sort(ti,ti+sz,cmp_cost);
        for(int i=0;i<sz;++i)tickets.push_back(ti[i]);
        delete []ti;
    }
    num=tickets.size();
//    cout<<"tickets size= "<<num<<endl;
    char head[8200];
    snprintf(head,sizeof(head),"%d",num);
    for(int i=0;i<num;++i){
        Ticket tmp_ti=tickets[i];
        char tmp[128];
        snprintf(tmp,sizeof(tmp),"\n%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d",
                 tmp_ti.train,tmp_ti.station_from,tmp_ti.depart.date.month,tmp_ti.depart.date.day,
                 tmp_ti.depart.hour,tmp_ti.depart.minute,
                 tmp_ti.station_to,tmp_ti.arrive.date.month,tmp_ti.arrive.date.day,
                 tmp_ti.arrive.hour,tmp_ti.arrive.minute,
                 tmp_ti.price,tmp_ti.seat);
        strcat(head,tmp);
    }
    char* ans=head;
    return ans;
}

const char* Trains::query_transfer(Date date, const char *from, const char *to, const char *op) {
    pair<int,int> hash_f=hash.hash_it(from),hash_t=hash.hash_it(to);
    if(!bpt_station.Exist(hash_f)||!bpt_station.Exist(hash_t))return "0";
    if(strcmp(op,"time")&&strcmp(op,"cost"))return "-1";

    int sta_f_pos=bpt_station.Find(hash_f),sta_t_pos=bpt_station.Find(hash_t);
    Station sta_f,sta_t;
    STATION.Read(sta_f,sta_f_pos),STATION.Read(sta_t,sta_t_pos);
    vector<Ticket> tickets;
    int sta_f_trainnum=sta_f.trainnum;
    Ticket ticket1,ticket2,ticket1_,ticket2_;
    int ans=0x7fffffff;
    for(int i=1;i<=sta_f_trainnum;++i){
        int pos1=bpt_sta_train.Find(make_pair(hash_f,i));
        pair<pair<int,int>,int> sta1;
        STA_TRAIN.Read(sta1,pos1);

        int tmp_train_pos=bpt_train.Find(sta1.first);
        Train tmp_train;
        TRAIN.Read(tmp_train,tmp_train_pos);

        int pos2=bpt_sta_train.Find(make_pair(hash_f,i));//本站的第几量火车
        pair<pair<int,int>,int> sta2;
        STA_TRAIN.Read(sta2,pos2);//sta2: <trainID.hash , tmptrain的第几站>
        int pos=sta2.second;
        Time t1=Time(date,tmp_train.startTime.hour,tmp_train.startTime.minute)
                +tmp_train.travelTimes[pos]+tmp_train.stopoverTimes[pos];
        int delt_day=(date-(t1.date-date))-tmp_train.Date1;
        if(((date - (t1.date - date)) < tmp_train.Date1) || (tmp_train.Date2 < (date- (t1.date - date)))) continue;

        int restseat_pos=bpt_seat.Find(hash.hash_it(tmp_train.trainID));
        Seat restseat;
        SEAT.Read(restseat,restseat_pos);

        int min_seat=restseat.seat[delt_day][pos];
        t1.date=date;
        for(int j=pos+1;j<tmp_train.stationNum;j++){
            if(hash.hash_it(tmp_train.stations[j])!=hash_t){
                Time t2=t1+(tmp_train.travelTimes[j]-(tmp_train.travelTimes[pos]+tmp_train.stopoverTimes[pos]));
                ticket1=Ticket(tmp_train.trainID,from,tmp_train.stations[j],tmp_train.prices[j]-tmp_train.prices[pos],min_seat,t1,t2);
                ticket2=find_ticket(t2.date,tmp_train.stations[j],to,op,tmp_train.trainID,t2.hour,t2.minute);
                if(ticket2.price==-1){if(restseat.seat[delt_day][j]<min_seat){min_seat=restseat.seat[delt_day][j];continue;}}
                if(op[0]=='t'){
                    if((ticket2.arrive-ticket1.depart<ans)||
                    (ticket2.arrive-ticket1.depart==ans)&&(ticket1.arrive-ticket1.depart<ticket1_.arrive-ticket1_.depart)){
                        ticket1_=ticket1,ticket2_=ticket2;
                        ans=ticket2.arrive-ticket1.depart;
                    }
                }else {
                    if((ticket1.price + ticket2.price < ans) || ((ticket1.price + ticket2.price == ans) && (ticket1.price < ticket1_.price))){
                        ticket1_=ticket1,ticket2_=ticket2;
                        ans=ticket1.price+ticket2.price;
                    }
                }
            }
            if(restseat.seat[delt_day][j]<min_seat)min_seat=restseat.seat[delt_day][j];
        }
    }
    if(ans==0x7fffffff)return "0";
    char head[8200];
    snprintf(head,sizeof(head),"%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n",
            ticket1_.train,ticket1_.station_from, ticket1_.depart.date.month, ticket1_.depart.date.day,
            ticket1_.depart.hour, ticket1_.depart.minute,
            ticket1_.station_to, ticket1_.arrive.date.month, ticket1_.arrive.date.day,
            ticket1_.arrive.hour, ticket1_.arrive.minute, ticket1_.price, ticket1_.seat);
    char tmp[128];
    snprintf(tmp, sizeof(tmp), "%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d",
             ticket2_.train,ticket2_.station_from, ticket2_.depart.date.month, ticket2_.depart.date.day,
             ticket2_.depart.hour, ticket2_.depart.minute,
             ticket2_.station_to, ticket2_.arrive.date.month, ticket2_.arrive.date.day,
             ticket2_.arrive.hour, ticket2_.arrive.minute, ticket2_.price, ticket2_.seat);
    strcat(head, tmp);
    char* ans_ = head;
    return ans_;
}