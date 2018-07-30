//
// Implementation of StoryProfile.h
//
#include "StoryProfile.h"
#include "GWorker.h"
#include "ShotLibrary.h"
#include "Shot.h"
#include "Character.h"
#include "Utility.h"

using namespace Gecode;
using namespace std;
using namespace Utility;

StoryProfile::StoryProfile()
{
}

StoryProfile::~StoryProfile()
{
}

void StoryProfile::Initialise(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   if (package->Library != nullptr)
   {
      auto lib = package->Library;
      int rowCount = 0;

      if (package->SolutionSize > 0)
      {
         vCols_ = package->SolutionSize;

         if (lib)
         {
            vRows_ = (lib->GetMaxNumShotVariables()); // Zero-index
            auto variables = worker->GetVariables();
            *variables = IntVarArray(*home, vCols_ * vRows_);
            ApplyDomains(worker, package);
            DefineValidShotData(worker, package);
            Constrain(worker, package);
         }
      }
   }
}

void StoryProfile::Constrain(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto lib = package->Library;
   auto vars = worker->GetVariables();
   eViewerType vType = (eViewerType)package->ViewerType;
   auto duration = worker->GetDuration();

   int desiredDuration = package->Duration;
   int desiredDurationBound = package->DurationBound;
   int desiredMinDuration = desiredDuration - desiredDurationBound;
   int minCost = package->MinCost;
   int mainCharacterId = package->MainCharacter;
   int mainCharacterAppearanceRate = package->CharacterAppearanceRateThreshold;

   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   IntVar* cost = worker->GetCost();
   IntVar* mainCharacterAppearance = worker->GetMainCharacterAppearance();

#pragma region Cost Function
   
   if (minCost > 0)
   {
      // Cost
      IntVarArgs gore(*home, vCols_, 0, 100);
      IntVarArgs thrill(*home, vCols_, 0, 100);

      for (int i = 0; i < vCols_; i++)
      {
         rel(*home, gore[i] == (1 + (mat(i, rowKey_[eP_ATTACK]) + mat(i, rowKey_[eP_KILL]))) *
            mat(i, rowKey_[eP_GORE]));

         rel(*home, thrill[i] ==
            (1 + (mat(i, rowKey_[eP_EMPATHY]) + mat(i, rowKey_[eP_ESCAPE]) +
            mat(i, rowKey_[eP_HIDE]) + mat(i, rowKey_[eP_THREATEN])
            + mat(i, rowKey_[eP_CHASE]))) *
            mat(i, rowKey_[eP_THRILL]));
      }

      if (vType == eV_GORE)
      {
         rel(*home, *cost == sum(gore) - sum(thrill));
      }
      else // Thrill
      {
         rel(*home, *cost == sum(thrill) - sum(gore));
      }

      // Cost threshold
      rel(*home, *cost >= minCost);
   }

#pragma endregion

#pragma region Duration

   {
      // If bound is 0, there is only 1 short shot in the library
      if (desiredDurationBound > 0)
      {
         auto dDomain = lib->GetUniqueTimeDurations();
         IntSet dDomainSet(dDomain.data(), dDomain.size());
         IntVarArgs d(*home, vCols_, dDomainSet);

         for (int i = 0; i < vCols_; i++)
         {
            rel(*home, d[i] == mat(i, rowKey_[eP_TIMEDURATION]));
         }
         rel(*home, *duration == sum(d));

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
         rel(*home, mat(i, rowKey_[eP_SHOTID]), IRT_NQ, NOSHOTID);
         rel(*home, mat(i, rowKey_[eP_TIMEDURATION]), IRT_NQ, 0);
      }
   }

#pragma endregion

#pragma region Ordering

   {
      // Symmetry breaking
      int temp = vCols_ - 1;
      for (int i = 0; i < temp; i++)
      {
         rel(*home, mat(i, rowKey_[eP_SHOTID]), IRT_LQ, mat(i + 1, rowKey_[eP_SHOTID]));
      }
   }

#pragma endregion

#pragma region Desired Character Appearance

   {
      rel(*home, *mainCharacterAppearance == sum(mat.row(GetCharacterRow((eChar)mainCharacterId))));
   }

#pragma endregion

}

void StoryProfile::Branch(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   IntVar* mainCharacterAppearance = worker->GetMainCharacterAppearance();

   Rnd r;
   r.time();
   Gecode::branch(*home, mat.row(0), INT_VAR_NONE(), INT_VAL_RND(r));
   Gecode::branch(*home, *mainCharacterAppearance, INT_VAL_MAX());
}

