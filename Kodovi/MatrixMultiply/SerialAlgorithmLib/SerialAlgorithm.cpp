#include "SerialAlgorithm.h"

// define constant to 32 (it will be changed for test purposes, so it can not be declared as const int or with #define)
int LOWER_BOUND = 32;

MatrixWrapper::MatrixWrapper(Matrix<TYPE>* m, int i_, int j_, int n_)
	: matrix(m), i(i_), j(j_), n(n_)
{
}


void serialAddMatrices(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Dest, bool minus)
{
	if (_Src1->n != _Src2->n || _Src1->n != _Dest->n || _Src2->n != _Dest->n)
		throw std::runtime_error("Invalid parameters!");

	for (int i = 0; i < _Src1->n; i++)
		for (int j = 0; j < _Src1->n; j++)
			_Dest->matrix->getData()[i + _Dest->i][j + _Dest->j] = _Src1->matrix->getData()[i + _Src1->i][j + _Src1->j]
			+ (_Src2->matrix->getData()[i + _Src2->i][j + _Src2->j] * (minus ? -1 : 1));
}

void serialAddMatricesPlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			_Dest[ic + i][jc + j] = _Src1[ia + i][ja + j] + (_Src2[ib + i][jb + j] * (minus ? -1 : 1));
}

void serialMultiplyRecursive(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Dest)
{
	if (_Src1->n != _Src2->n || _Src1->n != _Dest->n || _Src2->n != _Dest->n)
		throw std::runtime_error("Invalid parameters!");

	if (_Src1->n <= LOWER_BOUND)
		serialMultiplySimple(_Src1, _Src2, _Dest);

	else
	{
		// Create side matrices
		Matrix<TYPE> S1(_Src1->n / 2), S2(_Src1->n / 2), S3(_Src1->n / 2),
			S4(_Src1->n / 2), S5(_Src1->n / 2), S6(_Src1->n / 2), S7(_Src1->n / 2),
			S8(_Src1->n / 2), S9(_Src1->n / 2), S10(_Src1->n / 2), P1(_Src1->n / 2),
			P2(_Src1->n / 2), P3(_Src1->n / 2), P4(_Src1->n / 2), P5(_Src1->n / 2),
			P6(_Src1->n / 2), P7(_Src1->n / 2);

		serialAddMatrices(&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2), true);  // S1 = B12 - B22
		serialAddMatrices(&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(&S2, 0, 0, _Src1->n / 2));  // S2 = A11 + A12
		serialAddMatrices(&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(&S3, 0, 0, _Src1->n / 2));  // S3 = A21 + A22
		serialAddMatrices(&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j, _Src2->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->j, _Src2->n / 2),
			&MatrixWrapper(&S4, 0, 0, _Src1->n / 2), true);  // S4 = B21 - B11
		serialAddMatrices(&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(&S5, 0, 0, _Src1->n / 2));  // S5 = A11 + A22
		serialAddMatrices(&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->i, _Src2->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(&S6, 0, 0, _Src1->n / 2));  // S6 = B11 + B22
		serialAddMatrices(&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(&S7, 0, 0, _Src1->n / 2), true);  // S7 = A12 - A22
		serialAddMatrices(&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j, _Src2->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(&S8, 0, 0, _Src1->n / 2));  // S8 = B21 + B22
		serialAddMatrices(&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(&S9, 0, 0, _Src1->n / 2), true);  // S9 = A11 - A21
		serialAddMatrices(&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->j, _Src2->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(&S10, 0, 0, _Src1->n / 2));  // S10 = B11 + B12

		serialMultiplyRecursive(&MatrixWrapper(_Src1->matrix, _Src1->i, _Src1->j, _Src1->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P1, 0, 0, _Src1->n / 2));  // P1 = A11 * S1
		serialMultiplyRecursive(&MatrixWrapper(&S2, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i + _Src2->n / 2, _Src2->j + _Src2->n / 2, _Src2->n / 2),
			&MatrixWrapper(&P2, 0, 0, _Src1->n / 2));  // P2 = S2 * B22
		serialMultiplyRecursive(&MatrixWrapper(&S3, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Src2->matrix, _Src2->i, _Src2->i, _Src2->n / 2),
			&MatrixWrapper(&P3, 0, 0, _Src1->n / 2));  // P3 = S3 * B11
		serialMultiplyRecursive(&MatrixWrapper(_Src1->matrix, _Src1->i + _Src1->n / 2, _Src1->j + _Src1->n / 2, _Src1->n / 2),
			&MatrixWrapper(&S4, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P4, 0, 0, _Src1->n / 2));  // P4 = A22 * S4
		serialMultiplyRecursive(&MatrixWrapper(&S5, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S6, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P5, 0, 0, _Src1->n / 2));  // P5 = S5 * S6
		serialMultiplyRecursive(&MatrixWrapper(&S7, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S8, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P6, 0, 0, _Src1->n / 2));  // P6 = S7 * S8
		serialMultiplyRecursive(&MatrixWrapper(&S9, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S10, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P7, 0, 0, _Src1->n / 2));  // P7 = S9 * S10

		serialAddMatrices(&MatrixWrapper(&P5, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P4, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2));  // Temp matrix P5 + P4
		serialAddMatrices(&MatrixWrapper(&S1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P2, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2), true);  // Temp matrix P5 + P4 - P2
		serialAddMatrices(&MatrixWrapper(&S1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P6, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Dest->matrix, _Dest->i, _Dest->j, _Dest->n / 2));  // C11 = P5 + P4 - P2 + P6
		serialAddMatrices(&MatrixWrapper(&P1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P2, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Dest->matrix, _Dest->i, _Dest->j + _Dest->n / 2, _Dest->n / 2));  // C12 = P1 + P2
		serialAddMatrices(&MatrixWrapper(&P3, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P4, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Dest->matrix, _Dest->i + _Dest->n / 2, _Dest->j, _Dest->n / 2));  // C21 = P3 + P4
		serialAddMatrices(&MatrixWrapper(&P5, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2));  // Temp matrix P5 + P1
		serialAddMatrices(&MatrixWrapper(&S1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P3, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&S1, 0, 0, _Src1->n / 2), true);  // Temp matrix P5 + P1 - P3
		serialAddMatrices(&MatrixWrapper(&S1, 0, 0, _Src1->n / 2),
			&MatrixWrapper(&P7, 0, 0, _Src1->n / 2),
			&MatrixWrapper(_Dest->matrix, _Dest->i + _Dest->n / 2, _Dest->j + _Dest->n / 2, _Dest->n / 2), true);  // C22 = P5 + P1 - P3 - P7
	}
}

