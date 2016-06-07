#include "ArgumentHandler.h"


void printUsage()
{
	std::cout << "Usage:\n"
		<< "  This program multiply two matrices. You need to specifie "
		<< "command line arguments in order for program to work. "
		<< "Through them you should specifie input type (file or random), "
		<< "output file (path to file), other options are optional. "
		<< "If you want to specifie different paths for serial and parallel "
		<< "multiplications, you don't set \"-o\" option, but \"-os\" and \"-op\".\n\n"
		<< "  If input file is specified, it must contain two matrices "
		<< "following rules that both matrices must be same size, and that size "
		<< "must be 2 ^ k (where k >= 0). How do you format file doesn't matter "
		<< "all that matter is that there is no other characters, or signs, which "
		<< "are not numbers or spaces (tabs, newlines). You don't need to separate "
		<< "rows or two matrice, you can add all numbers in same line, all that "
		<< "matters is that each number is separated from its next only by white-line "
		<< "characters ('\\s', '\\t', '\\n'), and nothing else. First number inside "
		<< "file is treated as dimension of both matrices. File will be considered "
		<< "invalid if there is not enough numbers to fill both matrices, or if there is "
		<< "any character beside digits and whitespace characters.\n"
		<< "  If verbose is set in output file will be written result matrix, "
		<< "both of starting matrices and time spent to calculate."
		<< "Example: \n"
		<< "  > path_to_exe.exe -i \"path_to_input_file.txt\" -o \"path_to_output_file.txt\" -v\n\n"
		<< "Options:\n"
		<< "  -r NUM  - Generate random input matrices (if -r is specified -v is assumed)\n"
		<< "  -i PATH - Set path to input file\n"
		<< "  -o PATH - Set path to output file (sets same file for serial and parallel).\n"
		<< "  -os PATH - Set path to output file (for serial multiplication)\n"
		<< "  -op PATH - Set path to output file (for parallel multiplication)\n"
		<< "  -v      - Set verbose\n"
		<< "  -so	  - Serial algorithm only (don't do parallel version)\n"
		<< "  -po     - Parallel algorithm only (don't do serial version)\n"
		<< std::endl;
}

void parseArguments(int argc, char** argv)
{
	ApplicationData* appData = ApplicationData::getInstance();

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-i"))
		{
			if (i == argc - 1)
				throw std::runtime_error("Invalid pattern -i PATH must follow");
			appData->setInputPath(argv[++i]);
		}
		else if (!strcmp(argv[i], "-o"))
		{
			if (i == argc - 1)
				throw std::runtime_error("Invalid pattern -o PATH must follow");
			appData->setSerialOutputPath(argv[++i]);
			appData->setParallelOutputPath(argv[i]);
		}
		else if (!strcmp(argv[i], "-os"))
		{
			if (i == argc - 1)
				throw std::runtime_error("Invalid pattern -os PATH must follow");
			appData->setSerialOutputPath(argv[++i]);
		}
		else if (!strcmp(argv[i], "-op"))
		{
			if (i == argc - 1)
				throw std::runtime_error("Invalid pattern -op PATH must follow");
			appData->setParallelOutputPath(argv[++i]);
		}
		else if (!strcmp(argv[i], "-r"))
		{
			if (i == argc - 1)
				throw std::runtime_error("Invalid pattern -r SIZE must follow");

			if (atoi(argv[++i]) <= 0)
				throw std::runtime_error("Invalid arguments SIZE must be number");
			generateMatrices(atoi(argv[i]));
			appData->setVerbose(true);
		}
		else if (!strcmp(argv[i], "-so"))
		{
			appData->setSerial(true);
			appData->setParallel(false);
		}
		else if (!strcmp(argv[i], "-po"))
		{
			appData->setParallel(true);
			appData->setSerial(false);
		}
		else if (!strcmp(argv[i], "-v"))
			appData->setVerbose(true);
		else
			throw std::runtime_error("Invalid argument!");
	}

	validateArguments();
}

void validateArguments(bool testing)
{
	ApplicationData* appData = ApplicationData::getInstance();
	if ((appData->shouldDoSerial() ? !strcmp(appData->getSerialOutputPath(), "") : false)
		|| (appData->shouldDoParallel() ? !strcmp(appData->getParallelOutputPath(), "") : false)
		|| (!strcmp(appData->getInputPath(), "")
			&& (!appData->getA() || !appData->getB()))
		|| (!appData->shouldDoSerial() && !appData->shouldDoParallel()))
	{
		if (!testing) printUsage();
		throw std::runtime_error("Invalid arguments");
	}
}

void generateMatrices(int size)
{
	Matrix<TYPE> m(size), m2(size), m3(size), m4(size);

	srand((unsigned)time(NULL));

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m.getData()[i][j] = (TYPE)rand() % Matrix<TYPE>::RANDOM_MAX *
			(rand() % 10 + 1 > 5 ? -1 : 1); // add negative
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m2.getData()[i][j] = (TYPE)rand() % Matrix<TYPE>::RANDOM_MAX *
			(rand() % 10 + 1 > 5 ? -1 : 1); // add negative

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m3.getData()[i][j] = 0;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m4.getData()[i][j] = 0;

	ApplicationData* appData = ApplicationData::getInstance();

	appData->setMatrixA(m);
	appData->setMatrixB(m2);
	appData->setMatrixC(m3);
	appData->setMatrixCp(m4);
}