void StoryProfile::ReturnOutput(std::ostream& os, const void* worker, const void* package)
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
            duration += mat(j, rowKey_[eP_TIMEDURATION]).val();
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
   vector<int> durationData;
   for (int i = 0; i < vCols_; i++)
   {
      solutionData.push_back(mat(i, 0).val());
      durationData.push_back(mat(i, rowKey_[eP_TIMEDURATION]).val());
   }
   output->AddData("Solution", solutionData);
   output->AddData("Durations", durationData);
   output->AddData("Solution Size", sSize);
   output->AddData("Library Size", pPackage->SolutionSize);
   output->AddData("TotalDuration", duration);
   output->AddData("Cost", c);
   output->AddLocation(mat(0, rowKey_[eP_LOCATION]).val());

}

void StoryProfile::ApplyDomains(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   auto lib = package->Library;
   int currentRow = 0;

   rowKey_[eP_SHOTID] = currentRow;
   {
      auto domain = lib->GetShotIds();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_STRUCTURE] = currentRow;
   {
      IntSet domainSet(0, 2);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_GORE] = currentRow;
   {
      auto domain = lib->GetUniqueGoreValues();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_THRILL] = currentRow;
   {
      auto domain = lib->GetUniqueThrillValues();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_EMPATHY] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_THREATEN] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_CHASE] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_HIDE] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_KILL] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_ATTACK] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_ESCAPE] = currentRow;
   {
      IntSet domainSet(0, 1);
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_LOCATION] = currentRow;
   {
      auto domain = lib->GetUniqueLocationValues();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   rowKey_[eP_TIMEDURATION] = currentRow;
   {
      auto domain = lib->GetUniqueTimeDurations();
      IntSet domainSet(domain.data(), domain.size());
      ApplyDomainToRow(currentRow, home, &mat, &domainSet);
   }

   // For unique each character
   rowKey_[eP_CHARACTERSTARTROW] = currentRow;
   for (int i = 0; i < (int)eC_SIZE; i++)
   {
      // row character is present (true/false)
      {
         IntSet domainSet(0, 1);
         ApplyDomainToRow(currentRow, home, &mat, &domainSet);
      }
      // Alive
      {
         IntSet domainSet(0, 1);
         ApplyDomainToRow(currentRow, home, &mat, &domainSet);
      }
   }

   assert(currentRow == vRows_);
}

void StoryProfile::DefineValidShotData(GWorker* worker,
                                       ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto variables = worker->GetVariables();
   auto lib = package->Library;
   int tArgSize = vRows_;

   Matrix<IntVarArray> mat(*variables, vCols_, vRows_);
   vector<Shot> shots = lib->GetShots();
   vector<vector<int>> vArgs(shots.size());

   for (int i = 0; i < shots.size(); i++)
   {
      vArgs[i].resize(vRows_);
   }

   for (unsigned int i = 0; i < shots.size(); i++)
   {
      vArgs[i][rowKey_[eP_SHOTID]] = shots[i].GetId();
      vArgs[i][rowKey_[eP_STRUCTURE]] = shots[i].GetStructure();
      vArgs[i][rowKey_[eP_GORE]] = shots[i].GetGore();
      vArgs[i][rowKey_[eP_THRILL]] = shots[i].GetThrill();
      vArgs[i][rowKey_[eP_EMPATHY]] = shots[i].GetEmpathy();
      vArgs[i][rowKey_[eP_THREATEN]] = shots[i].GetThreaten();
      vArgs[i][rowKey_[eP_CHASE]] = shots[i].GetChase();
      vArgs[i][rowKey_[eP_HIDE]] = shots[i].GetHide();
      vArgs[i][rowKey_[eP_KILL]] = shots[i].GetKill();
      vArgs[i][rowKey_[eP_ATTACK]] = shots[i].GetAttack();
      vArgs[i][rowKey_[eP_ESCAPE]] = shots[i].GetEscape();
      vArgs[i][rowKey_[eP_LOCATION]] = shots[i].GetLocation();
      vArgs[i][rowKey_[eP_TIMEDURATION]] = shots[i].GetTimeDuration();

      // For each character in shot
      auto charList = shots[i].GetCharacterList();
      for (int j = 0; j < eC_SIZE; j++)
      {
         int currentCharacterRow = rowKey_[eP_CHARACTERSTARTROW] + (j * Character::NumMembers());

         // If character is in the shot
         if (Utility::VectorContains(charList, (eChar)j))
         {
            auto currentChar = shots[i].GetCharacter((eChar)j);
            vArgs[i][currentCharacterRow + (int)eCP_NAME] = true;
            vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = currentChar.GetAlive();
         }
         else
         {
            vArgs[i][currentCharacterRow + (int)eCP_NAME] = false;
            vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = 0;
         }
      }
   }

   // Add and finalize data
   TupleSet validData;
   for (int i = 0; i < shots.size(); i++)
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

int StoryProfile::GetCharacterRow(eChar ch)
{
   int out = rowKey_[eP_CHARACTERSTARTROW];
   out += ((int)ch * Character::NumMembers());
   return out;
}