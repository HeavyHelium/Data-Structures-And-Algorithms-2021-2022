#include <iostream>
#include "input.hpp"

/// TODO: make it a relative path
#include "G:\FMI\Year 2\fall_winter\SDP\Homeworks\HW1\tests\1\src\implementation.hpp"

int main() try
{
#if 1
	InputHandler ihandler;
	ihandler.getInput();
	MyStore MrazMag;
	MrazMag.init(ihandler.workersCnt(), 0, 0);
	std::cout << std::endl;
	MrazMag.addClients(ihandler.getClients(), ihandler.clientsCnt());
	MrazMag.executeDay();
	std::cout << std::endl;
	std::cout << MrazMag.getBanana() << ", " << MrazMag.getSchweppes() << std::endl;
#endif
#if 0
	MyStore MrazMag;
	MrazMag.init(0, 0, 0);
	Client c1{ 0, 0, 10, 1 };
	MrazMag.addClients(&c1, 1);
	//MrazMag.addClients(&c2, 1);
	//MrazMag.addClients(&c3, 1);
	MrazMag.advanceTo(1);
	return 0;
#endif
}
catch (const std::exception& e)
{
	std::cerr << e.what() << '\n';
	return -1;
}

/*
5 4
0 10 0 10
45 35 0 30
46 30 20 100
200 10 10 1

Изход:
W 0 banana
0 10 0 0
W 46 schweppes
D 60 banana
1 60 35 0
D 106 schweppes
2 106 30 20
3 200 10 10

5 4
0 50 50 70
10 50 50 50
60 50 50 0
60 50 50 50

Изход:
W 0 banana
W 0 schweppes
D 60 banana
D 60 schweppes
0 60 50 50
1 60 50 50
W 60 banana
W 60 schweppes
2 60 0 0
3 110 0 0
D 120 banana
D 120 schweppes

*/

/*
 1 3
 0 15 20 65
 0 35 20 5
 10 5 0 5

Изход:
W 0 banana
W 0 schweppes
1 5 0 0
2 15 0 0
D 60 banana
D 60 schweppes
0 60 15 20


3 4
0  50 0 60
10 50 0 60
20 20 0 60
30 10 0 60

Изход:
W 0 banana
W 20 banana
D 60 banana
0 60 50 0
1 60 50 0
D 80 banana
2 80 20 0
3 80 10 0

1  2
0 20 0 61
1  90 0 59

Изход:
W 0 banana
W 1 banana
D 60 banana
0 60 20 0
1 60 80 0
D 61 banana

*/