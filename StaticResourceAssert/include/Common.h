#ifndef __COMMON_H
#define __COMMON_H


#include <iostream>
#include <sstream>


extern const char* SRA_PROGRAM_VERSION;

////////////////////////////////////////////////////////////////////////////////////////////



void __pstream(const std::ostream &t, bool bNewline = true );

#define PSTREAM( s ) __pstream( std::stringstream()<< s, false )

#define PSTREAM_NL( s ) __pstream( std::stringstream()<< s, true )

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with);

void TrimStartWhitespace(std::string& inout);

void TrimEndWhitespace(std::string& inout);

// Program arguments

bool HasArg( const char* argStr, int argc, char** argv );

std::string GetArgData( const char* argStr, int argc, char** argv );

#endif