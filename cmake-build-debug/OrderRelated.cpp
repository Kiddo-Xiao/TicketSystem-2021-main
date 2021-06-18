#include "OrderRelated.hpp"
#include "Include.hpp"
#include "Connect.hpp"

using namespace std;

const char* Orders::buy_ticket(const char *u, const char *id, Date date, int n, const char *from, const char *to,
                               const char *q) {
    pair<int, int> hash_u = hash.hash_it(u), hash_ID = hash.hash_it(id), hash_f = hash.hash_it(
            from), hash_t = hash.hash_it(to);
    if ((!connect->cuser.bpt_user.Exist(hash_u)) || (!connect->ctrain.bpt_train.Exist(hash_ID)) ||
        (!connect->ctrain.bpt_station.Exist(hash_f)) ||
        (!connect->ctrain.bpt_station.Exist(hash_t))) { return "-1"; }
    if(strcmp(q,"false")&&strcmp(q,"true")){return "-1";}
    if(!connect->loguser.count(u)){return "-1";}

    //debug
    string aa="NoirCorne";
    int nnpos=connect->cuser.bpt_user.Find(hash.hash_it(aa.c_str()));
    cout<<"position : "<<nnpos<<endl;
    User nn;
    USER.Read(nn,nnpos);
    cout<<"NoirCorne now password : "<<nn.password<<endl;


    int tmptrain_pos=connect->ctrain.bpt_train.Find(hash_ID);
    Train tmptrain;
    TRAIN.Read(tmptrain,tmptrain_pos);
    if(tmptrain.release==0||tmptrain.seatNum<n){return "-1";}

    int sta_from_pos=connect->ctrain.bpt_station.Find(hash_f);
    int sta_to_pos=connect->ctrain.bpt_station.Find(hash_t);
    Station sta_from,sta_to;
    STATION.Read(sta_from,sta_from_pos);
    STATION.Read(sta_to,sta_to_pos);
    int from_pos=-1,to_pos=-1;

//    cout<<"here:2"<<endl;

    for(int i=1;i<=sta_from.trainnum;i++){
        int pos=connect->ctrain.bpt_sta_train.Find(make_pair(hash_f,i));
        pair<pair<int,int>,int> tmp;
        STA_TRAIN.Read(tmp,pos);

        if(hash_ID == tmp.first){
            from_pos=tmp.second;
            break;
        }
    }
    for(int i=1;i<=sta_to.trainnum;++i){
        int pos=connect->ctrain.bpt_sta_train.Find(make_pair(hash_t,i));
        pair<pair<int,int>,int> tmp;
        STA_TRAIN.Read(tmp,pos);

        if(hash_ID==tmp.first){
            to_pos=tmp.second;
            break;
        }
    }
    if((from_pos==-1)||(to_pos==-1)||to_pos<=from_pos){return "-1";}
//
    Time from_time=tmptrain.startTime;from_time.date=date;
    from_time=from_time+(tmptrain.travelTimes[from_pos]+tmptrain.stopoverTimes[from_pos]);
    int delt_day=(date-(from_time.date-date))-tmptrain.Date1;
    if((date-(from_time.date-date)<tmptrain.Date1)||(tmptrain.Date2<(date-(from_time-date)))){return "-1";}
    from_time.date=date;
    Time to_time=from_time+(tmptrain.travelTimes[to_pos]-(tmptrain.travelTimes[from_pos]+tmptrain.stopoverTimes[from_pos]));
    Ticket ticket=Ticket(id,from,to,tmptrain.prices[to_pos]-tmptrain.prices[from_pos],n,from_time,to_time);
    int seat_pos=connect->ctrain.bpt_seat.Find(hash_ID);
    Seat seat;
    SEAT.Read(seat,seat_pos);
    char status[25];
    int minn=0x7fffffff;
    for(int i=from_pos;i<to_pos;++i){//7 - 8 : 7
        minn=min(minn,seat.seat[delt_day][i]);
    }
    if(minn>=n) {
        for (int i = from_pos; i < to_pos; ++i)seat.seat[delt_day][i] -= n;
        strcpy(status, "success");
        SEAT.Write(seat,seat_pos);
//        cout << "from = " << from_pos << " to = " << to_pos << endl;
//        for(int i = 0; i < to_pos; i++)
//            cout << seat.seat[delt_day][i] << " ";
//        puts("");
    }else {
        if(!strcmp(q,"true"))strcpy(status,"pending");
        else return "-1";
    }
    int tmpuser_pos=connect->cuser.bpt_user.Find(hash_u);
    User tmpuser;
    USER.Read(tmpuser,tmpuser_pos);
    tmpuser.ordernum++;
//    cerr<<tmpuser.name<<" ordernum = "<<tmpuser.ordernum<<endl;
    USER.Write(tmpuser,tmpuser_pos);
    if(strcmp(status,"pending")==0){
        seat.ordernum[delt_day]++;
        SEAT.Write(seat,seat_pos);
        TRAIN.Write(tmptrain,tmptrain_pos);
        Order order=Order(status,u,tmpuser.ordernum,id,delt_day,seat.ordernum[delt_day],ticket);
        int order_pos=ORDER.Newpos(),use_order_pos=USE_ORDER.Newpos();
        ORDER.Write(order,order_pos),USE_ORDER.Write(order,use_order_pos);
        bpt_order.Insert(make_pair(make_pair(make_pair(hash_ID,delt_day),seat.ordernum[delt_day]),order_pos));
        use_order.Insert(make_pair(make_pair(hash_u,tmpuser.ordernum),use_order_pos));
//        cout<<"use_order_pos= "<<use_order_pos<<endl;
        return "queue";
    }else{
        Order order=Order(status,u,tmpuser.ordernum,id,delt_day,0,ticket);
        int order_pos=USE_ORDER.Newpos();
//        cout<<"use_order_pos= "<<order_pos<<endl;
//        cout<<"order.status= "<<order.status<<endl;

        USE_ORDER.Write(order,order_pos);
        Order tmporder;
        USE_ORDER.Read(tmporder, order_pos);

        use_order.Insert(make_pair(make_pair(hash_u,tmpuser.ordernum),order_pos));
//        cout<<"order_pos= "<<order_pos<<endl;
        int aaa=use_order.Find(make_pair(hash_u,tmpuser.ordernum));
//        cout<<"aaa= "<<aaa<<endl;
        Order bbb;
        USE_ORDER.Read(bbb,aaa);
//        cout<<"use_order_pos= "<<aaa<<endl;
//        cout<<"order.status= "<<bbb.status<<endl;

        char* ans;
        char head[256];
        snprintf(head,256,"%d",ticket.price*ticket.seat);
        ans =head;
        return ans;
    }
}

