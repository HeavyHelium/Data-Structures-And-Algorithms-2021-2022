#define CONSOLE
#define LOOP true
#include "input.h"
#include "implementation.h"

int main()try
{
#ifdef LOOP 
	while (true) 
#endif
	{
		InputHandler ihandler;
		ihandler.getInput();
		std::cout << std::endl;
		MyStore MrazMag;
		MrazMag.init(ihandler.workersCnt(), 0, 0);
		MrazMag.addClients(ihandler.getClients(), ihandler.clientsCnt());
		MrazMag.executeDay();
		std::cout << std::endl;
	}
	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << '\n';
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
 2 3
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
D 60 banana
0 60 50 0
1 60 50 0
2 80 0 0
3 90 0 0

2  2
0 20 0 61
1  90 0 59

Изход:
W 0 banana
D 60 banana
0 60 20 0
1 60 80 0



*/