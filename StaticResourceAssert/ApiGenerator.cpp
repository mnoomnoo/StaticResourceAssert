
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
	uint64_t counter = 0;

	for( size_t c = 0; c < numOfItems; c++ )
	{
		const std::string str = arrayItems[c];

		// build the element
		arrayElements += "u8\"";
		arrayElements += str;
		arrayElements += "\"";

		// add comma iff we're not at the end of the array
		if( c < numOfItems-1 )
			arrayElements += ",";

		counter++;
		if(0 == (counter % 5)) {
			arrayElements += "\n";
		}
	}

	replace_all(headerString, "${ResourceFiles}", arrayElements);
	replace_all(headerString, "${ArraySize}", std::to_string(numOfItems));

    // Get current time from system clock
    auto now = std::chrono::system_clock::now();

    // Get the duration since the Unix epoch in milliseconds
    auto duration = now.time_since_epoch();

    // Convert duration to milliseconds
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	replace_all(headerString, "${ArrayID}", std::to_string(milliseconds));

	return headerString;
}