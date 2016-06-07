#include "OutputHandler.h"


void writeToFile()
{
	ApplicationData* appData = ApplicationData::getInstance();

	if (!appData->getA() || !appData->getB() || !appData->getC() || !appData->getCp())
		throw std::runtime_error("Matrices not initialized");

	if (appData->shouldDoSerial())
	{
		std::ofstream fout(appData->getSerialOutputPath());
		if (!fout.is_open())
			throw std::runtime_error("Could not open output file.");

		if (appData->isVerbose())
			fout << "A: \n" << *(appData->getA()) << "\n" << "B: \n" << *(appData->getB()) << "\n";
		fout << "C: \n" << *(appData->getC());

		fout.close();
	}

	if (appData->shouldDoParallel())
	{
		std::ofstream fout(appData->getParallelOutputPath());
		if (!fout.is_open())
			throw std::runtime_error("Could not open parallel output file.");

		if (appData->isVerbose())
			fout << "A: \n" << *(appData->getA()) << "\n" << "B: \n" << *(appData->getB()) << "\n";
		fout << "C: \n" << *(appData->getCp());

		fout.close();
	}
}