const char* Orders::query_order(const char *u) {
    pair<int,int> hash_u=hash.hash_it(u);
    if(!(connect->cuser.bpt_user.Exist(hash_u))){return "-1";}
    if(!connect->loguser.count(u))return "-1";

    char* ans;
    char head[8200];
//    cout<<"ordernum= "<<tmpuser.ordernum<<endl;
    int user_pos=connect->cuser.bpt_user.Find(hash_u);
    User tmpuser;
    USER.Read(tmpuser,user_pos);
    snprintf(head,sizeof(head),"%d",tmpuser.ordernum);
    for(int i=tmpuser.ordernum;i>=1;i--) {
        int order_pos=use_order.Find(make_pair(hash_u, i));
//        cout<<"use_order_pos= "<<order_pos<<endl;
        Order order ;
        USE_ORDER.Read(order,order_pos);
//        cout << "order_status: "<< order.status << endl;
        char tmp[128];
//        cout<<"order_trainname: "<<order.ticket.train<<endl;
//        cout<<"order_seatnum: "<<order.ticket.seat<<endl;
        snprintf(tmp, sizeof(tmp), "\n[%s] %s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d", order.status,
                 order.ticket.train,
                 order.ticket.station_from, order.ticket.depart.date.month, order.ticket.depart.date.day,
                 order.ticket.depart.hour, order.ticket.depart.minute,
                 order.ticket.station_to, order.ticket.arrive.date.month, order.ticket.arrive.date.day,
                 order.ticket.arrive.hour, order.ticket.arrive.minute, order.ticket.price, order.ticket.seat);
        strcat(head, tmp);
    }
    ans=head;
    return ans;
}