void serialMultiplyRecursivePlain(int** A, int** B, int** C, int ia, int ja, int ib, int jb, int ic, int jc, int n)
{
	if (n <= LOWER_BOUND)
		serialMultiplySimplePlain(A, B, C, ia, ja, ib, jb, ic, jc, n);
	else
	{
		// Create side matrices
		// For optimization 16 matrices (sized n/2 x n/2) are placed in one big matrix S (2n x 2n)
		// So that heap allocation will be called only for two matrices (2n x 2n and one n/2 x n/2)
		// instead for 17 times (n/2 x n/2)
		Matrix<TYPE> sideMat(2 * n), sideMatH(n / 2);

		int** S = sideMat.getData();
		int** P7 = sideMatH.getData();

		// isolate indexes into variables for easier access to submatrixes (and to avoid redundant calculations)
		int is1, is2, is3, is4, is5, is6, is7, is8, is9, is10, ip1, ip2, ip3, ip4, ip5, ip6;
		int js1, js2, js3, js4, js5, js6, js7, js8, js9, js10, jp1, jp2, jp3, jp4, jp5, jp6;
		int ia2 = ia + n / 2, ja2 = ja + n / 2, ib2 = ib + n / 2, jb2 = jb + n / 2, ic2 = ic + n / 2, jc2 = jc + n / 2;

		is1 = is5 = is9 = ip3 = js1 = js2 = js3 = js4 = 0;
		is2 = is6 = is10 = ip4 = js5 = js6 = js7 = js8 = n / 2;
		is3 = is7 = ip1 = ip5 = js9 = js10 = jp1 = jp2 = n;
		is4 = is8 = ip2 = ip6 = jp3 = jp4 = jp5 = jp6 = 3 * n / 2;

		serialAddMatricesPlain(B, B, S, ib, jb2, ib2, jb2, is1, js1, n / 2, true);  // S1 = B12 - B22
		serialAddMatricesPlain(A, A, S, ia, ja, ia, ja2, is2, js2, n / 2);  // S2 = A11 + A12
		serialAddMatricesPlain(A, A, S, ia2, ja, ia2, ja2, is3, js3, n / 2);  // S3 = A21 + A22
		serialAddMatricesPlain(B, B, S, ib2, jb, ib, jb, is4, js4, n / 2, true);  // S4 = B21 - B11
		serialAddMatricesPlain(A, A, S, ia, ja, ia2, ja2, is5, js5, n / 2);  // S5 = A11 + A22
		serialAddMatricesPlain(B, B, S, ib, jb, ib2, jb2, is6, js6, n / 2);  // S6 = B11 + B22
		serialAddMatricesPlain(A, A, S, ia, ja2, ia2, ja2, is7, js7, n / 2, true);  // S7 = A12 - A22
		serialAddMatricesPlain(B, B, S, ib2, jb, ib2, jb2, is8, js8, n / 2);  // S8 = B21 + B22
		serialAddMatricesPlain(A, A, S, ia, ja, ia2, ja, is9, js9, n / 2, true);  // S9 = A11 - A21
		serialAddMatricesPlain(B, B, S, ib, jb, ib, jb2, is10, js10, n / 2);  // S10 = B11 + B12

		serialMultiplyRecursivePlain(A, S, S, ia, ja, is1, js1, ip1, jp1, n / 2);  // P1 = A11 * S1
		serialMultiplyRecursivePlain(S, B, S, is2, js2, ib2, jb2, ip2, jp2, n / 2);  // P2 = S2 * B22
		serialMultiplyRecursivePlain(S, B, S, is3, js3, ib, jb, ip3, jp3, n / 2);  // P3 = S3 * B11
		serialMultiplyRecursivePlain(A, S, S, ia2, ja2, is4, js4, ip4, jp4, n / 2);  // P4 = A22 * S4
		serialMultiplyRecursivePlain(S, S, S, is5, js5, is6, js6, ip5, jp5, n / 2);  // P5 = S5 * S6
		serialMultiplyRecursivePlain(S, S, S, is7, js7, is8, js8, ip6, jp6, n / 2);  // P6 = S7 * S8
		serialMultiplyRecursivePlain(S, S, P7, is9, js9, is10, js10, 0, 0, n / 2);  // P7 = S9 * S10

		serialAddMatricesPlain(S, S, C, ip5, jp5, ip4, jp4, ic, jc, n / 2);  // half step
		serialAddMatricesPlain(C, S, C, ic, jc, ip2, jp2, ic, jc, n / 2, true);  // half step
		serialAddMatricesPlain(C, S, C, ic, jc, ip6, jp6, ic, jc, n / 2);  // C11 = P5 + P4 - P2 + P6
		serialAddMatricesPlain(S, S, C, ip1, jp1, ip2, jp2, ic, jc2, n / 2);  // C12 = P1 + P2
		serialAddMatricesPlain(S, S, C, ip3, jp3, ip4, jp4, ic2, jc, n / 2);  // C21 = P3 + P4
		serialAddMatricesPlain(S, S, C, ip5, jp5, ip1, jp1, ic2, jc2, n / 2);  // half step
		serialAddMatricesPlain(C, S, C, ic2, jc2, ip3, jp3, ic2, jc2, n / 2, true);  // half step
		serialAddMatricesPlain(C, P7, C, ic2, jc2, 0, 0, ic2, jc2, n / 2, true);  // C22 = P5 + P1 - P3 - P7
	}
}

