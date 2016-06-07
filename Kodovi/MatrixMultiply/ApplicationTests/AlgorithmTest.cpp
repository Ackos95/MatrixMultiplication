#include <gtest\gtest.h>
#include "Utility.h"
#include <InputHandler.h>
#include <OutputHandler.h>
#include <SerialAlgorithm.h>
#include <ParallelAlgorithm.h>
#include <ApplicationData.h>
#include <ArgumentHandler.h>

#define TEST_UNTIL 256

TEST(AlgorithmTests, TwoByTwoMatrixMultiplicationTest)
{
	char** argv = new char*[10];
	for (int i = 0; i < 10; i++)
		argv[i] = new char[512];

	const char* baseFolder = ".\\..\\..\\..\\TestDatoteke\\MatlabTests\\";
	const char* inPostfix = "_in.txt";
	const char* outPostfix = "_out.txt";
	const char* outSerialPostfix = "_out_app_s.txt";
	const char* outParallelPostfix = "_out_app_p.txt";

	char numberBuffer[20];

	strcpy_s(argv[0], 512, "");
	strcpy_s(argv[1], 512, "-i");  // real path is generated in for loop (argv[2])
	strcpy_s(argv[3], 512, "-os"); // real path is generated in for loop (argv[4])
	strcpy_s(argv[5], 512, "-op"); // real path is generated in for loop (argv[6])
	strcpy_s(argv[7], 512, "-v");

	for (int k = 2; k <= TEST_UNTIL; k *= 2)
	{
		_itoa_s(k, numberBuffer, 10);

		strcpy_s(argv[2], 512, baseFolder); // input file path
		strcpy_s(argv[4], 512, baseFolder); // outputSerial file path
		strcpy_s(argv[6], 512, baseFolder); // outputParallel file path
		strcpy_s(argv[8], 512, baseFolder); // expected output file path

		strcat_s(argv[2], 512, numberBuffer);
		strcat_s(argv[2], 512, inPostfix);
		strcat_s(argv[4], 512, numberBuffer);
		strcat_s(argv[4], 512, outSerialPostfix);
		strcat_s(argv[6], 512, numberBuffer);
		strcat_s(argv[6], 512, outParallelPostfix);
		strcat_s(argv[8], 512, numberBuffer);
		strcat_s(argv[8], 512, outPostfix);

		parseArguments(8, argv);
		parseInputFile();
		serialMultiply();
		parallelMultiply();
		writeToFile();

		ApplicationData::getInstance()->reset();

		ASSERT_TRUE(isFilesEqual(argv[8], argv[4]));
		ASSERT_TRUE(isFilesEqual(argv[8], argv[6]));
	}
}