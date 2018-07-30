//
// Class container for a library of shots and associated
// data access functions
//
#pragma once

#include "common.h"

#include <vector>
#include <map>
#include <string>

class Shot;

class ShotLibrary
{
public:
   ShotLibrary();
   ~ShotLibrary();

   // Returns false if shot id is already included in the library
   bool AddShot(Shot& shot);
   bool RemoveShotById(int id);
   void Finalize();
	void Clear();
   void SetViewerType(int vt);
   int GetViewerType() { return viewerType_; }
   std::vector<Shot> GetShots() const;
   int GetMaxNumShotVariables() const;
   bool GetIsFinalized() const;
	Shot GetShotById(int id) const;
   int GetShotLibSize() const;
   int GetShotIdByIndex(int index) const;
   int GetStructureByIndex(int index) const;
   int GetGoreByIndex(int index) const;
   int GetEmpathyByIndex(int index) const;
   int GetLocationByIndex(int index) const;
   int GetTimeDurationByIndex(int index) const;
   int GetShotNumCharactersByIndex(int index) const;
   std::string GetCharacterNameInShotByIndex(int shotIndex, int charIndex);

   // Numerical values only for domain data acquisition and validation
   std::vector<int> GetShotIds() const { return shotIds_; }
   std::vector<int> GetUniqueStructureValues() const { return uniqueStructureValues_; }
   std::vector<int> GetUniqueGoreValues() const { return uniqueGoreValues_; }
   std::vector<int> GetUniqueThrillValues() const { return uniqueThrillValues_; }
   std::vector<int> GetUniqueEmpathyValues() const { return uniqueEmpathyValues_; }
   std::vector<int> GetUniqueChaseValues() const { return uniqueChaseValues_; }
   std::vector<int> GetUniqueHideValues() const { return uniqueHideValues_; }
   std::vector<int> GetUniqueKillValues() const { return uniqueKillValues_; }
   std::vector<int> GetUniqueAttackValues() const { return uniqueAttackValues_; }
   std::vector<int> GetUniqueEscapeValues() const { return uniqueEscapeValues_; }
   std::vector<int> GetUniqueLocationValues() const { return uniqueLocationValues_; }
   std::vector<int> GetUniqueTimeDurations() const { return uniqueTimeDurations_; }
   std::vector<int> GetUniqueCharacters() const { return uniqueCharacters_; }
   
	// Duration
   std::vector<int> GetRealTimeDurations();
   bool GetTotalDurationForLocation(int loc, int* out);
   std::map<int, int> GetLocationDurationValues() { return locationDurationValues_; }

	// Character appearance
   bool GetCharacterAppearanceForLocation(int loc, int c, int* out);
   std::map<int, std::vector<TwoInt>> GetCharacterAppearanceRates() { return characterAppearanceRates_; }
   std::vector<int> GetUniqueCharacterAppearanceRates() { return uniqueCharacterAppearanceRates_; }
   
	// Cost
	bool GetMaxCostAtLocation(int loc, int* out);
	int CalculateMaxCostAtLocation(int loc, std::vector<Shot>& shots);
	int CalculateCostForShot(Shot& shot);
   std::map<int, int> GetMaxCostForLocations() { return maxCostForLocations_; }
	std::vector<int> GetUniqueCostValues() { return uniqueCostValues_; }   

   // String conversions used by GUI
   std::vector<std::string>* GetAllCharacterNames() { return &allCharacterNames_; }

   // Logging functions
   void LogShotsAtLocation(int loc, std::string filename);

   void BeginEdit() { bIsFinalized_ = false; }
   void EndEdit() { Finalize(); }

private:

   void ResetData();
   void CalculateData();   

   int viewerType_;
   bool bIsFinalized_;   
   std::vector<Shot> shots_;

   std::vector<int> shotIds_;
   std::vector<int> uniqueStructureValues_;
   std::vector<int> uniqueGoreValues_;
   std::vector<int> uniqueThrillValues_;
   std::vector<int> uniqueEmpathyValues_;
   std::vector<int> uniqueChaseValues_;
   std::vector<int> uniqueHideValues_;
   std::vector<int> uniqueKillValues_;
   std::vector<int> uniqueAttackValues_;
   std::vector<int> uniqueEscapeValues_;
   std::vector<int> uniqueLocationValues_;
   std::vector<int> uniqueTimeDurations_;
   std::vector<int> uniqueCharacters_;

   std::vector<std::string> allCharacterNames_;
   std::map<int, int> locationDurationValues_;

   // characterAppearanceRates_ maps as <location, <char, appearance rate>>
   std::map<int, std::vector<TwoInt>> characterAppearanceRates_;
   std::vector<int> uniqueCharacterAppearanceRates_;

   std::map<int, int> maxCostForLocations_;
   std::vector<int> uniqueCostValues_;
};

