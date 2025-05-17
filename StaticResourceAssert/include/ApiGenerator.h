#ifndef __APIGENERATOR_H
#define __APIGENERATOR_H

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////

std::string GenerateAPI_HeaderString( const std::string_view& directoryCataloged, const std::vector<DirEntryInfo>& arrayItems );

std::string GenerateAPI_HeaderString(const std::string_view& outputHeaderName);

std::string GenerateAPI_CMakeListsString( const std::string_view& projectName, const std::string_view& libraryName, const std::string_view& outputHeaderName);

#endif