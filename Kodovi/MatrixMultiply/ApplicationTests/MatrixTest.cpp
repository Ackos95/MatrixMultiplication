#include <gtest\gtest.h>
#include <Matrix.h>


TEST(MatrixTestCase, InitialMatrixTesting)
{
	Matrix<int> m(10);

	ASSERT_EQ(m.getSize(), 10);
}

TEST(MatrixTestCase, CopyConstructorTesting)
{
	Matrix<int> m(5);
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			m.getData()[i][j] = j;

	Matrix<int> m2(m);

	ASSERT_EQ(m2.getSize(), m.getSize());
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			ASSERT_EQ(m2.getData()[i][j], m.getData()[i][j]);
}