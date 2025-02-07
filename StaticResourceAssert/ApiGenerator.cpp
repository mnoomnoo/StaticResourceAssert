
#include "include/Common.h"
#include "include/ApiGenerator.h"

#include <chrono>
#include <fstream>
#include <cmath>

std::string GenerateAPIHeaderString( const std::vector<std::string>& arrayItems )
{
	if (arrayItems.empty())
		return std::string();

	std::ifstream inputFile("StaticResourceAssert/templates/static_resource_assert_api.ht");
	if(!inputFile) {
		return "";
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string headerString = buffer.str();
	inputFile.close();

	std::string arrayElements;

	const uint64_t numOfItems = arrayItems.size();
	uint64_t itemCounter = 0;

	const uint64_t rowSize = static_cast<uint64_t>(std::ceil(std::sqrt(static_cast<double>(numOfItems))));
	const uint64_t colSize = rowSize;

	const uint64_t matrixSize = rowSize * colSize;

	uint64_t counter = 0;

	for( size_t c = 0; c < numOfItems; c++ )
	{
		const std::string str = arrayItems[c];

		if(0 == counter) {
			arrayElements += "{";
		}
		counter++;

		// build the element
		arrayElements += "u8\"";
		arrayElements += str;
		arrayElements += "\"";

		if(colSize == counter) {
			arrayElements += "}\n";
			counter = 0;
		}

		// add comma iff we're not at the end of the array
		if( c < numOfItems-1 )
			arrayElements += ",";

		itemCounter++;
	}

	if(itemCounter < matrixSize && *arrayElements.cend() != ',') {
		arrayElements += ",";
	}

	while(itemCounter < matrixSize) {
		if(0 == counter) {
			arrayElements += "{";
		}
		counter++;

		// build the element
		arrayElements += "\"nullptr";
		arrayElements += "\"";

		if(colSize == counter) {
			arrayElements += "}\n";
			counter = 0;
		}

		// add comma iff we're not at the end of the array
		if( itemCounter < matrixSize-1 )
			arrayElements += ",";

		itemCounter++;
	}

	replace_all(headerString, "${ResourceFiles}", arrayElements);
	replace_all(headerString, "${ArraySize}", std::to_string(numOfItems));
	replace_all(headerString, "${ArrayRowSize}", std::to_string(rowSize));
	replace_all(headerString, "${ArrayColSize}", std::to_string(colSize));

    // Get current time from system clock
    auto now = std::chrono::system_clock::now();

    // Get the duration since the Unix epoch in milliseconds
    auto duration = now.time_since_epoch();

    // Convert duration to milliseconds
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	replace_all(headerString, "${ArrayID}", std::to_string(milliseconds));

	return headerString;
}