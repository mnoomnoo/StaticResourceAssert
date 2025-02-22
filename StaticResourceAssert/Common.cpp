
#include "include/Common.h"

#include <cctype>

#ifdef _WIN32
#include "Windows.h"
#endif

/* "A.B.YYYYMMDD"
* A = Major number. Ex: 8, 9, 10, 11. 0 == main branch
* B = Minor number. 0 == main branch
* YYYYMMDD = Year (ex: 2024), month (04), day (02)
*
* EXAMPLES:
* "1.6.20240109" = Major version 1, Minor version 6, 2024, January, 9th
* "2.3.20240814" = Major version 2, Minor version 3, 2024, August, 14th
* "2.4.20240814" = Major version 2, Minor version 4, 2024, August, 14th
* "2.5.20240822" = Major version 2, Minor version 5, 2024, August, 22nd
* "8.12.20241008" = Major version 8, Minor version 12, 2024, October, 8th
* "0.0.20241001" = Main branch version number, 2024, October, 1st

NOTE: 0.0.YYYYMMDD should only be in the main branch. Its a non-release version number
*/
const char* SRA_PROGRAM_VERSION = "1.1.20250221";

////////////////////////////////////////////////////////////////////////////////////////////

void P( const char* str, bool bNewline)
{
	std::cout << str;
#ifdef _WIN32
	OutputDebugStringA( str );
#endif

	if( bNewline )
	{
		std::cout << "\n";
#ifdef _WIN32
		OutputDebugStringA( "\n" );
#endif
	}
}

void __pstream(const std::ostream &t, bool bNewline)
{
	std::stringstream ss;
	ss << t.rdbuf();

	P( ss.str().c_str(), bNewline );
}

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
	std::size_t count{};
	for (std::string::size_type pos{};
		inout.npos != (pos = inout.find(what.data(), pos, what.length()));
		pos += with.length(), ++count) {
		inout.replace(pos, what.length(), with.data(), with.length());
	}
	return count;
}

void TrimStartWhitespace(std::string& inout) {
	if(inout.empty()) {
		return;
	}
	auto beginIter = inout.begin();
	while(inout.end() != beginIter && std::isspace(*beginIter)) {
		beginIter = inout.erase(beginIter);
	}
}

void TrimEndWhitespace(std::string& inout) {
	if(inout.empty()) {
		return;
	}
	auto endIter = inout.end()-1;
	while(inout.end() != endIter && std::isspace(*endIter)) {
		inout.pop_back();
		endIter--;
	}
}

bool HasArg( const char* argStr, int argc, char** argv )
{
	for( int c = 0; c < argc; c++ )
	{
		std::string arg( argv[c] );
		if( argStr == arg )
			return true;
	}
	return false;
}

std::string GetArgData( const char* argStr, int argc, char** argv )
{
	std::string argData;
	for( int c = 0; c < argc; c++ )
	{
		std::string arg( argv[c] );
		if( argStr == arg )
		{
			if( c + 1 < argc )
			{
				argData = argv[c+1];
			}
		}
	}
	return argData;
}
