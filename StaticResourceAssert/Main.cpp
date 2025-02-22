

#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

////////////////////////

#include "include/Common.h"
#include "include/ApiGenerator.h"

constexpr const char* helpFile =
"-d : Required. The directory to recursively search through and catalog entries\n"
"-o : Name of the output api header. Always include the .h extention. Default is: static_resource_assert_api.h\n"
"-h : Print version and command line options\n"
;

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
	replace_all( directoryStr, "\\", "/" );
	replace_all( directoryStr, "\\", "" );
	replace_all( directoryStr, "<", "" );
	replace_all( directoryStr, ">", "" );
	replace_all( directoryStr, "\"", "" );
	replace_all( directoryStr, "|", "" );
	replace_all( directoryStr, "?", "" );
	replace_all( directoryStr, "*", "" );

	TrimStartWhitespace(directoryStr);
	TrimEndWhitespace(directoryStr);

	std::string outputHeaderStr = "static_resource_assert_api.h";
	if( HasArg( "-o", argc, argv ) )
	{
		outputHeaderStr = GetArgData( "-o", argc, argv );
	}

	const fs::path dirPath { directoryStr };
	if( !fs::exists(dirPath) ) {
		PSTREAM_NL( "Directory: " << directoryStr << " doesn't exist" );
		return -2;
	}

	PSTREAM_NL( "Indexing directory: " << directoryStr << " ..." );

	fs::directory_entry resourceDir(dirPath);

	std::error_code ec;
	std::vector<std::string> arrayItems;
	for( const fs::directory_entry& dirEntry : fs::recursive_directory_iterator{resourceDir, fs::directory_options::skip_permission_denied, ec} )
	{
		std::string pathPathStr = dirEntry.path().u8string();

		replace_all( pathPathStr, "\\", "/" );

		replace_all( pathPathStr, resourceDir.path().string() + "/", "" );
		arrayItems.push_back( pathPathStr );
	}

	std::sort(arrayItems.begin(), arrayItems.end());

	PSTREAM_NL( "Directory elements found: " << arrayItems.size() );

	PSTREAM_NL( "Generating " << outputHeaderStr << " ..." );

	const std::string apiHeader = GenerateAPIHeaderString(directoryStr, arrayItems);
	if(apiHeader.empty()) {
		PSTREAM_NL( "Error: Failed to generate API");
		return -3;
	}

	std::fstream file(outputHeaderStr, std::ios::trunc | std::ios::out);
	if( file.is_open() )
	{
		file.write( apiHeader.c_str(), apiHeader.size() );
		file.close();
	}

	PSTREAM_NL( "API Header generated\n" );

	return 0;
}