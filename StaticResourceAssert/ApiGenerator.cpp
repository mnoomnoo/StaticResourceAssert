
#include "include/Common.h"
#include "include/ApiGenerator.h"

#include <chrono>
#include <fstream>
#include <iomanip>

std::string GenerateAPI_HeaderString( const std::string_view& directoryCataloged, const std::vector<DirEntryInfo>& arrayItems )
{
	if (arrayItems.empty())
		return std::string();

	std::ifstream inputFile("StaticResourceAssert/templates/static_resource_assert_api.h.template");
	if(!inputFile) {
		return std::string();
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string headerString = buffer.str();
	inputFile.close();

	std::string arrayElements;

	const size_t numOfItems = arrayItems.size();
	uint64_t counter = 0;

	for( size_t c = 0; c < numOfItems; c++ )
	{
		const std::string str = arrayItems[c].path;

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
	// generate information about the generation
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    const std::tm local_tm = *std::localtime(&now_time);

	std::ostringstream oss;
	oss << "SRA version: " << SRA_PROGRAM_VERSION << std::endl;
	oss << "Generation date: " << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
	oss << "Directory cataloged: " << directoryCataloged << std::endl;
	oss << "Number of items found: " << numOfItems << std::endl;

	ReplaceAll(headerString, "${ResourceFiles}", arrayElements);
	ReplaceAll(headerString, "${ArraySize}", std::to_string(numOfItems));
	ReplaceAll(headerString, "${GenerationComment}", oss.str());

    // Get the duration since the Unix epoch in milliseconds
    const auto duration = now.time_since_epoch();

    // Convert duration to milliseconds
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	ReplaceAll(headerString, "${ArrayID}", std::to_string(milliseconds));

	return headerString;
}

std::string GenerateAPI_CppString(const std::string_view& outputHeaderName, const std::vector<DirEntryInfo>& arrayItems) {
	if (outputHeaderName.empty())
		return std::string();

	std::ifstream inputFile("StaticResourceAssert/templates/static_resource_assert_api.cpp.template");
	if(!inputFile) {
		return std::string();
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string cppString = buffer.str();
	inputFile.close();

	const size_t numOfItems = arrayItems.size();

	ReplaceAll(cppString, "${IncludePath}", outputHeaderName);
	ReplaceAll(cppString, "${ArraySize}", std::to_string(numOfItems));

	std::string fileContent;
	uint64_t counter = 0;
	for( size_t c = 0; c < numOfItems; c++ )
	{
		const std::string path = arrayItems[c].path;
		const std::string contents = arrayItems[c].fileContents;

		// build the element
		fileContent += "std::pair(\"";
		fileContent += path + "\",";
		fileContent += "std::string_view( R\"###(";
		fileContent += contents;
		fileContent += ")###\", " + std::to_string(contents.size()) + "))";

		// add comma iff we're not at the end of the array
		if( c < numOfItems-1 )
			fileContent += ",";

		counter++;
		if(0 == (counter % 5)) {
			fileContent += "\n";
		}
	}

	ReplaceAll(cppString, "${FileContent}", fileContent);

	return cppString;
}

std::string GenerateAPI_CMakeListsString( const std::string_view& projectName, const std::string_view& libraryName, const std::string_view& outputHeaderName)
{
	if (projectName.empty() || libraryName.empty() || outputHeaderName.empty())
		return std::string();

	std::ifstream inputFile("StaticResourceAssert/templates/CMakeLists.txt.template");
	if(!inputFile) {
		return std::string();
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string cmakeListsString = buffer.str();
	inputFile.close();

	ReplaceAll(cmakeListsString, "${CMakeProjectName}", projectName);
	ReplaceAll(cmakeListsString, "${CMakeLibraryName}", libraryName);

	std::string outputHeaderNameStr = outputHeaderName.data();
	const std::string_view filename = RemoveExt(outputHeaderName.data(), ".h");
	if (!filename.empty()) {
		outputHeaderNameStr = filename;
	}
	outputHeaderNameStr += ".cpp";
	ReplaceAll(cmakeListsString, "${CMakeLibrarySources}", outputHeaderNameStr);

	return cmakeListsString;
}


