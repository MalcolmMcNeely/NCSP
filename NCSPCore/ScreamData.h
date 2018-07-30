//
// Extracts shot data from an XML file and returns the information
// in a container
//
#pragma once

#include "common.h"

class Shot;

namespace ScreamData
{
   int GetData(std::vector<Shot>& data);
}