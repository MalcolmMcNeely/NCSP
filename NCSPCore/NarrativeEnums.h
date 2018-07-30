//
//	Enumeration values for shot properties and functions
// for friendly names (some of these may be depreciated)
//
#pragma once
#include <string>

enum eViewerType
{
   eV_THRILL                     = 0,
   eV_GORE                       = 1
};

enum eRowProperties
{
   eP_SHOTID                     = 0,
   eP_STRUCTURE                  = 1,
   eP_GORE                       = 2,
   eP_THRILL                     = 3,
   eP_EMPATHY                    = 4,
   eP_THREATEN                   = 5,
   eP_CHASE                      = 6,
   eP_HIDE                       = 7,
   eP_KILL                       = 8,
   eP_ATTACK                     = 9,
   eP_ESCAPE                     = 10,
   eP_LOCATION                   = 11,
   eP_TIMEDURATION               = 12,
   eP_CHARACTERSTARTROW          = 13,
   eP_SIZE                       = 14
};

enum eCharacterProperties
{
   eCP_NAME                      = 0,
   eCP_ALIVE                     = 1,
   eCP_SIZE                      = 2
};

enum eLocation
{
   eL_NONE_DUMMY                 = 0,
   eL_SCHOOL                     = 1,
   eL_OUT_SCHOOL                 = 2,
   eL_IN_SCHOOL                  = 3,
   eL_IN_HOUSE1                  = 4,
   eL_OUT_HOUSE1                 = 5,
   eL_HOUSE2                     = 6,
   eL_IN_HOUSE2                  = 7,
   eL_OUT_HOUSE2                 = 8,
   eL_GROUND_HOUSE2              = 9,
   eL_UP_HOUSE2                  = 10,
   eL_OUT_HOUSE3                 = 11,
   eL_UP_HOUSE3                  = 12,
   eL_IN_HOUSE3                  = 13,
   eL_ROAD_HOUSE3                = 14,
   eL_GARAGE_HOUSE3              = 15,
   eL_HOUSE4                     = 16,
   eL_PARK                       = 17,
   eL_POLICE_STATION             = 18,
   eL_OUTSIDE                    = 19,
   eL_SIZE                       = 20
};

enum eChar
{
   eC_CASEY                      = 0,
   eC_SIDNEY                     = 1,
   eC_BILLY                      = 2,
   eC_DEWEY                      = 3,
   eC_TATUM                      = 4,
   eC_GALE                       = 5,
   eC_SIZE                       = 6,
   eC_NONE                       = 7
};

namespace NarrativeEnums 
{
   // Enum to String
   std::string GetRowPropertyName(eRowProperties e);
   std::string GetCharName(eChar e);
   std::string GetLocationName(eLocation e);

   // String to Enum
   eLocation GetLocationFromString(std::string s);
   eChar GetCharFromString(std::string s);
}