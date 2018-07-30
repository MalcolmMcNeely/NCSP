#include "MinMaxProfile.h"
#include "GWorker.h"
#include "ShotLibrary.h"
#include "Shot.h"
#include "Utility.h"

#include <vector>
#include <algorithm>

using namespace Gecode;
using namespace std;

MinMaxProfile::MinMaxProfile()
{

}

MinMaxProfile::~MinMaxProfile()
{
}

void MinMaxProfile::Initialise(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   if (package->Library != nullptr)
   {
      auto lib = package->Library;

      if (lib)
      {
         if (lib->GetShotLibSize() > 0)
         {
            // ID, Cost, Duration
            vRows_ = 3 + (int)eC_SIZE;
            vCols_ = lib->GetShotLibSize();            

            auto variables = worker->GetVariables();
            *variables = IntVarArray(*home, vCols_ * vRows_);

            shotIds_.clear();
            costs_.clear();
            durations_.clear();
            auto shots = lib->GetShots();

            for (auto& s : shots)
            {
               shotIds_.push_back(s.GetId());
               costs_.push_back(lib->CalculateCostForShot(s));
               durations_.push_back(s.GetTimeDuration());
            }

            ApplyDomains(worker, package);
            DefineValidShotData(worker, package);
         }
      }      
   }
}

void MinMaxProfile::Constrain(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   auto lib = package->Library;

   int desiredDuration = package->Duration;
   int desiredDurationBound = package->DurationBound;
   int desiredMinDuration = desiredDuration - desiredDurationBound;
   int minCost = package->MinCost;
   int mainCharacterId = package->MainCharacter;
   int mainCharacterAppearanceRate = package->CharacterAppearanceRateThreshold;

   IntVar* cost = worker->GetCost();
   IntVar* duration = worker->GetDuration();
   IntVar* mainCharacterAppearance = worker->GetMainCharacterAppearance();

#pragma region Conditional Distinct

   {
      // Grab shots with a relevant shots (w/ duration > 0)
      auto shotIDs = lib->GetShotIds();
      int indexSizeWOZero = 0;

      for (int i = 0; i < shotIDs.size(); i++)
      {
         if (shotIDs[i] != NOSHOTID)
            indexSizeWOZero++;
      }

      IntArgs args(indexSizeWOZero);
      int argSize = 0;
      for (int i = 0; i < shotIDs.size(); i++)
      {
         if (shotIDs[i] != NOSHOTID)
         {
            args[argSize] = shotIDs[i];
            argSize++;
         }
      }

      // Relevant shots can only appear 0 or 1 times
      for (int i = 0; i < argSize; i++)
      {
         count(*home, mat.row(0), args[i], IRT_LQ, 1);
      }
   }

#pragma endregion

#pragma region Cost

   // Map cost
   rel(*home, *cost == sum(mat.row(1)));
   // Cost threshold
   rel(*home, *cost >= minCost);

#pragma endregion

#pragma region Duration

   {
      // If bound is 0, there is only 1 short shot in the library
      if (desiredDurationBound > 0)
      {
         //auto dDomain = lib->GetUniqueTimeDurations();
         //IntSet dDomainSet(dDomain.data(), dDomain.size());
         //IntVarArgs d(*home, vCols_, dDomainSet);
         //for (int i = 0; i < vCols_; i++)
         //{
         //   rel(*home, d[i] == mat(i, 2));
         //}
         //rel(*home, *duration == sum(d));

         rel(*home, *duration == sum(mat.row(2)));

         if ((desiredDuration - desiredDurationBound) > 0)
         {
            rel(*home, *duration >= (desiredDuration - desiredDurationBound));
         }

         if ((desiredDuration + desiredDurationBound) > 0)
         {
            rel(*home, *duration <= (desiredDuration + desiredDurationBound));
         }
      }
   }

#pragma endregion

#pragma region Redundant Constraints

   {
      // Find min shots needed to fufill duration equirements
      auto durations = lib->GetRealTimeDurations();
      int minShotsNeeded = 0;
      int currentTime = 0;

      sort(durations.begin(), durations.end(), [](int a, int b){ return b < a; });
      for (int i = 0; i < durations.size(); i++)
      {
         currentTime += durations[i];
         minShotsNeeded++;

         if (currentTime > desiredMinDuration)
            break;
      }
      minShotsNeeded = std::min(minShotsNeeded, vCols_);

      // Both duration and ID of these shots cannot be empty
      for (int i = 0; i < minShotsNeeded; i++)
      {
         rel(*home, mat(i, 0), IRT_NQ, NOSHOTID);
         rel(*home, mat(i, 2), IRT_NQ, 0);
      }
   }

#pragma endregion

#pragma region Ordering

   // Symmetry breaking (Ordering)
   for (int i = 0; i < vCols_ - 1; i++)
   {
      rel(*home, mat(i, 0) >= mat(i + 1, 0));
   }

#pragma endregion

#pragma region Desired Character Appearance

   {
      rel(*home, *mainCharacterAppearance == sum(mat.row(GetCharacterRow((eChar)mainCharacterId))));
   }

#pragma endregion

}

