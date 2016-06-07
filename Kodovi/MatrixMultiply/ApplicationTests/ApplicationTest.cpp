#include <gtest\gtest.h>
#include <iostream>
#include <ParallelAlgorithm.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();
	
	system("PAUSE");
	return r;
}