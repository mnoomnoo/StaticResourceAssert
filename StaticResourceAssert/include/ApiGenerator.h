#ifndef __APIGENERATOR_H
#define __APIGENERATOR_H

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////

std::string GenerateAPIHeaderString( const std::string_view& directoryCataloged, const std::vector<std::string>& arrayItems );

#endif