void MinMaxProfile::Branch(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   IntVar* cost = worker->GetCost();
   IntVar* duration = worker->GetDuration();
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   IntVar* mainCharacterAppearance = worker->GetMainCharacterAppearance();
   eBranchingBehaviour branching = (eBranchingBehaviour)package->BranchingBehaviour;

   Rnd r;
   r.time();

   //branch(*home, mat.row(0), INT_VAR_NONE(), INT_VAL_MIN());

   if (branching == eBB_Max)
   {
      //branch(*home, mat.row(1), INT_VAR_NONE(), INT_VAL_MIN());
      branch(*home, *cost, INT_VAL_MAX());
      //branch(*home, mat.row(2), INT_VAR_NONE(), INT_VAL_MIN());
      branch(*home, *duration, INT_VAL_MAX());      
   }
   else // eBB_Min
   {
      //branch(*home, mat.row(1), INT_VAR_NONE(), INT_VAL_MAX());
      branch(*home, *cost, INT_VAL_MIN());
      //branch(*home, mat.row(2), INT_VAR_NONE(), INT_VAL_MAX());
      branch(*home, *duration, INT_VAL_MIN());      
   }  

   branch(*home, *mainCharacterAppearance, INT_VAL_MAX());
}

void MinMaxProfile::ReturnOutput(std::ostream& os, const void* worker, const void* package)
{
   IntVarArray* variables = ((GWorker*)worker)->GetVariables();
   ProfilePackage* pPackage = (ProfilePackage*)package;
   Matrix<IntVarArray> mat(*variables, vCols_, vRows_);
   IntVar* cost = ((GWorker*)worker)->GetCost();

   pPackage->Output->SetSolutionFound(true);

   int sSize = 0;
   int duration = 0;
   int c = 0;

   if (pPackage->SolutionSize > 0)
   {
      for (int j = 0; j < vCols_; j++)
      {
         if (mat(j, 0).val() != NOSHOTID)
         {
            os << mat(j, 0).val() << "\t";
            duration += mat(j, 2).val();
            sSize++;
         }
         else
         {
            break;
         }
      }

      os << endl;
   }

   if (cost->assigned())
   {
      os << "Cost:\t" << cost->val() << endl;
      c = cost->val();
   }
   else
   {
      os << "Cost:\t0" << endl;
   }

   // Populate output data
   GOutput* output = pPackage->Output;
   vector<int> solutionData;
   for (int i = 0; i < vCols_; i++)
   {
      solutionData.push_back(mat(i, 0).val());
   }
   output->AddData("Solution", solutionData);
   output->AddData("Solution Size", sSize);
   output->AddData("Library Size", pPackage->SolutionSize);
   output->AddData("TotalDuration", duration);
   output->AddData("Cost", c);

   if (pPackage->BranchingBehaviour == (int)eBB_Min)
   {
      output->SetSolutionType(eSType_Min);
   }
   else if (pPackage->BranchingBehaviour == (int)eBB_Max)
   {
      output->SetSolutionType(eSType_Max);
   }   
}

void MinMaxProfile::ApplyDomains(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto lib = package->Library;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   int currentRow = 0;

   // ID
   {
      IntSet domainSet(shotIds_.data(), shotIds_.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Costs
   {
      IntSet domainSet(costs_.data(), costs_.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // Durations
   {
      IntSet domainSet(durations_.data(), durations_.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // For unique each character
   charStartRow_ = currentRow;
   for (int i = 0; i < (int)eC_SIZE; i++)
   {
      // row character is present (true/false)
      {
         IntSet domainSet(0, 1);
         ApplyDomainToRow(currentRow, home, &mat, &domainSet);
      }
   }
}

void MinMaxProfile::DefineValidShotData(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto variables = worker->GetVariables();
   Matrix<IntVarArray> mat(*variables, vCols_, vRows_);
   auto lib = package->Library;
   vector<Shot> shots = lib->GetShots();
   vector<vector<int>> vArgs(vCols_);

   for (unsigned int i = 0; i < shots.size(); i++)
   {
      vArgs[i].resize(vRows_);
   }

   for (unsigned int i = 0; i < shots.size(); i++)
   {
      vArgs[i][0] = shotIds_[i];
      vArgs[i][1] = costs_[i];
      vArgs[i][2] = durations_[i];

      // For each character in shot
      auto charList = shots[i].GetCharacterList();
      for (int j = 0; j < eC_SIZE; j++)
      {
         int currentCharacterRow = charStartRow_ + (j);

         // If character is in the shot
         if (Utility::VectorContains(charList, (eChar)j))
         {
            //auto currentChar = shots[i].GetCharacter((eChar)j);
            vArgs[i][currentCharacterRow + (int)eCP_NAME] = true;
            //vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = currentChar.GetAlive();
         }
         else
         {
            vArgs[i][currentCharacterRow + (int)eCP_NAME] = false;
            //vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = 0;
         }
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

int MinMaxProfile::GetCharacterRow(eChar ch)
{
   int out = charStartRow_;
   //out += ((int)ch * Character::NumMembers());
   out += ((int)ch);
   return out;
}
