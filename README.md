# TicketSystem-2021-main
 Procrastination is the original sin.

# Ticket System_2021--Aplese&Kiddo

------

###分工

> * Bplustree -- aplese
> * 主体逻辑 -- kiddo

#Bplutree简介
* 用一个struct
* node来表示b+Tree里的所有节点，保存left,right,size,val,son,leaf共6种变量。
* 其中left与right分别表示改节点的左右兄弟节点分别在文件中所存储的位置。
* size为当前节点已经存储的信息个数。
* son为数组，其中son[i]记录第i个儿子所在文件中存储的位置。
* val为一个pair类型的数组，存的是key与val的pair。

###B+Tree可调用的函数
|   函数名  | 功能  |
| :-------- :  | :-------:  |
|Insert|插入|
|Delete|删除|
|Find|找key对应的value|
|Exist|判key存在|
|Empty|判空|
|Change|修改key的value|
|Clean|清空整棵树|

#主体逻辑简介
##文件

- from aplese
> * Bpt.hpp (from aplese
> * File_oi.hpp (from aplese

- from kiddo
> * BasicElements.hpp
> * Include.hpp
> * Connect.hpp
> * Connect.cpp
> * UserRelated.hpp
> * UserRelated.cpp
> * TrainRelated.hpp
> * TrainRelated.cpp
> * OrderRelated.hpp
> * OrderRelated.cpp
> * main.cpp

##Bplustree & 存储类文件
|   树名   | key  |  value  | bpt文件名 |
| :-------- :  | :-------:  | :-------:  |:-----:|
|bpt_user  哈希-用户名|User在文件中位置|Bpt_User.dat|
|bpt_train|哈希-车名|Train在文件中位置|Bpt_Train.dat|
|bpt_seat|哈希-车名|Seat在文件中位置|Bpt_Seat.dat|SEAT.dat|
|bpt_station|哈希-站名|Station在文件中位置|Bpt_Station.dat|
|bpt_sta_train|pair<哈希-站名,经该站车次>|Train在文件中位置|Bpt_Sta_Train.dat|
|bpt_order|pair<use_order.kay,车次序号> | Order在文件中位置 |Bpt_Order.dat|
|use_order| pair<哈希-用户,用户order序号> | Order在文件中位置|Use_Order.dat|

| 储存文件名 |
|-----|
|USER.dat|
|TRAIN.dat|
|STATION.dat|
|STA_TRAIN.dat|
|ORDER.dat|
|USE_ORDER_.dat|


##类功能函数

### Users
|   函数名   | 描述   |  返回值  |
| :--------:   | :-----:  | :----:  |
| print_user     | 输出用户信息 |  无    |
| add_user |  添加用户   |   0/-1   |
| login |   用户登录 |  0/-1  |
|logout|退出登录|0/-1|
|query_profile|查询用户信息|用户信息/失败|
|modify_profile|修改用户信息|用户信息/失败|

###Trains
|   函数名   | 描述   |  返回值  |
| :--------:   | :-----:  | :----:  |
|add_train|添加车次|0/-1|
|release_train|发布车次|0/-1|
|query_train|查询车次|车次信息/失败|
|delete_train|删除车次|0/-1|
|find_ticket|查询某时发车某站到某站车次|最优车次|
|query_ticket|查询车票|车票信息/失败|
|query_transfer|车票改签|时间最优/价格最优车票|

###Orders
|   函数名   | 描述   |  返回值  |
| :--------:   | :-----:  | :----:  |
|but_ticket|买票|0/-1|
|query_order|查询指令|某用户所有指令/失败|
|refund_ticket|退票|0/-1|

###Connect
|   函数名   | 描述   |  返回值  |
| :--------:   | :-----:  | :----:  |
|run|程序运行|无|
|Exit|程序退出 所有用户下线|bye|
|read_xxx|按格式要求读入xxx|读取转化后格式|
***下同User Train Order函数功能**
