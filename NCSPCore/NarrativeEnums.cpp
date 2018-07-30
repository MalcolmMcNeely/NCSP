//
// Implementation of NarrativeEnums.h
//
#include "NarrativeEnums.h"
#include <assert.h>

#pragma region Lookup Tables

const char* eRowPropertiesNames[] =
{
   "ShotID",
   "Structure"
   "Gore",
   "Thrill",
   "Empathy",
   "Threaten",
   "Chase",
   "Hide",
   "Kill",
   "Attack",
   "Escape",
   "Location",
   "TimeDuration",
   "CharacterName"
};
const char* eCharNames[] =
{
   "Casey",
   "Sidney",
   "Billy",
   "Dewey",
   "Tatum",
   "Gale"
};
const char* eLocationNames[] =
{
   "NONE",
   "SCHOOL",
   "OUT-SCHOOL",
   "IN-SCHOOL",
   "IN-HOUSE1",
   "OUT-HOUSE1",
   "HOUSE2",
   "IN-HOUSE2",
   "OUT-HOUSE2",
   "GROUND-HOUSE2",
   "UP-HOUSE2",
   "OUT-HOUSE3",
   "UP-HOUSE3",
   "IN-HOUSE3",
   "ROAD-HOUSE3",
   "GARAGE-HOUSE3",
   "HOUSE4",
   "PARK",
   "POLICE-STATION",
   "OUTSIDE"
};

#pragma endregion

namespace NarrativeEnums
{
   std::string GetRowPropertyName(eRowProperties e)
   {
      assert((int)e >= 0);
      assert((int)e < (int)eP_SIZE);
      return eRowPropertiesNames[(int)e];
   }

   std::string GetCharName(eChar e)
   {
      assert((int)e >= 0);
      assert((int)e < (int)eC_SIZE);
      return eCharNames[(int)e];
   }

   std::string GetLocationName(eLocation e)
   {
      assert((int)e >= 0);
      assert((int)e < (int)eL_SIZE);
      return eLocationNames[(int)e];
   }

   eLocation GetLocationFromString(std::string s)
   {
      eLocation out = eL_NONE_DUMMY;

      for (int i = 0; i < (int)eL_SIZE; i++)
      {
         if (std::strcmp(eLocationNames[i], s.c_str()) == 0)
         {
            out = (eLocation)i;
            break;
         }
      }

      return out;
   }

   eChar GetCharFromString(std::string s)
   {
      eChar out = eC_SIZE;

      for (int i = 0; i < (int)eC_SIZE; i++)
      {
         if (std::strcmp(eCharNames[i], s.c_str()) == 0)
         {
            out = (eChar)i;
            break;
         }
      }

      return out;
   }
}