void serialMultiplySimple(MatrixWrapper* _Src1, MatrixWrapper* _Src2, MatrixWrapper* _Dest)
{
	for (int i = 0; i < _Src1->n; i++)
		for (int j = 0; j < _Src1->n; j++)
		{
			_Dest->matrix->getData()[_Dest->i + i][_Dest->j + j] = 0;
			for (int k = 0; k < _Src1->n; k++)
				_Dest->matrix->getData()[_Dest->i + i][_Dest->j + j] += _Src1->matrix->getData()[_Src1->i + i][_Src1->j+ k]
				* _Src2->matrix->getData()[_Src2->i + k][_Src2->j + j];
		}
}

void serialMultiplySimplePlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			_Dest[ic + i][jc + j] = 0;
			for (int k = 0; k < n; k++)
				_Dest[ic + i][jc + j] += _Src1[ia + i][ja + k] * _Src2[ib + k][jb + j];
		}
}

void serialMultiply(bool plain)
{
	ApplicationData* appData = ApplicationData::getInstance();
	if (!appData->getA() || !appData->getB() || !appData->getC())
		throw std::runtime_error("Undefined matrixes");

	if (!plain)
		serialMultiplyRecursive(&MatrixWrapper(appData->getA(), 0, 0, appData->getA()->getSize()),
			&MatrixWrapper(appData->getB(), 0, 0, appData->getB()->getSize()),
			&MatrixWrapper(appData->getC(), 0, 0, appData->getC()->getSize()));
	else
		serialMultiplyRecursivePlain(appData->getA()->getData(), appData->getB()->getData(), appData->getC()->getData(),
			0, 0, 0, 0, 0, 0, appData->getA()->getSize());
}

void serialMultiplySimple(bool plain)
{
	ApplicationData* appData = ApplicationData::getInstance();
	if (!appData->getA() || !appData->getB() || !appData->getC())
		throw std::runtime_error("Undefined matrixes");

	if (!plain)
		serialMultiplySimple(&MatrixWrapper(appData->getA(), 0, 0, appData->getA()->getSize()),
			&MatrixWrapper(appData->getB(), 0, 0, appData->getB()->getSize()),
			&MatrixWrapper(appData->getC(), 0, 0, appData->getC()->getSize()));
	else
		serialMultiplySimplePlain(appData->getA()->getData(), appData->getB()->getData(), appData->getC()->getData(),
			0, 0, 0, 0, 0, 0, appData->getA()->getSize());
}