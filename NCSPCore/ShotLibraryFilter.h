//
// Class for returning a library filter by various characteristics
//
#pragma once

#include "NarrativeEnums.h"

class ShotLibrary;

class ShotLibraryFilter
{
public:
   ShotLibraryFilter();
   ~ShotLibraryFilter();

   void FilterByLocation(ShotLibrary* in, ShotLibrary* out, eLocation loc, bool includeNoShot = true);
   void FilterByCharacter(ShotLibrary* in, ShotLibrary* out, eChar ch, bool includeNoShot = true);
};

