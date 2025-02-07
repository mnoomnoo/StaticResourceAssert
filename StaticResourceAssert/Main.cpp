

#include <filesystem>
#include <fstream>



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
	versionStr += _PROGRAM_VERSION;
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

	std::string outputHeaderStr = "static_resource_assert_api.h";
	if( HasArg( "-o", argc, argv ) )
	{
		outputHeaderStr = GetArgData( "-o", argc, argv );
	}

	const std::filesystem::path dirPath { directoryStr };
	if( !std::filesystem::exists(dirPath) ) {
		PSTREAM_NL( "Directory: " << directoryStr << " doesn't exist" );
		return -2;
	}

	PSTREAM_NL( "Indexing directory: " << directoryStr << " ..." );

	std::filesystem::directory_entry resourceDir(dirPath);

	std::vector<std::string> arrayItems;
	for( const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator{resourceDir, std::filesystem::directory_options::skip_permission_denied} )
	{
		std::filesystem::path dirPath = dirEntry.path();

		std::string pathPathStr = dirPath.u8string();

		replace_all( pathPathStr, "\\", "/" );

		replace_all( pathPathStr, resourceDir.path().string() + "/", "" );
		arrayItems.push_back( pathPathStr );
	}

	PSTREAM_NL( "Directory elements found: " << arrayItems.size() );

	PSTREAM_NL( "Generating " << outputHeaderStr << " ..." );

	const std::string apiHeader = GenerateAPIHeaderString(arrayItems);
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