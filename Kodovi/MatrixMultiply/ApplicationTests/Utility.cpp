#include "Utility.h"
#include <fstream>

/// helper function to test writing into output file
bool isFilesEqual(const char* lFilePath, const char* rFilePath)
{
	std::ifstream lFile(lFilePath, std::ios::in | std::ios::binary);
	std::ifstream rFile(rFilePath, std::ios::in | std::ios::binary);

	if (!lFile.good() || !rFile.good())
		return false;

	std::streamsize lReadBytesCount = 0;
	std::streamsize rReadBytesCount = 0;

	int bufferSize = 8192;
	char* p_lBuffer = new char[bufferSize];
	char* p_rBuffer = new char[bufferSize];

	do {
		lFile.read(p_lBuffer, bufferSize);
		rFile.read(p_rBuffer, bufferSize);
		lReadBytesCount = lFile.gcount();
		rReadBytesCount = rFile.gcount();

		if (lReadBytesCount != rReadBytesCount || std::memcmp(p_lBuffer, p_rBuffer, (size_t)lReadBytesCount) != 0)
		{
			delete[] p_lBuffer;
			delete[] p_rBuffer;

			return false;
		}
	} while (lFile.good() || rFile.good());

	delete[] p_lBuffer;
	delete[] p_rBuffer;

	return true;
}