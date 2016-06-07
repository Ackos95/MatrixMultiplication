#ifndef _SERIAL_ALGORITHM_H_
#define _SERIAL_ALGORITHM_H_

#include <Matrix.h>
#include <ApplicationData.h>

extern int LOWER_BOUND;

struct MatrixWrapper
{
	Matrix<TYPE>* matrix;
	int i, j;
	int n;

	MatrixWrapper(Matrix<TYPE> *, int, int, int);
};

void serialAddMatrices(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Dest, bool minus = false);
void serialAddMatricesPlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus = false);

void serialMultiplyRecursive(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Dest);
void serialMultiplyRecursivePlain(int ** A, int** B, int** C, int ia, int ja, int ib, int jb, int ic, int jc, int n);

void serialMultiplySimple(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Src3);
void serialMultiplySimplePlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n);

void serialMultiply(bool = true);
void serialMultiplySimple(bool = true);

#endif