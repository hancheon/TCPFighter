#include "stdafx.h"
#include "Network.h"

int main()
{
	timeBeginPeriod(1);

	Server server;

	if (server.Init()) {
		server.Run();
	}

	timeEndPeriod(1);
}