#include "ApplicationData.h"

/// set Singlethon pattern variable to null
ApplicationData* ApplicationData::instance = nullptr;

ApplicationData::ApplicationData()
	: verbose(false), A(nullptr), B(nullptr),
	C(nullptr), Cp(nullptr), serial(true), parallel(true)
{
	inputPath = new char[STRING_SIZE];
	serialOutputPath = new char[STRING_SIZE];
	parallelOutputPath = new char[STRING_SIZE];

	inputPath[0] = '\0';
	serialOutputPath[0] = '\0';
	parallelOutputPath[0] = '\0';
}

ApplicationData::~ApplicationData()
{
	delete[] inputPath;
	delete[] serialOutputPath;
	delete[] parallelOutputPath;

	deleteAllocatedMatrices();
}

void ApplicationData::reset()
{
	inputPath[0] = '\0';
	serialOutputPath[0] = '\0';
	parallelOutputPath[0] = '\0';
	verbose = false;
	serial = parallel = true;

	deleteAllocatedMatrices();
	A = nullptr;
	B = nullptr;
	C = nullptr;
	Cp = nullptr;
}

void ApplicationData::deleteAllocatedMatrices()
{
	if (A) delete A;
	if (B) delete B;
	if (C) delete C;
	if (Cp) delete Cp;
}

ApplicationData* ApplicationData::getInstance()
{
	if (!instance)
		instance = new ApplicationData;
	return instance;
}

void ApplicationData::setInputPath(const char* path)
{
	if (strcpy_s(inputPath, STRING_SIZE, path))
		throw std::runtime_error("Couldn't copy input path inside object.");
}

void ApplicationData::setSerialOutputPath(const char* path)
{
	if (strcpy_s(serialOutputPath, STRING_SIZE, path))
		throw std::runtime_error("Couldn't copy output path inside object.");
}

void ApplicationData::setParallelOutputPath(const char* path)
{
	if (strcpy_s(parallelOutputPath, STRING_SIZE, path))
		throw std::runtime_error("Couldn't copy output path inside object.");
}

void ApplicationData::setVerbose(bool v)
{
	verbose = v;
}

void ApplicationData::setSerial(bool s)
{
	serial = s;
}

void ApplicationData::setParallel(bool p)
{
	parallel = p;
}

void ApplicationData::setMatrixA(const Matrix<TYPE>& _A)
{
	if (B && B->getSize() != _A.getSize() || C && C->getSize() != _A.getSize() || Cp && Cp->getSize() != _A.getSize())
		throw std::runtime_error("Matrices must be same size");
	double log2Size = log2((double) _A.getSize());
	if (log2Size - (int)log2Size != 0)
		throw std::runtime_error("Matrix size must be 2^k, k>=0");

	A = new Matrix<TYPE>(_A);
}

void ApplicationData::setMatrixB(const Matrix<TYPE>& _B)
{
	if (A && A->getSize() != _B.getSize() || C && C->getSize() != _B.getSize() || Cp && Cp->getSize() != _B.getSize())
		throw std::runtime_error("Matrices must be same size");
	double log2Size = log2((double)_B.getSize());
	if (log2Size - (int)log2Size != 0)
		throw std::runtime_error("Matrix size must be 2^k, k>=0");

	B = new Matrix<TYPE>(_B);
}

void ApplicationData::setMatrixC(const Matrix<TYPE>& _C)
{
	if (A && A->getSize() != _C.getSize() || B && B->getSize() != _C.getSize() || Cp && Cp->getSize() != _C.getSize())
		throw std::runtime_error("Matrices must be same size");
	double log2Size = log2((double)_C.getSize());
	if (log2Size - (int)log2Size != 0)
		throw std::runtime_error("Matrix size must be 2^k, k>=0");

	C = new Matrix<TYPE>(_C);
}

void ApplicationData::setMatrixCp(const Matrix<TYPE>& _C)
{
	if (A && A->getSize() != _C.getSize() || B && B->getSize() != _C.getSize() || C && C->getSize() != _C.getSize())
		throw std::runtime_error("Matrices must be same size");
	double log2Size = log2((double)_C.getSize());
	if (log2Size - (int)log2Size != 0)
		throw std::runtime_error("Matrix size must be 2^k, k>=0");

	Cp = new Matrix<TYPE>(_C);
}

const char* ApplicationData::getInputPath() const
{
	return inputPath;
}

const char* ApplicationData::getSerialOutputPath() const
{
	return serialOutputPath;
}

const char* ApplicationData::getParallelOutputPath() const
{
	return parallelOutputPath;
}

bool ApplicationData::isVerbose() const
{
	return verbose;
}

bool ApplicationData::shouldDoSerial() const
{
	return serial;
}

bool ApplicationData::shouldDoParallel() const
{
	return parallel;
}

Matrix<TYPE>* ApplicationData::getA()
{
	return A;
}

Matrix<TYPE>* ApplicationData::getB()
{
	return B;
}

Matrix<TYPE>* ApplicationData::getC()
{
	return C;
}

Matrix<TYPE>* ApplicationData::getCp()
{
	return Cp;
}