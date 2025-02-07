
#include "include/Common.h"

#ifdef _WIN32
#include "Windows.h"
#endif

const char* _PROGRAM_VERSION = "0.0.20250206";

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