const char* Orders::refund_ticket(const char *u, int n) {
    pair<int,int> hash_u=hash.hash_it(u);
    if(!(connect->cuser.bpt_user.Exist(hash_u)))return "-1";
//    printf("tmpuser : %s  is_login = %d\n", tmpuser.username, tmpuser.logstate);
    if(!connect->loguser.count(u))return "-1";

    int tmpuser_pos=connect->cuser.bpt_user.Find(hash_u);
    User tmpuser;
    USER.Read(tmpuser,tmpuser_pos);
    if(tmpuser.ordernum<n)return "-1";
    int order_pos=use_order.Find(make_pair(hash_u,tmpuser.ordernum+1-n));
    Order order;
    USE_ORDER.Read(order,order_pos);
    pair<int,int> hash_id=hash.hash_it(order.train_ID);
    int tmptrain_pos=connect->ctrain.bpt_train.Find(hash_id);
    Train tmptrain;
    TRAIN.Read(tmptrain,tmptrain_pos);

    if(!strcmp(order.status,"refunded")){return "-1";}
    else if(!strcmp(order.status,"pending")){
//        cout<<"22222222222222222222222222222222222222222222"<<endl;
        strcpy(order.status,"refunded");
        int use_order_pos=use_order.Find(make_pair(hash_u,order.id_user));
        int order_pos_=bpt_order.Find(make_pair(make_pair(hash_u, order.d_day), order.id_train));
        USE_ORDER.Write(order,use_order_pos);
        ORDER.Write(order,order_pos_);
        return "0";
    }else {
//        cout<<"333333333333333333333333333333333333333333333"<<endl;
        strcpy(order.status,"refunded");
        int seat_pos=connect->ctrain.bpt_seat.Find(hash_id);
        Seat seat;
        SEAT.Read(seat,seat_pos);
        int sta_from_pos=connect->ctrain.bpt_station.Find(hash.hash_it(order.ticket.station_from));
        int sta_to_pos=connect->ctrain.bpt_station.Find(hash.hash_it(order.ticket.station_to));
        Station from,to;
        STATION.Read(from,sta_from_pos),STATION.Read(to,sta_to_pos);
        int from_pos=-1,to_pos=-1;

        for(int i=1;i<=from.trainnum;i++){
            int pos1=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(order.ticket.station_from),i));
            pair<pair<int,int>,int> sta1;
            STA_TRAIN.Read(sta1,pos1);
            if(hash_id==sta1.first){

                int pos2=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(order.ticket.station_from),i));
                pair<pair<int,int>,int> sta2;
                STA_TRAIN.Read(sta2,pos2);
                from_pos=sta2.second;
                break;
            }
        }
        for(int i=1;i<=to.trainnum;++i){
            int pos1=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(order.ticket.station_to), i));
            pair<pair<int,int>,int> sta1;
            STA_TRAIN.Read(sta1,pos1);
            if(hash_id==sta1.first){
                int pos2=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(order.ticket.station_to), i));
                pair<pair<int,int>,int> sta2;
                STA_TRAIN.Read(sta2,pos2);
                to_pos=sta2.second;
                break;
            }
        }
        for(int i=from_pos;i<to_pos;++i)seat.seat[order.d_day][i]+=order.ticket.seat;
        int use_order_pos=use_order.Find(make_pair(hash_u, order.id_user));
        int order_pos=bpt_order.Find(make_pair(make_pair(hash_u,order.d_day),order.id_train));
        USE_ORDER.Write(order,use_order_pos);
        ORDER.Write(order,order_pos);

        Order usr_tmp, tmp;
        USE_ORDER.Read(usr_tmp, use_order_pos);
        ORDER.Read(tmp, order_pos);
