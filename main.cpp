#include "newrack.hpp"
#include "RackWrapper.h"
#include "Fundamental.hpp"
#include <stdlib.h>
#include <unistd.h>

int main()
{
	bool ret = RackSetup(16, 2, 0);
	if(!ret)
		return -1;

	for(int n = 0; n < 1000; ++n)
	{
		RackRender(1);
		usleep(1000);
	}

	RackCleanup();
	return 0;
}
