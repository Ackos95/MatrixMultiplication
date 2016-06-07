#include <gtest\gtest.h>
#include <ApplicationData.h>


class ApplicationDataTests : public ::testing::Test
{
	void TearDown();
};

void ApplicationDataTests::TearDown()
{
	ApplicationData::getInstance()->reset();
}


TEST_F(ApplicationDataTests, SinglethonTest)
{
	ApplicationData* app = ApplicationData::getInstance();
	ApplicationData* app2 = ApplicationData::getInstance();

	ASSERT_EQ(app, app2);
}

TEST_F(ApplicationDataTests, SimpleTypesGetterSetterTest)
{
	ApplicationData* appData = ApplicationData::getInstance();

	appData->setInputPath("Random input");
	ASSERT_EQ(strcmp(appData->getInputPath(), "Random input"), 0);  // strings are equal

	appData->setInputPath("New input path");
	ASSERT_NE(strcmp(appData->getInputPath(), "Random input"), 0);
	ASSERT_EQ(strcmp(appData->getInputPath(), "New input path"), 0);

	appData->setSerialOutputPath("Random output");
	ASSERT_EQ(strcmp(appData->getSerialOutputPath(), "Random output"), 0);
	appData->setParallelOutputPath("Random output");
	ASSERT_EQ(strcmp(appData->getParallelOutputPath(), "Random output"), 0);

	ASSERT_FALSE(appData->isVerbose());
	appData->setVerbose(true);
	ASSERT_TRUE(appData->isVerbose());

	ASSERT_TRUE(appData->shouldDoParallel());
	ASSERT_TRUE(appData->shouldDoSerial());

	appData->setParallel(false);
	ASSERT_FALSE(appData->shouldDoParallel());

	appData->setSerial(false);
	ASSERT_FALSE(appData->shouldDoSerial());
}

TEST_F(ApplicationDataTests, MatrixGetterSetterTest)
{
	Matrix<TYPE> A(4), B(4), C(4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			A.getData()[i][j] = 1;
			B.getData()[i][j] = 2;
			C.getData()[i][j] = 3;
		}

	ApplicationData* appData = ApplicationData::getInstance();

	// throw because of invalid matrix size (must be 2^k, k>=0)
	ASSERT_THROW(appData->setMatrixA(Matrix<TYPE>(3)), std::runtime_error);

	appData->setMatrixA(A);
	appData->setMatrixB(B);
	appData->setMatrixC(C);

	// value check
	for (int i = 0; i < 4; i++)
	{
		// TYPE must be primitive for this test to pass
		// if type is some non-primitive value this will fail
		// because setMatrix methods makes copies of objects
		// just in case for some future update but this test, 
		// and few other things in code, wont work if TYPE is non-primitive

		ASSERT_EQ(memcmp(appData->getA()->getData()[i], A.getData()[i], 4), 0);
		ASSERT_EQ(memcmp(appData->getB()->getData()[i], B.getData()[i], 4), 0);
		ASSERT_EQ(memcmp(appData->getC()->getData()[i], C.getData()[i], 4), 0);
	}

	// throw because of different dimensions
	ASSERT_THROW(appData->setMatrixA(Matrix<TYPE>(8)), std::runtime_error);

}