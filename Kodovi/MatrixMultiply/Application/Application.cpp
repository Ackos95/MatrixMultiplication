#include <ArgumentHandler.h>
#include <InputHandler.h>
#include <SerialAlgorithm.h>
#include <ParallelAlgorithm.h>
#include <OutputHandler.h>
#include <tbb\tbb.h>
#include <tbb\tbb.h>


int main(int argc, char** argv)
{
	try
	{
		std::cout << "Reading data and loading (generating) matrixes..." << std::endl;
		parseArguments(argc, argv);
		parseInputFile();
		std::cout << "Matrixes loaded." << std::endl << std::endl;
		tbb::tick_count before, after;

		/*
		std::cout << "Serial Multiplication Simple started\nThis may take a minute...\n\n";
		before = tbb::tick_count::now();
		serialMultiplySimple();
		after = tbb::tick_count::now();
		std::cout << "Serial multiplication Simple finished!" << std::endl
			<< "Calculating took " << (after - before).seconds() << " seconds.\n\n";
		*/

		if (ApplicationData::getInstance()->shouldDoSerial())
		{
			std::cout << "Serial Multiplication started\nThis may take a minute...\n\n";
			before = tbb::tick_count::now();
			serialMultiply();
			after = tbb::tick_count::now();
			std::cout << "Serial multiplication finished!" << std::endl
				<< "Calculating took " << (after - before).seconds() << " seconds.\n\n";
		}

		if (ApplicationData::getInstance()->shouldDoParallel())
		{
			std::cout << "Parallel Multiplication started \nThis may take a minute...\n\n";
			before = tbb::tick_count::now();
			parallelMultiply();
			after = tbb::tick_count::now();
			std::cout << "Parallel multiplication finished!" << std::endl
				<< "Calculating took " << (after - before).seconds() << " seconds.\n\n";
		}

		/*
		std::cout << "Writting data to output file." << std::endl;
		writeToFile();
		std::cout << "Data writting finished\n. Check results in \""
			<< ApplicationData::getInstance()->getOutputPath() << "\" file.\n\n";
		*/
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}