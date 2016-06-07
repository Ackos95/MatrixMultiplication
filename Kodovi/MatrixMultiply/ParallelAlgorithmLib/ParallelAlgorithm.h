#ifndef _PARALLEL_ALGORITHM_H_
#define _PARALLEL_ALGORITHM_H_

#include <Matrix.h>
#include <ApplicationData.h>
#include <SerialAlgorithm.h>
#include <tbb\tbb.h>
#include <tbb\task.h>
#include <tbb\parallel_for.h>


class ParallelMutliplyTask : public tbb::task
{
private:
	int** A;
	int** B;
	int** C;

	int ia, ja, ib, jb, ic, jc, n;

public:
	ParallelMutliplyTask(int** A, int** B, int** C, int ia, int ja, int ib, int jb, int ic, int jc, int n);

	tbb::task* execute();
};


class ParallelAdding2D
{
private:
	int** A;
	int** B;
	int** C;

	int ia, ja, ib, jb, ic, jc, n, i;
	bool minus;

public:
	ParallelAdding2D(int** A, int ** B, int ** C, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus);

	void operator()(tbb::blocked_range2d<int>& range) const;
};


void parallelAddMatricesPlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus = false);

void parallelMultiply();

#endif