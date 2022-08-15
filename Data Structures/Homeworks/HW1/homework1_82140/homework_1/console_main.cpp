#define CONSOLE
//#define LOOP true
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
