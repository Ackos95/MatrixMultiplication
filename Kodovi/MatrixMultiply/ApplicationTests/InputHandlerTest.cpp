#include <gtest\gtest.h>
#include <InputHandler.h>


class InputHandlerTests : public ::testing::Test
{
public:
	void TearDown();
};

void InputHandlerTests::TearDown()
{
	ApplicationData::getInstance()->reset();
}

TEST(InputHandlerTests, parseInputTest)
{
	ApplicationData* appData = ApplicationData::getInstance();
	appData->setInputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\validInput1.txt");
	ASSERT_NO_THROW(parseInputFile());
	ASSERT_EQ(appData->getA()->getSize(), 2);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			ASSERT_EQ(appData->getA()->getData()[i][j], i * 2 + j + 1);
			ASSERT_EQ(appData->getB()->getData()[i][j], 4 - (i * 2 + j));
			ASSERT_EQ(appData->getC()->getData()[i][j], 0);
		}
	appData->reset();

	appData->setInputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\validInput2.txt");
	ASSERT_NO_THROW(parseInputFile());
	ASSERT_EQ(appData->getA()->getSize(), 4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			ASSERT_EQ(appData->getA()->getData()[i][j], i * 4 + j + 1);
			ASSERT_EQ(appData->getB()->getData()[i][j], 16 - (i * 4 + j));
			ASSERT_EQ(appData->getC()->getData()[i][j], 0);
		}
	appData->reset();

	// new lines and tabs doesn't matter
	// and reader will take 2 * size^2 numbers from file, others will be disgarded
	// so more numbers then necessary will pass as valid input
	appData->setInputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\validInput3.txt");
	ASSERT_NO_THROW(parseInputFile());
	ASSERT_EQ(appData->getA()->getSize(), 4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			ASSERT_EQ(appData->getA()->getData()[i][j], i * 4 + j + 1);
			ASSERT_EQ(appData->getB()->getData()[i][j], 16 - (i * 4 + j));
			ASSERT_EQ(appData->getC()->getData()[i][j], 0);
		}
	appData->reset();

	// size is not 2^k, k>=0
	appData->setInputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\invalidInput1.txt");
	ASSERT_THROW(parseInputFile(), std::runtime_error);

	// non matching size and matrix (not enough numbers to fill both matrices)
	appData->setInputPath(".\\..\\..\\..\\TestDatoteke\\UnitTest\\invalidInput2.txt");
	ASSERT_THROW(parseInputFile(), std::runtime_error);

	appData->setInputPath("invalid_path");
	ASSERT_THROW(parseInputFile(), std::runtime_error);

	appData->reset();
}