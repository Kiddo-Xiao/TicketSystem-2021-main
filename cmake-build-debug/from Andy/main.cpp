#include "Include.hpp"
#include "Connect.hpp"
#include <unordered_map>

using namespace std;


Connector connector;

int main(){

    freopen("1.in","r",stdin);
    freopen("test_out.txt","w",stdout);

//    BPlusTree<pair<int,int>,Seat> bpt_seat{"Bpt_Seat.dat"};


//    printf("一个包子");
//    cerr << ".................asdasdasd................" << endl;
//    fflush(stderr);
//    puts("-----------before run-----------");
    connector.run();
    return 0;
}