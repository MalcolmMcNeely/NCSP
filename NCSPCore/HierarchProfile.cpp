//
// Implementation of HierarchProfile.h
//
#include "HierarchProfile.h"
#include "GWorker.h"
#include "ShotLibrary.h"
#include "Utility.h"

using namespace Gecode;
using namespace Utility;
using namespace std;
using namespace NarrativeEnums;

HierarchProfile::HierarchProfile()
{
}

HierarchProfile::~HierarchProfile()
{
}

void HierarchProfile::Initialise(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   if (package->Library != nullptr)
   {
      auto lib = package->Library;

      if (package->SolutionSize > 0)
      {
         vCols_ = package->SolutionSize;

         if (lib)
         {
            auto locations = lib->GetUniqueLocationValues();

            if (locations.size() > 0)
            {
               auto characters = lib->GetUniqueCharacters();

               if (characters.size() > 0)
               {
                  // Location, Total duration, character, character appearance rate, costs
                  vRows_ = 5;
                  auto variables = worker->GetVariables();
                  *variables = IntVarArray(*home, vCols_ * vRows_);
                  ApplyDomains(worker, package);
                  DefineValidShotData(worker, package);
               }
            }
         }
      }
   }
}

void HierarchProfile::Constrain(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);

   int duration = package->Duration;
   int durationBound = package->DurationBound;

   // Each location must be different
   distinct(*home, mat.row(0));
   // Duration is within specified bounds
   rel(*home, sum(mat.row(1)) >= duration);
   // The same character is chosen for all segments
   nvalues(*home, mat.row(2), IRT_EQ, 1);

   // The character selected must have an appearance rate of t in all locations
   for (int i = 0; i < vCols_; i++)
   {
      rel(*home, mat(i, 3) >= package->CharacterAppearanceRateThreshold);
   }

   // If cost is specified then we must at least match it
   auto costs = package->Costs;
   if (!costs.empty())
   {
      // Sanity check
      assert(costs.size() == vCols_);

      for (int i = 0; i < vCols_; i++)
      {
         if (costs[i] > 0)
         {
            rel(*home, mat(i, 4) >= costs[i]);
         }
      }
   }
}

void HierarchProfile::Branch(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();

   Rnd r;
   r.time();
   branch(*home, *vars, INT_VAR_NONE(), INT_VAL_RND(r));
}

void HierarchProfile::ReturnOutput(std::ostream& os, const void* worker, const void* package)
{
   IntVarArray* variables = ((GWorker*)worker)->GetVariables();
   ProfilePackage* pPackage = (ProfilePackage*)package;
   Matrix<IntVarArray> mat(*variables, vCols_, vRows_);
   
   pPackage->Output->SetSolutionFound(true);

   os << endl;
   os << "Hierarch Profile Output: ";
   if (pPackage->SolutionSize > 0)
   {
      os << "size " << pPackage->SolutionSize << endl;
      os << "Rows: Location, Total Duration, Character, Character Appearance Rate, Max Cost" << endl;

      for (int i = 0; i < vCols_; i++)
      {
         os << GetLocationName((eLocation)mat(i, 0).val()) << "\t";
      }

      os << endl;

      for (int i = 0; i < vCols_; i++)
      {
         os << mat(i, 1).val() << "\t";
      }

      os << endl;

      for (int i = 0; i < vCols_; i++)
      {
         os << GetCharName((eChar)mat(i, 2).val()) << "\t";
      }

      os << endl;

      for (int i = 0; i < vCols_; i++)
      {
         os << mat(i, 3).val() << "\t";
      }

      os << endl;

      for (int i = 0; i < vCols_; i++)
      {
         os << mat(i, 4).val() << "\t";
      }

      os << endl;
   }

   GOutput* output = pPackage->Output;

   vector<int> locationData;
   for (int i = 0; i < vCols_; i++)
   {
      locationData.push_back(mat(i, 0).val());
   }
   output->AddData("Location", locationData);

   vector<int> durationData;
   for (int i = 0; i < vCols_; i++)
   {
      durationData.push_back(mat(i, 1).val());
   }
   output->AddData("TotalDuration", durationData);

   vector<int> characterData;
   for (int i = 0; i < vCols_; i++)
   {
      characterData.push_back(mat(i, 2).val());
   }
   output->AddData("Character", characterData);
}

void HierarchProfile::ApplyDomains(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   auto lib = package->Library;
   int currentRow = 0;

   // Location
   {
      auto domain = lib->GetUniqueLocationValues();

      // Remove dummy location if contained
      int dummyIndex = 0;
      bool dummyFound = false;
      for (int i = 0; i < domain.size(); i++)
      {
         if (domain[i] == (int)eL_NONE_DUMMY)
         {
            dummyFound = true;
            dummyIndex = i;
         }
      }

      if (dummyFound)
      {
         domain.erase(domain.begin() + dummyIndex);
      }

      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Total Duration
   {
      auto locationDurationValues = lib->GetLocationDurationValues();
      std:vector<int> domain;
      for (auto k : locationDurationValues)
      {
         domain.push_back(k.second);
      }

      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Characters
   {
      auto domain = lib->GetUniqueCharacters();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Character appearance rate
   {
      auto domain = lib->GetUniqueCharacterAppearanceRates();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Costs
   {
      auto domain = lib->GetUniqueCostValues();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }
}

void HierarchProfile::DefineValidShotData(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto variables = worker->GetVariables();
   auto lib = package->Library;

   auto locations = lib->GetUniqueLocationValues();
   auto characters = lib->GetUniqueCharacters();

   Matrix<IntVarArray> mat(*variables, vCols_, vRows_);
   vector<vector<int>> vArgs(locations.size() * characters.size());

   for (unsigned int i = 0; i < vArgs.size(); i++)
   {
      vArgs[i].resize(vRows_);
   }

   // Foreach location
   int pos = 0;
   for (unsigned int i = 0; i < locations.size(); i++)
   {
      int location = locations[i];
      int locationTotalDuration;

      // Location duration
      if (lib->GetTotalDurationForLocation(location, &locationTotalDuration))
      {
         int locationMaxCost;

         // Location max cost
         if (lib->GetMaxCostAtLocation(location, &locationMaxCost))
         {
            // Foreach character
            for (unsigned int j = 0; j < characters.size(); j++)
            {
               int characterAppearance;

               // Character appearance in location
               if (lib->GetCharacterAppearanceForLocation(locations[i], characters[j], &characterAppearance))
               {
                  vArgs[pos][0] = location;
                  vArgs[pos][1] = locationTotalDuration;
                  vArgs[pos][2] = characters[j];
                  vArgs[pos][3] = characterAppearance;
                  vArgs[pos][4] = locationMaxCost;
               }
               else
               {
                  throw std::exception("Character appearance for location not found in HierarchProfile.cpp");
               }

               pos++;
            }
         }
      }
      else
      {
         throw std::exception("Total duration for location not found in HierarchProfile.cpp");
      }
   }

   // Add and finalize data
   TupleSet validData;
   for (int i = 0; i < vArgs.size(); i++)
   {
      IntArgs args(vArgs[i]);
      validData.add(args);
   }
   validData.finalize();

   // Constrain columns to be valid shot data
   for (int i = 0; i < vCols_; i++)
   {
      extensional(*home, mat.col(i), validData);
   }
}

