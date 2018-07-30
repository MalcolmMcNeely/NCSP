//
// Implementation of ShotLibrary.h
//
#include "ShotLibrary.h"
#include "Shot.h"
#include "Character.h"
#include "Utility.h"
#include "Logger.h"

using namespace Utility;
using namespace std;

ShotLibrary::ShotLibrary() : bIsFinalized_(false) { }
ShotLibrary::~ShotLibrary() { }

bool ShotLibrary::AddShot(Shot& shot)
{
   int id = shot.GetId();

	// Shots must have unique Id
   for (auto& s : shots_)
   {
      if (s.GetId() == shot.GetId())
      {
         return false;
      }
   }

   shots_.push_back(shot);

   if (bIsFinalized_)
   {
      CalculateData();
   }

   return true;
}

bool ShotLibrary::RemoveShotById(int id)
{
   if (bIsFinalized_) return false;

   shots_.erase(remove_if(begin(shots_), end(shots_),
      [id](Shot& s){return (s.GetId() == id); }),
      end(shots_));

   return true;
}

void ShotLibrary::Finalize()
{
   bIsFinalized_ = true;
   CalculateData();
}

void ShotLibrary::Clear()
{
	bIsFinalized_ = false;
	shots_.clear();
   ResetData();
}

void ShotLibrary::SetViewerType(int vt)
{
   viewerType_ = vt;
}

std::vector<Shot> ShotLibrary::GetShots() const
{
   return shots_;
}

int ShotLibrary::GetMaxNumShotVariables() const
{
   int out = 0;
   out += Shot::NumBasicMembers();
   out += Character::NumMembers() * MAXCHARACTERS;
   return out;
}

bool ShotLibrary::GetIsFinalized() const
{
   return bIsFinalized_;
}

Shot ShotLibrary::GetShotById(int id) const
{
	for (Shot shot : shots_)
	{
		if (shot.GetId() == id)
		{
			return shot;
		}
	}

	return 0;
}

int ShotLibrary::GetShotLibSize() const
{
   int output = -1;
   if (bIsFinalized_) output = shots_.size();
   return output;
}

int ShotLibrary::GetShotIdByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = shots_[index].GetId();
   return output;
}

int ShotLibrary::GetStructureByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = shots_[index].GetStructure();
   return output;
}

int ShotLibrary::GetGoreByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = (int)shots_[index].GetGore();
   return output;
}

int ShotLibrary::GetEmpathyByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = (int)shots_[index].GetEmpathy();
   return output;
}

int ShotLibrary::GetLocationByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = (int)shots_[index].GetLocation();
   return output;
}

int ShotLibrary::GetTimeDurationByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = shots_[index].GetTimeDuration();
   return output;
}

int ShotLibrary::GetShotNumCharactersByIndex(int index) const
{
   int output = -1;
   if (bIsFinalized_) output = shots_[index].GetCharacterList().size();
   return output;
}

string ShotLibrary::GetCharacterNameInShotByIndex(int shotIndex, int charIndex)
{
   string output;
   if (bIsFinalized_)
   {
      vector<eChar> charList = shots_[shotIndex].GetCharacterList();
      output = NarrativeEnums::GetCharName(charList[charIndex]);
   }
   return output.c_str();
}

vector<int> ShotLibrary::GetRealTimeDurations()
{
   vector<int> out;

   for (auto& shot : shots_)
   {
      out.push_back(shot.GetTimeDuration());
   }

   return out;
}

bool ShotLibrary::GetTotalDurationForLocation(int loc, int* out)
{
   try
   {
      *out = locationDurationValues_.at(loc);
   }
   catch (const std::out_of_range exception)
   {
      *out = 0;
      return false;
   }

   return true;
}

bool ShotLibrary::GetCharacterAppearanceForLocation(int loc, int c, int* out)
{
   try
   {
      auto characters = characterAppearanceRates_.at(loc);

      for (auto& ch : characters)
      {
         if (ch.First == c)
         {
            *out = ch.Second;
            return true;
         }
      }
   }
   catch (const std::out_of_range exception)
   {
      *out = 0;
      return false;
   }

   return true;
}

bool ShotLibrary::GetMaxCostAtLocation(int loc, int* out)
{
   try
   {
      *out = maxCostForLocations_.at(loc);
   }
   catch (const std::out_of_range exception)
   {
      *out = 0;
      return false;
   }

   return true;
}

int ShotLibrary::CalculateMaxCostAtLocation(int loc, vector<Shot>& shots)
{
   int out = 0;

   for (auto& shot : shots)
   {
      if (shot.GetLocation() == (eLocation)loc)
      {
         int cost = CalculateCostForShot(shot);
         if (cost > 0) out += cost;
      }
   }

   return out;
}

int ShotLibrary::CalculateCostForShot(Shot& shot)
{
   int gore = 0;
   int thrill = 0;

   gore += (1 + (shot.GetAttack() + shot.GetKill())) * shot.GetGore();
   thrill += (1 + (shot.GetEmpathy() + shot.GetEscape() + shot.GetHide() +
      shot.GetThreaten() + shot.GetChase())) * shot.GetThrill();

   return viewerType_ == eV_GORE ? gore - thrill : thrill - gore;
}

