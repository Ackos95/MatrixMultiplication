#include <gtest\gtest.h>
#include <SerialAlgorithm.h>


TEST(SerialAlgorithmTests, serialAddMatricesTest)
{
	Matrix<TYPE> src1(4), src2(4), nonPlain(4), plain(4);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			src1.getData()[i][j] = src2.getData()[i][j] = i * 10;
	
	// sum whole matrices
	ASSERT_NO_THROW(serialAddMatrices(&MatrixWrapper(&src1, 0, 0, 4), &MatrixWrapper(&src2, 0, 0, 4), &MatrixWrapper(&nonPlain, 0, 0, 4)));
	ASSERT_NO_THROW(serialAddMatricesPlain(src1.getData(), src2.getData(), plain.getData(), 0, 0, 0, 0, 0, 0, 4));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			ASSERT_EQ(nonPlain.getData()[i][j], i * 20);
			ASSERT_EQ(plain.getData()[i][j], i * 20);
		}

	// sum A11 submatrix with B22 submatrix and place it in C11 submatrix
	ASSERT_NO_THROW(serialAddMatrices(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 2, 2, 2), &MatrixWrapper(&nonPlain, 0, 0, 2)));
	ASSERT_NO_THROW(serialAddMatricesPlain(src1.getData(), src2.getData(), plain.getData(), 0, 0, 2, 2, 0, 0, 2));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			ASSERT_EQ(nonPlain.getData()[i][j], i * 20 + 20);
			ASSERT_EQ(plain.getData()[i][j], i * 20 + 20);
		}

	// subtract two (sub)matrices
	ASSERT_NO_THROW(serialAddMatrices(&MatrixWrapper(&src1, 1, 1, 2), &MatrixWrapper(&src1, 1, 1, 2), &MatrixWrapper(&src1, 1, 1, 2), true));
	ASSERT_NO_THROW(serialAddMatricesPlain(src2.getData(), src2.getData(), src2.getData(), 1, 1, 1, 1, 1, 1, 2, true));


	for (int i = 1; i < 3; i++)
		for (int j = 1; j < 3; j++)
		{
			ASSERT_EQ(src1.getData()[i][j], 0);
			ASSERT_EQ(src2.getData()[i][j], 0);
		}

	// unequal sizes of matrices
	ASSERT_ANY_THROW(serialAddMatrices(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 0, 0, 4), &MatrixWrapper(&nonPlain, 0, 0, 3)));
}


// few multiplication tests
// more will be with in integration tests
// with file i/o 
// in each test case all 4 matrix multiplication functions are tested
// simple, simplePlain, recursive, recursivePlain
TEST(SerialAlgorithmTests, matrixMultiplicationTest)
{
	Matrix<TYPE> src1(2), src2(2), nonPlain(2), plain(2), naive(2), naivePlain(2);

	LOWER_BOUND = 1;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			src1.getData()[i][j] = src2.getData()[i][j] = 1;

	ASSERT_NO_THROW(serialMultiplySimple(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 0, 0, 2), &MatrixWrapper(&naive, 0, 0, 2)));
	ASSERT_NO_THROW(serialMultiplySimplePlain(src1.getData(), src2.getData(), naivePlain.getData(), 0, 0, 0, 0, 0, 0, 2));
	ASSERT_NO_THROW(serialMultiplyRecursive(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 0, 0, 2), &MatrixWrapper(&nonPlain, 0, 0, 2)));
	ASSERT_NO_THROW(serialMultiplyRecursivePlain(src1.getData(), src2.getData(), plain.getData(), 0, 0, 0, 0, 0, 0, 2));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			ASSERT_EQ(naive.getData()[i][j], 2);
			ASSERT_EQ(naivePlain.getData()[i][j], 2);
			ASSERT_EQ(nonPlain.getData()[i][j], 2);
			ASSERT_EQ(plain.getData()[i][j], 2);
		}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			src1.getData()[i][j] = (i == j ? 1 : 0);
			src2.getData()[i][j] = rand() % 20 + 1;
			nonPlain.getData()[i][j] = plain.getData()[i][j] 
				= naive.getData()[i][j] = naivePlain.getData()[i][j] = 0;
		}

	ASSERT_NO_THROW(serialMultiplySimple(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 0, 0, 2), &MatrixWrapper(&naive, 0, 0, 2)));
	ASSERT_NO_THROW(serialMultiplySimplePlain(src1.getData(), src2.getData(), naivePlain.getData(), 0, 0, 0, 0, 0, 0, 2));
	ASSERT_NO_THROW(serialMultiplyRecursive(&MatrixWrapper(&src1, 0, 0, 2), &MatrixWrapper(&src2, 0, 0, 2), &MatrixWrapper(&nonPlain, 0, 0, 2)));
	ASSERT_NO_THROW(serialMultiplyRecursivePlain(src1.getData(), src2.getData(), plain.getData(), 0, 0, 0, 0, 0, 0, 2));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			ASSERT_EQ(naive.getData()[i][j], src2.getData()[i][j]);
			ASSERT_EQ(naivePlain.getData()[i][j], src2.getData()[i][j]);
			ASSERT_EQ(nonPlain.getData()[i][j], src2.getData()[i][j]);
			ASSERT_EQ(plain.getData()[i][j], src2.getData()[i][j]);
		}

	Matrix<TYPE> src41(4), src42(4), nonPlain4(4), plain4(4), naive4(4), naivePlain4(4);
	int src41int[]{ 17, -4, 8, 3, 22, 0, 1, 4, -5, 2, -1, 10, 12, 7, 6, 6 };
	int src42int[]{ 2, 1, 1, 4, -1, 2, -1, -1, -3, -2, 2, 2, 1, 0, 0, 1 };
	int dest4int[]{ 17, -7, 37, 91, 45, 20, 24, 94, 1, 1, -9, -14, 5, 14, 17, 59 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			src41.getData()[i][j] = src41int[i * 4 + j];
			src42.getData()[i][j] = src42int[i * 4 + j];
		}

	ASSERT_NO_THROW(serialMultiplySimple(&MatrixWrapper(&src41, 0, 0, 4), &MatrixWrapper(&src42, 0, 0, 4), &MatrixWrapper(&naive4, 0, 0, 4)));
	ASSERT_NO_THROW(serialMultiplySimplePlain(src41.getData(), src42.getData(), naivePlain4.getData(), 0, 0, 0, 0, 0, 0, 4));
	ASSERT_NO_THROW(serialMultiplyRecursive(&MatrixWrapper(&src41, 0, 0, 4), &MatrixWrapper(&src42, 0, 0, 4), &MatrixWrapper(&nonPlain4, 0, 0, 4)));
	ASSERT_NO_THROW(serialMultiplyRecursivePlain(src41.getData(), src42.getData(), plain4.getData(), 0, 0, 0, 0, 0, 0, 4));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			ASSERT_EQ(naive4.getData()[i][j], dest4int[i * 4 + j]);
			ASSERT_EQ(naivePlain4.getData()[i][j], dest4int[i * 4 + j]);
			ASSERT_EQ(nonPlain4.getData()[i][j], dest4int[i * 4 + j]);
			ASSERT_EQ(plain4.getData()[i][j], dest4int[i * 4 + j]);
		}

	LOWER_BOUND = 32;
}