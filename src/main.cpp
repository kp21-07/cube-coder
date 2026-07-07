#include "test/test.h"
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(NULL));
	test_solver(IDA_Combined);
	return 0;
}
