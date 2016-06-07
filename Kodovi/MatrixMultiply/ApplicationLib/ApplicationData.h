#ifndef _APPLICATION_DATA_H_
#define _APPLICATION_DATA_H_

#include <stdexcept>
#include <cstdlib>
#include "Matrix.h"

class ApplicationData
{
private:
	static const int STRING_SIZE = 512;
	static ApplicationData* instance;

	char* inputPath;
	char* serialOutputPath;
	char* parallelOutputPath;
	bool verbose;
	bool serial, parallel;

	Matrix<TYPE> *A, *B, *C, *Cp;
	ApplicationData();
	void deleteAllocatedMatrices();

public:
	static ApplicationData* getInstance();
	~ApplicationData();

	void reset();

	void setInputPath(const char *);
	void setSerialOutputPath(const char *);
	void setParallelOutputPath(const char *);
	void setVerbose(bool);
	void setSerial(bool);
	void setParallel(bool);
	void setMatrixA(const Matrix<TYPE>&);
	void setMatrixB(const Matrix<TYPE>&);
	void setMatrixC(const Matrix<TYPE>&);
	void setMatrixCp(const Matrix<TYPE>&);

	const char* getInputPath() const;
	const char* getSerialOutputPath() const;
	const char* getParallelOutputPath() const;
	bool isVerbose() const;
	bool shouldDoSerial() const;
	bool shouldDoParallel() const;
	Matrix<TYPE>* getA();
	Matrix<TYPE>* getB();
	Matrix<TYPE>* getC();
	Matrix<TYPE>* getCp();
};

#endif