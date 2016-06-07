#include <gtest\gtest.h>
#include <ParallelAlgorithm.h>

TEST(ParallelAlgorithmTests, parallelAddMatricesTest)
{
	Matrix<TYPE> src1(4), src2(4), dest(4);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			src1.getData()[i][j] = src2.getData()[i][j] = i * 10;

	// sum whole matrices
	ASSERT_NO_THROW(parallelAddMatricesPlain(src1.getData(), src2.getData(), dest.getData(), 0, 0, 0, 0, 0, 0, 4));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ASSERT_EQ(dest.getData()[i][j], i * 20);

	// sum A11 submatrix with B22 submatrix and place it in C11 submatrix
	ASSERT_NO_THROW(parallelAddMatricesPlain(src1.getData(), src2.getData(), dest.getData(), 0, 0, 2, 2, 0, 0, 2));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			ASSERT_EQ(dest.getData()[i][j], i * 20 + 20);

	// subtract two (sub)matrices
	ASSERT_NO_THROW(serialAddMatricesPlain(src2.getData(), src2.getData(), src2.getData(), 1, 1, 1, 1, 1, 1, 2, true));


	for (int i = 1; i < 3; i++)
		for (int j = 1; j < 3; j++)
			ASSERT_EQ(src2.getData()[i][j], 0);
}


// few multiplication tests
// more will be with in integration tests
// with file i/o 
// in each test case all 4 matrix multiplication functions are tested
// simple, simplePlain, recursive, recursivePlain
TEST(ParallelAlgorithmTests, parallelMatrixMultiplicationTest)
{
	Matrix<TYPE> src1(2), src2(2), dest(2);

	LOWER_BOUND = 1;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			src1.getData()[i][j] = src2.getData()[i][j] = 1;

	ParallelMutliplyTask* p = new(tbb::task::allocate_root()) ParallelMutliplyTask(src1.getData(), src2.getData(), dest.getData(), 0, 0, 0, 0, 0, 0, 2);
	p->execute();

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			ASSERT_EQ(dest.getData()[i][j], 2);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			src1.getData()[i][j] = (i == j ? 1 : 0);
			src2.getData()[i][j] = rand() % 20 + 1;
			dest.getData()[i][j] = 0;
		}

	ParallelMutliplyTask* p1 = new(tbb::task::allocate_root()) ParallelMutliplyTask(src1.getData(), src2.getData(), dest.getData(), 0, 0, 0, 0, 0, 0, 2);
	p1->execute();

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			ASSERT_EQ(dest.getData()[i][j], src2.getData()[i][j]);

	Matrix<TYPE> src41(4), src42(4), dest4(4);
	int src41int[]{ 17, -4, 8, 3, 22, 0, 1, 4, -5, 2, -1, 10, 12, 7, 6, 6 };
	int src42int[]{ 2, 1, 1, 4, -1, 2, -1, -1, -3, -2, 2, 2, 1, 0, 0, 1 };
	int dest4int[]{ 17, -7, 37, 91, 45, 20, 24, 94, 1, 1, -9, -14, 5, 14, 17, 59 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			src41.getData()[i][j] = src41int[i * 4 + j];
			src42.getData()[i][j] = src42int[i * 4 + j];
		}

	ParallelMutliplyTask* p4 = new(tbb::task::allocate_root()) ParallelMutliplyTask(src41.getData(), src42.getData(), dest4.getData(), 0, 0, 0, 0, 0, 0, 4);
	p4->execute();

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ASSERT_EQ(dest4.getData()[i][j], dest4int[i * 4 + j]);

	LOWER_BOUND = 32;
}