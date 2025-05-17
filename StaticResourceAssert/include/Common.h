#ifndef __COMMON_H
#define __COMMON_H


#include <iostream>
#include <sstream>
#include <string_view>

extern const char* SRA_PROGRAM_VERSION;

////////////////////////////////////////////////////////////////////////////////////////////

struct DirEntryInfo {
	std::string path;
	std::string fileContents;
};

////////////////////////////////////////////////////////////////////////

void __pstream(const std::ostream &t, bool bNewline = true );

#define PSTREAM( s ) __pstream( std::stringstream()<< s, false )

#define PSTREAM_NL( s ) __pstream( std::stringstream()<< s, true )

std::size_t ReplaceAll(std::string& inout, std::string_view what, std::string_view with);

std::string_view RemoveExt(const std::string_view& in, const std::string_view& ext);

void TrimStartWhitespace(std::string& inout);

void TrimEndWhitespace(std::string& inout);

// Program arguments

bool HasArg( const char* argStr, int argc, char** argv );

std::string GetArgData( const char* argStr, int argc, char** argv );

#endif