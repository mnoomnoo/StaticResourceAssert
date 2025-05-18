

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cstdint>

namespace fs = std::filesystem;

////////////////////////

#include "include/Common.h"
#include "include/ApiGenerator.h"

constexpr const char* helpFile =
"-d : Required. The directory to recursively search through and catalog entries\n"
"-o : Name of the output api header. Always include the .h extention. Default is: static_resource_assert_api.h\n"
"-odir : Output directory. Always add a '/' to the end of the directory path. Default is: sra_output/"
"--c : Read file contents\n"
"-h : Print version and command line options\n"
;

////////////////////////////////////////////////////////////////////////

bool OutputAPI_HeaderString(const std::string& outputDir, const std::string& outputHeaderName, const std::string& directoryStr, const std::vector<DirEntryInfo>& arrayItems) {
	PSTREAM_NL( "Generating " << outputHeaderName << " ..." );

	const std::string apiHeader = GenerateAPI_HeaderString(directoryStr, arrayItems);
	if(apiHeader.empty()) {
		PSTREAM_NL( "\tError: Failed to generate API header");
		return false;
	}

	std::fstream file(outputDir + outputHeaderName, std::ios::trunc | std::ios::out);
	if( file.is_open() )
	{
		file.write( apiHeader.c_str(), apiHeader.size() );
		file.close();
	}

	PSTREAM_NL( "\tAPI header generated\n" );
	return true;
}

bool OutputAPI_CppString(const std::string& outputDir, const std::string& outputHeaderName, const std::vector<DirEntryInfo>& arrayItems) {
	const std::string_view filenameStrView = RemoveExt(outputHeaderName.data(), ".h");
	std::string filename(filenameStrView);
	filename += ".cpp";
	PSTREAM_NL( "Generating "  << filename << " ..." );

	const std::string apiCpp = GenerateAPI_CppString(outputHeaderName, arrayItems);
	if(apiCpp.empty()) {
		PSTREAM_NL( "\tError: Failed to generate API cpp");
		return false;
	}

	std::fstream file(outputDir + filename, std::ios::trunc | std::ios::out);
	if( file.is_open() )
	{
		file.write( apiCpp.c_str(), apiCpp.size() );
		file.close();
	}

	PSTREAM_NL( "\tAPI cpp generated\n" );
	return true;
}

bool OutputAPI_CMakeListsString(const std::string& outputDir, const std::string& outputHeaderName, const std::string& projectName, const std::string& libraryName) {
	PSTREAM_NL( "Generating CMakeLists.txt ..." );

	const std::string apiCMakeLists = GenerateAPI_CMakeListsString(projectName, libraryName, outputHeaderName);
	if(apiCMakeLists.empty()) {
		PSTREAM_NL( "\tError: Failed to generate API CMakeLists");
		return false;
	}

	std::fstream file(outputDir + "CMakeLists.txt", std::ios::trunc | std::ios::out);
	if( file.is_open() )
	{
		file.write( apiCMakeLists.c_str(), apiCMakeLists.size() );
		file.close();
	}

	PSTREAM_NL( "\tAPI CMakeLists generated\n" );
	return true;
}

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	std::string versionStr = "\nStatic Resource Assert V: ";
	versionStr += SRA_PROGRAM_VERSION;
	versionStr += "\n\n";

	PSTREAM( versionStr.c_str() );

	if( 2 > argc || HasArg( "-h", argc, argv ) )
	{
		PSTREAM_NL( helpFile );
		return 0;
	}

	if( !HasArg( "-d", argc, argv ) ) {
		PSTREAM_NL( "Directory: (-d) argument was not found\n" );
		PSTREAM_NL( helpFile );
		return -1;
	}

	std::string directoryStr = GetArgData( "-d", argc, argv );
	ReplaceAll( directoryStr, "\\", "/" );
	ReplaceAll( directoryStr, "\\", "" );
	ReplaceAll( directoryStr, "<", "" );
	ReplaceAll( directoryStr, ">", "" );
	ReplaceAll( directoryStr, "\"", "" );
	ReplaceAll( directoryStr, "|", "" );
	ReplaceAll( directoryStr, "?", "" );
	ReplaceAll( directoryStr, "*", "" );

	TrimStartWhitespace(directoryStr);
	TrimEndWhitespace(directoryStr);

	std::string outputHeaderName = "static_resource_assert_api.h";
	if( HasArg( "-o", argc, argv ) ) {
		outputHeaderName = GetArgData( "-o", argc, argv );
	}

	std::string outputDir = "sra_output/";
	if( HasArg( "-odir", argc, argv ) ) {
		outputDir = GetArgData( "-odir", argc, argv );
	}
	fs::create_directories(outputDir);

	bool readContents = false;
	if( HasArg( "--c", argc, argv ) ) {
		readContents = true;
	}

	const fs::path dirPath { directoryStr };
	if( !fs::exists(dirPath) ) {
		PSTREAM_NL( "Directory: " << directoryStr << " doesn't exist" );
		return -2;
	}

	PSTREAM_NL( "Indexing directory: " << directoryStr << " ..." );

	fs::directory_entry resourceDir(dirPath);

	std::error_code ec;
	std::vector<DirEntryInfo> arrayItems;
	for( const fs::directory_entry& dirEntry : fs::recursive_directory_iterator{resourceDir, fs::directory_options::skip_permission_denied, ec} )
	{
		std::string pathPathStr = dirEntry.path().u8string();

		ReplaceAll( pathPathStr, "\\", "/" );

		const std::string resourceDirStr = resourceDir.path().string();
		ReplaceAll( pathPathStr, resourceDirStr + "/", "" );

		DirEntryInfo dirEntryInfo;
		dirEntryInfo.path = pathPathStr;

		if(readContents) {
			std::ifstream istrm(resourceDirStr + "/" + dirEntryInfo.path, std::ios::binary);
			if( istrm.is_open() )
			{
				// get its size:
				istrm.seekg(0, std::ios::end);
				const size_t fileSize = istrm.tellg();
				istrm.seekg(0, std::ios::beg);

				if(0 != fileSize) {

					dirEntryInfo.fileContents.resize(fileSize);
					istrm.read(dirEntryInfo.fileContents.data(), fileSize);
				}

				istrm.close();
			}
		}

		arrayItems.push_back( dirEntryInfo );
	}

	// sort all the items found
	std::sort(arrayItems.begin(), arrayItems.end()
	, [](const DirEntryInfo& a, const DirEntryInfo& b) -> bool {
		return a.path < b.path;
	});

	PSTREAM_NL( "Directory elements found: " << arrayItems.size() );

	if(!OutputAPI_HeaderString(outputDir, outputHeaderName, directoryStr, arrayItems)) {
		PSTREAM_NL( "API header failed to generate." );
		return -3;
	}

	if(!OutputAPI_CppString(outputDir, outputHeaderName, arrayItems)) {
		PSTREAM_NL( "API cpp failed to generate." );
		return -4;
	}

	if(!OutputAPI_CMakeListsString(outputDir, outputHeaderName, "TestProjectName", "TestLibraryName")) {
		PSTREAM_NL( "API CMakeLists.txt failed to generate." );
		return -5;
	}

	return 0;
}