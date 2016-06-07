#include "InputHandler.h"

void parseInputFile()
{
	ApplicationData* appData = ApplicationData::getInstance();
	if (appData->getA() && appData->getB())
		return;

	std::ifstream fin(appData->getInputPath());
	if (!fin.is_open())
		throw std::runtime_error("Could not open input file.");

	int size;
	fin >> size;
	if (fin.fail() || fin.bad() || size <= 0)
		throw std::runtime_error("Invalid file format.");

	Matrix<TYPE> A(size), B(size);
	
	fin >> A;
	fin >> B;
	fin.close();

	Matrix<TYPE> C(size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			C.getData()[i][j] = 0;
	Matrix<TYPE> Cp(size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			Cp.getData()[i][j] = 0;

	appData->setMatrixA(A);
	appData->setMatrixB(B);
	appData->setMatrixC(C);
	appData->setMatrixCp(Cp);
}