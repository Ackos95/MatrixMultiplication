#include "ParallelAlgorithm.h"


ParallelMutliplyTask::ParallelMutliplyTask(int ** A, int ** B, int ** C, int ia, int ja, int ib, int jb, int ic, int jc, int n)
	: A(A), B(B), C(C), ia(ia), ib(ib), ic(ic), ja(ja), jb(jb), jc(jc), n(n)
{
}

tbb::task * ParallelMutliplyTask::execute()
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


		parallelAddMatricesPlain(B, B, S, ib, jb2, ib2, jb2, is1, js1, n / 2, true);  // S1 = B12 - B22
		parallelAddMatricesPlain(A, A, S, ia, ja, ia, ja2, is2, js2, n / 2);  // S2 = A11 + A12
		parallelAddMatricesPlain(A, A, S, ia2, ja, ia2, ja2, is3, js3, n / 2);  // S3 = A21 + A22
		parallelAddMatricesPlain(B, B, S, ib2, jb, ib, jb, is4, js4, n / 2, true);  // S4 = B21 - B11
		parallelAddMatricesPlain(A, A, S, ia, ja, ia2, ja2, is5, js5, n / 2);  // S5 = A11 + A22
		parallelAddMatricesPlain(B, B, S, ib, jb, ib2, jb2, is6, js6, n / 2);  // S6 = B11 + B22
		parallelAddMatricesPlain(A, A, S, ia, ja2, ia2, ja2, is7, js7, n / 2, true);  // S7 = A12 - A22
		parallelAddMatricesPlain(B, B, S, ib2, jb, ib2, jb2, is8, js8, n / 2);  // S8 = B21 + B22
		parallelAddMatricesPlain(A, A, S, ia, ja, ia2, ja, is9, js9, n / 2, true);  // S9 = A11 - A21
		parallelAddMatricesPlain(B, B, S, ib, jb, ib, jb2, is10, js10, n / 2);  // S10 = B11 + B12

		ParallelMutliplyTask* p1 = new(allocate_child()) ParallelMutliplyTask(A, S, S, ia, ja, is1, js1, ip1, jp1, n / 2);  // P1 = A11 * S1
		ParallelMutliplyTask* p2 = new(allocate_child()) ParallelMutliplyTask(S, B, S, is2, js2, ib2, jb2, ip2, jp2, n / 2);  // P2 = S2 * B22
		ParallelMutliplyTask* p3 = new(allocate_child()) ParallelMutliplyTask(S, B, S, is3, js3, ib, jb, ip3, jp3, n / 2);  // P3 = S3 * B11
		ParallelMutliplyTask* p4 = new(allocate_child()) ParallelMutliplyTask(A, S, S, ia2, ja2, is4, js4, ip4, jp4, n / 2);  // P4 = A22 * S4
		ParallelMutliplyTask* p5 = new(allocate_child()) ParallelMutliplyTask(S, S, S, is5, js5, is6, js6, ip5, jp5, n / 2);  // P5 = S5 * S6
		ParallelMutliplyTask* p6 = new(allocate_child()) ParallelMutliplyTask(S, S, S, is7, js7, is8, js8, ip6, jp6, n / 2);  // P6 = S7 * S8
		ParallelMutliplyTask* p7 = new(allocate_child()) ParallelMutliplyTask(S, S, P7, is9, js9, is10, js10, 0, 0, n / 2);  // P7 = S9 * S10

		set_ref_count(8);

		spawn(*p1);
		spawn(*p2);
		spawn(*p3);
		spawn(*p4);
		spawn(*p5);
		spawn(*p6);
		// p7->execute();  theorethcally faster but practically not?
		// wait_for_all();
		spawn_and_wait_for_all(*p7);

		parallelAddMatricesPlain(S, S, C, ip5, jp5, ip4, jp4, ic, jc, n / 2);  // half step
		parallelAddMatricesPlain(C, S, C, ic, jc, ip2, jp2, ic, jc, n / 2, true);  // half step
		parallelAddMatricesPlain(C, S, C, ic, jc, ip6, jp6, ic, jc, n / 2);  // C11 = P5 + P4 - P2 + P6
		parallelAddMatricesPlain(S, S, C, ip1, jp1, ip2, jp2, ic, jc2, n / 2);  // C12 = P1 + P2
		parallelAddMatricesPlain(S, S, C, ip3, jp3, ip4, jp4, ic2, jc, n / 2);  // C21 = P3 + P4
		parallelAddMatricesPlain(S, S, C, ip5, jp5, ip1, jp1, ic2, jc2, n / 2);  // half step
		parallelAddMatricesPlain(C, S, C, ic2, jc2, ip3, jp3, ic2, jc2, n / 2, true);  // half step
		parallelAddMatricesPlain(C, P7, C, ic2, jc2, 0, 0, ic2, jc2, n / 2, true);  // C22 = P5 + P1 - P3 - P7

		/*
		delete p1;
		delete p2;
		delete p3;
		delete p4;
		delete p5;
		delete p6;
		delete p7;
		*/
	}
	return nullptr;
}

ParallelAdding2D::ParallelAdding2D(int** A, int ** B, int ** C, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus)
	: A(A), B(B), C(C), ia(ia), ja(ja), ib(ib), jb(jb), ic(ic), jc(jc), n(n), minus(minus), i(i)
{
}

void ParallelAdding2D::operator()(tbb::blocked_range2d<int>& range) const
{
	for (int i = range.rows().begin(); i != range.rows().end(); i++)
		for (int j = range.cols().begin(); j != range.cols().end(); j++)
			C[ic + i][jc + j] = A[ia + i][ja + j] + (B[ib + i][jb + j] * (minus ? -1 : 1));
}


void parallelAddMatricesPlain(int** _Src1, int** _Src2, int** _Dest, int ia, int ja, int ib, int jb, int ic, int jc, int n, bool minus)
{
	if (n < 8192)
		serialAddMatricesPlain(_Src1, _Src2, _Dest, ia, ja, ib, jb, ic, jc, n, minus);
	else
		tbb::parallel_for(tbb::blocked_range2d<int>(0, n, 1024, 0, n, 1024), ParallelAdding2D(_Src1, _Src2, _Dest, ia, ja, ib, jb, ic, jc, n, minus));
}

void parallelMultiply()
{
	ApplicationData* appData = ApplicationData::getInstance();
	if (!appData->getA() || !appData->getB() || !appData->getCp())
		throw std::runtime_error("Undefined matrixes");

	ParallelMutliplyTask* p = new( tbb::task::allocate_root() ) ParallelMutliplyTask(appData->getA()->getData(), appData->getB()->getData(), appData->getCp()->getData(),
			0, 0, 0, 0, 0, 0, appData->getA()->getSize());
	tbb::task::spawn_root_and_wait(*p);

	// delete p;
}