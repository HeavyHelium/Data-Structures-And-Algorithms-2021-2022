#define CONSOLE
#include "input.h"
#include "..\..\hw1\hw1\implementation.h"

int main()try
{
	InputHandler ihandler;
	ihandler.getInput();
	std::cout << std::endl;
	MyStore MrazMag;
	MrazMag.init(ihandler.workersCnt(), 0, 0);
	MrazMag.addClients(ihandler.getClients(), ihandler.clientsCnt());
	MrazMag.executeDay();
	return 0;
}
catch (const std::exception& e)
{ std::cerr << e.what() << '\n'; }