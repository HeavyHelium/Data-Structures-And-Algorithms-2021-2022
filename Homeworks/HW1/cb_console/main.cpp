#include <iostream>
#include "interface.h"

int main()
{

    /*
    5 4
    0 10 0 10
    45 35 0 30
    46 30 20 100
    200 10 10 1
    */
    /*
    W 0 banana
    0 10 0 0
    W 46 schweppes
    D 60 banana
    1 60 35 0
    D 106 schweppes
    2 106 30 20
    3 200 10 10
    */
    MyStore mrazMag;
    mrazMag.init(5, 0, 0);
    Client cl0 {0, 10, 0, 10};
    Client cl1 {45, 35, 0, 30};
    Client cl2 {46, 30, 20, 100};
    Client cl3 {200, 10, 10, 1};
    //std::cout << mrazMag.getBanana() << " " << mrazMag.getSchweppes() << std::endl;
    mrazMag.addClient(cl0);
    mrazMag.advanceTo(11);
    //std::cout << mrazMag.getBanana() << " " << mrazMag.getSchweppes() << std::endl;
    //mrazMag.addClient(cl1);
    //mrazMag.addClient(cl2);
    //mrazMag.addClient(cl3);
    //std::cout << mrazMag.getBanana() << " " << mrazMag.getSchweppes() << std::endl;
    //mrazMag.advanceTo(cl3.maxWaitTime + cl3.arriveMinute);


    return 0;
}
