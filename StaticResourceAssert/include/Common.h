#ifndef __COMMON_H
#define __COMMON_H


#include <iostream>
#include <sstream>

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
extern const char* _PROGRAM_VERSION;

////////////////////////////////////////////////////////////////////////////////////////////



void __pstream(const std::ostream &t, bool bNewline = true );

#define PSTREAM( s ) __pstream( std::stringstream()<< s, false )

#define PSTREAM_NL( s ) __pstream( std::stringstream()<< s, true )

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with);


// Program arguments

bool HasArg( const char* argStr, int argc, char** argv );

std::string GetArgData( const char* argStr, int argc, char** argv );

#endif