void ShotLibrary::LogShotsAtLocation(int loc, std::string filename)
{
   Logger log(filename.c_str());

   eLocation eLoc = (eLocation)loc;

   log << "Logging for Location: " << NarrativeEnums::GetLocationName(eLoc) << endl;
   
   int temp = 0;
   for (auto& s : shots_)
   {
      if (s.GetLocation() == eLoc)
      {
         temp++;
      }
   }

   log << "Number of shots: " << temp << endl;
   temp = 0;

   GetTotalDurationForLocation(loc, &temp);
   log << "Total Duration: " << temp << endl;
   temp = 0;

   std::vector<TwoInt> tempV;

   try
   {
      tempV = characterAppearanceRates_.at(loc);
   }
   catch (const std::out_of_range exception) 
   {
      log << "## No Character Appearances ##" << endl;
   }

   log << "Characters and appearance rate: " << endl;   
   for (auto& t : tempV)
   {
      log << NarrativeEnums::GetCharName((eChar)t.First) << "\t" << t.Second << endl;
   }

   log << "Raw Data: " << endl;
   log << "GT, ETCHKAE = Gore, Thrill \t Empathy, Threaten, Chase, Hide, Kill, Attack, Escape" << endl;

   for (auto s : shots_)
   {
      if (s.GetLocation() == eLoc)
      {
         log << "\tID: " << s.GetId() << "\t";
         log << s.GetGore() << s.GetThrill() << "\t";
         log << s.GetEmpathy() << s.GetThreaten() << s.GetChase() << s.GetHide();
         log << s.GetKill() << s.GetAttack();
         log << "\tCost(" << CalculateCostForShot(s) << ")";
         log << "\tDuration(" << s.GetTimeDuration() << ")";
         log << "\t";

         for (auto& c : s.GetCharacterList())
         {
            log << NarrativeEnums::GetCharName(c) << "\t";
         }

         log << endl;
      }
   }
}

void ShotLibrary::ResetData()
{
   shotIds_.clear();
   uniqueStructureValues_.clear();
   uniqueGoreValues_.clear();
   uniqueThrillValues_.clear();
   uniqueEmpathyValues_.clear();
   uniqueChaseValues_.clear();
   uniqueHideValues_.clear();
   uniqueKillValues_.clear();
   uniqueAttackValues_.clear();
   uniqueEscapeValues_.clear();
   uniqueLocationValues_.clear();
   uniqueTimeDurations_.clear();
   uniqueCharacters_.clear();
}

void ShotLibrary::CalculateData()
{
   ResetData();
   
   // Add unique Ids of narrative values
   for (auto& s : shots_)
   {
      shotIds_.push_back(s.GetId());
      AddValueIfUnique(uniqueStructureValues_, s.GetStructure());
      AddValueIfUnique(uniqueGoreValues_, s.GetGore());
      AddValueIfUnique(uniqueThrillValues_, s.GetThrill());
      AddValueIfUnique(uniqueEmpathyValues_, s.GetEmpathy());
      AddValueIfUnique(uniqueChaseValues_, s.GetChase());
      AddValueIfUnique(uniqueHideValues_, s.GetHide());
      AddValueIfUnique(uniqueKillValues_, s.GetKill());
      AddValueIfUnique(uniqueAttackValues_, s.GetAttack());
      AddValueIfUnique(uniqueEscapeValues_, s.GetEscape());
      AddValueIfUnique(uniqueLocationValues_, (int)s.GetLocation());
      AddValueIfUnique(uniqueTimeDurations_, s.GetTimeDuration());
      
      if (s.HasCharacters())
      {
         for (auto& c : s.GetCharacterList())
         {            
            Character ch = s.GetCharacter(c);
            assert(!Character::IsNullCharacter(&ch));
            AddValueIfUnique(uniqueCharacters_, (int)ch.GetCharName());      
         }
      }
   }

   // Construct vector of character names
   for (int character : uniqueCharacters_)
   {
      allCharacterNames_.push_back(NarrativeEnums::GetCharName((eChar)character));
   }

   // Total duration for each location
   for (int loc : uniqueLocationValues_)
   {
      int totalDuration = 0;

      for (auto& s : shots_)
      {
         if ((int)s.GetLocation() == loc)
         {
            totalDuration += s.GetTimeDuration();
         }
      }

      locationDurationValues_[loc] = totalDuration;
   }

   // Character appearance rates for each location
   for (int loc : uniqueLocationValues_)
   {
      vector<Shot*> shotsInThisLocation;

      for (auto& s : shots_)
      {
         if (s.GetLocation() == loc)
         {
            shotsInThisLocation.push_back(&s);
         }
      }

      for (int c : uniqueCharacters_)
      {
         TwoInt characterAppearance;
         characterAppearance.First = c;
         float numAppearances = 0;

         for (auto& shots : shotsInThisLocation)
         {
            for (auto& characters : shots->GetCharacters())
            {
               if (characters.GetCharName() == c)
               {
                  numAppearances++;
               }
            }
         }

         int appearanceRate = (numAppearances / shotsInThisLocation.size()) * 100;
         characterAppearance.Second = appearanceRate;
         characterAppearanceRates_[loc].push_back(characterAppearance);

         // Cache unique value
         AddValueIfUnique(uniqueCharacterAppearanceRates_, appearanceRate);
      }
   }

   // Max cost attainable at each location
   for (int loc : uniqueLocationValues_)
   {
      int cost = CalculateMaxCostAtLocation(loc, shots_);
      maxCostForLocations_[loc] = cost; 
      AddValueIfUnique(uniqueCostValues_, cost);
   }
}