//        cout << "use_order_pos = " << use_order_pos << endl;
//        cout << "usr_tmp status :::: " << usr_tmp.status << endl;
//        cout << "tmp status :::: " << tmp.status << endl;

        for(int i=1;i<=seat.ordernum[order.d_day];++i){
            int tmporder_pos=bpt_order.Find(make_pair(make_pair(hash_id,order.d_day),i));
            Order tmporder;
            ORDER.Read(tmporder,tmporder_pos);
            if(strcmp(tmporder.status,"pending"))continue;
            int tmp_from_pos_=connect->ctrain.bpt_station.Find(hash.hash_it(tmporder.ticket.station_from));
            int tmp_to_pos_=connect -> ctrain.bpt_station.Find(hash.hash_it(tmporder.ticket.station_to));
            Station tmp_from,tmp_to;
            STATION.Read(tmp_from,tmp_from_pos_),STATION.Read(tmp_to,tmp_to_pos_);
            int tmp_from_cnt=tmp_from.trainnum,tmp_to_cnt=tmp_to.trainnum;
            int tmp_from_pos=-1,tmp_to_pos=-1;
            for(int i=1;i<=tmp_from_cnt;++i){
                int pos1=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(tmporder.ticket.station_from),i));
                pair<pair<int,int>,int> sta1;
                STA_TRAIN.Read(sta1,pos1);
                if(hash_id==sta1.first){
                    int pos2=connect -> ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(tmporder.ticket.station_from), i));
                    pair<pair<int,int>,int> sta2;
                    STA_TRAIN.Read(sta2,pos2);
                    tmp_from_pos = sta2.second;
//                    cout<<6<<endl;
                    break;
                }
            }

            for(int i=1;i<=tmp_to_cnt;++i){
                int pos1=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(tmporder.ticket.station_to),i));
                pair<pair<int,int>,int> sta1;
                STA_TRAIN.Read(sta1,pos1);
                if(hash_id==sta1.first){
                    int pos2=connect->ctrain.bpt_sta_train.Find(make_pair(hash.hash_it(tmporder.ticket.station_to),i));
                    pair<pair<int,int>,int> sta2;
                    STA_TRAIN.Read(sta2,pos2);
                    tmp_to_pos=sta2.second;
//                    cout<<7<<endl;
                    break;
                }
            }
            int minn=0x7fffffff;
            for(int i=tmp_from_pos;i<tmp_to_pos;++i)minn=min(minn,seat.seat[order.d_day][i]);
            if(minn>=tmporder.ticket.seat){
                strcpy(tmporder.status,"success");
                for(int i=tmp_from_pos;i<tmp_to_pos;++i)seat.seat[order.d_day][i]-=tmporder.ticket.seat;
//                int order_pos_=ORDER.Newpos(),use_order_pos_=USE_ORDER.Newpos();
                ORDER.Write(tmporder,tmporder_pos),USE_ORDER.Write(tmporder,use_order_pos);
                use_order.Change(make_pair(hash.hash_it(tmporder.username),tmporder.id_user),order_pos);
                bpt_order.Change(make_pair(make_pair(hash.hash_it(tmporder.train_ID), tmporder.d_day), tmporder.id_train), use_order_pos);
            }
        }
        int pppos=SEAT.Newpos();
        SEAT.Write(seat,pppos);
        connect -> ctrain.bpt_seat.Change(hash_id, pppos);
        return "0";
    }
}

