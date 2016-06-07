#include <gtest\gtest.h>
#include <ArgumentHandler.h>


class ArgumentHandlerTests : public ::testing::Test
{
	void TearDown();
};

void ArgumentHandlerTests::TearDown()
{
	ApplicationData::getInstance()->reset();
}

TEST_F(ArgumentHandlerTests, validateArgumentsTest)
{
	ApplicationData* appData = ApplicationData::getInstance();

	ASSERT_THROW(validateArguments(true), std::runtime_error);
	appData->setSerialOutputPath("random_output.txt");
	appData->setParallelOutputPath("random_output.txt");
	ASSERT_THROW(validateArguments(true), std::runtime_error);
	appData->setInputPath("random_input_path.txt");
	ASSERT_NO_THROW(validateArguments(true));
	appData->setInputPath("");
	appData->setMatrixA(Matrix<TYPE>(4));
	ASSERT_THROW(validateArguments(true), std::runtime_error);
	appData->setMatrixB(Matrix<TYPE>(4));
	ASSERT_NO_THROW(validateArguments(true));

	appData->reset();
}

TEST_F(ArgumentHandlerTests, generateMatricesTest)
{
	ApplicationData* appData = ApplicationData::getInstance();

	ASSERT_THROW(generateMatrices(3), std::runtime_error);
	generateMatrices(4);

	ASSERT_EQ(appData->getA()->getSize(), appData->getB()->getSize());
	ASSERT_EQ(appData->getA()->getSize(), appData->getB()->getSize());

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ASSERT_EQ(appData->getC()->getData()[i][j], 0);

	appData->reset();
}

TEST_F(ArgumentHandlerTests, parseArgumentsTest)
{
	ApplicationData* appData = ApplicationData::getInstance();
	char** arguments = new char*[7];

	arguments[0] = new char[5]{ "path" }; // this wont change through tests
	for (int i = 1; i < 7; i++)
		arguments[i] = new char[10];

	// -i and no path (should throw error)
	strcpy_s(arguments[1], 10, "-v");
	strcpy_s(arguments[2], 10, "-i");
	ASSERT_THROW(parseArguments(3, arguments), std::runtime_error);
	appData->reset();

	// -o and no path (should throw error)
	// acctually this fails becuase "i_path" is not recognized as command
	// it will set "-i" as a path to input file, which should be then rechecked
	// for validation in InptutHandler (there is checked file validation)
	strcpy_s(arguments[1], 10, "-o");
	strcpy_s(arguments[2], 10, "-i");
	strcpy_s(arguments[3], 10, "i_path");
	ASSERT_THROW(parseArguments(2, arguments), std::runtime_error);
	appData->reset();

	strcpy_s(arguments[1], 10, "-i");
	strcpy_s(arguments[2], 10, "i_path");
	strcpy_s(arguments[3], 10, "-o");
	strcpy_s(arguments[4], 10, "o_path");
	ASSERT_NO_THROW(parseArguments(5, arguments));
	ASSERT_EQ(strcmp(appData->getInputPath(), "i_path"), 0);
	ASSERT_EQ(strcmp(appData->getSerialOutputPath(), "o_path"), 0);
	ASSERT_EQ(strcmp(appData->getParallelOutputPath(), "o_path"), 0);
	ASSERT_FALSE(appData->isVerbose());
	appData->reset();

	// no output file (must fail)
	strcpy_s(arguments[1], 10, "-r");
	strcpy_s(arguments[2], 10, "4");
	ASSERT_THROW(parseArguments(2, arguments), std::runtime_error);
	appData->reset();

	strcpy_s(arguments[3], 10, "-os");
	strcpy_s(arguments[4], 10, "o_path");
	strcpy_s(arguments[5], 10, "-so"); // if no parallel output path is set and serial only is not that is invalid
	ASSERT_NO_THROW(parseArguments(6, arguments));
	ASSERT_EQ(appData->getA()->getSize(), 4); // all matrix sizes will match (check generateMatrixTest)
	ASSERT_TRUE(appData->isVerbose());
	ASSERT_EQ(strcmp(appData->getSerialOutputPath(), "o_path"), 0);
	appData->reset();

	strcpy_s(arguments[3], 10, "-op");
	strcpy_s(arguments[4], 10, "o_path");
	strcpy_s(arguments[5], 10, "-po"); // if no serial output path is set and parallel only is ot that is invalid
	ASSERT_NO_THROW(parseArguments(6, arguments));
	ASSERT_EQ(appData->getA()->getSize(), 4); // all matrix sizes will match (check generateMatrixTest)
	ASSERT_TRUE(appData->isVerbose());
	ASSERT_EQ(strcmp(appData->getParallelOutputPath(), "o_path"), 0);
	appData->reset();

	// path -r 4 -o o_path -so
	strcpy_s(arguments[3], 10, "-o");
	strcpy_s(arguments[5], 10, "-so");
	ASSERT_NO_THROW(parseArguments(6, arguments));
	ASSERT_TRUE(appData->shouldDoSerial());
	ASSERT_FALSE(appData->shouldDoParallel());
	appData->reset();

	// path -r 4 -o o_path -so
	strcpy_s(arguments[5], 10, "-po");
	ASSERT_NO_THROW(parseArguments(6, arguments));
	ASSERT_FALSE(appData->shouldDoSerial());
	ASSERT_TRUE(appData->shouldDoParallel());
	appData->reset();

	// path -r asd -o o_path
	strcpy_s(arguments[2], 10, "asd");
	ASSERT_THROW(parseArguments(5, arguments), std::runtime_error);
	appData->reset();

	for (int i = 0; i < 7; i++)
		delete[] arguments[i];
	delete[] arguments;
}