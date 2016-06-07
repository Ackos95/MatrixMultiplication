#include <gtest\gtest.h>
#include "Utility.h"
#include <OutputHandler.h>


TEST(OutputHandlerTests, writeOutputFileTest)
{
	Matrix<TYPE> A(4), B(4), C(4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			A.getData()[i][j] = i * 4 + j + 1;
			B.getData()[i][j] = 16 - (i * 4 + j);
			C.getData()[i][j] = 0;
		}
	ApplicationData* appData = ApplicationData::getInstance();

	// no data set, can not write to file
	ASSERT_THROW(writeToFile(), std::runtime_error);

	appData->setMatrixA(A);
	appData->setMatrixB(B);

	// not enough data set, can not write to file
	ASSERT_THROW(writeToFile(), std::runtime_error);
	appData->setMatrixC(C);
	appData->setMatrixCp(C);

	appData->setSerialOutputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out.txt");
	appData->setParallelOutputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out_p.txt");
	ASSERT_NO_THROW(writeToFile());
	ASSERT_TRUE(isFilesEqual(".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out.txt", ".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out_expected.txt"));
	ASSERT_TRUE(isFilesEqual(".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out_p.txt", ".\\..\\..\\..\\TestDatoteke\\UnitTest\\non_verbose_out_expected.txt"));

	appData->setVerbose(true);
	appData->setSerialOutputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out.txt");
	appData->setParallelOutputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out_p.txt");
	ASSERT_NO_THROW(writeToFile());
	ASSERT_TRUE(isFilesEqual(".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out.txt", ".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out_expected.txt"));
	ASSERT_TRUE(isFilesEqual(".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out_p.txt", ".\\..\\..\\..\\TestDatoteke\\UnitTest\\verbose_out_expected.txt"));

	appData->reset();
}