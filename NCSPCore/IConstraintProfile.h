// 
// Interface w/ general function(s) for creating a profile of constraints
// for the GWorker class
//
#pragma once
#include "common.h"
#include "ShotLibrary.h"
#include "GOutput.h"

class GWorker;

// Struct to hold NCSP configuration data
// Gecode will copy this with each new state, so I 
// have included an overwrite function
struct ProfilePackage
{
   void Overwrite(const ProfilePackage& other)
   {
      Library = other.Library;
      Output = other.Output;
      Input = other.Input;
      SolutionSize = other.SolutionSize;      
      ViewerType = other.ViewerType;
      Duration = other.Duration;
      DurationBound = other.DurationBound;
      MinCost = other.MinCost;
      CharacterAppearanceRateThreshold = other.CharacterAppearanceRateThreshold;
      MainCharacter = other.MainCharacter;
      for (int i : other.Costs)
      {
         Costs.push_back(i);
      }
      VariantDistance = other.VariantDistance;
      BranchingBehaviour = other.BranchingBehaviour;
   }

   ShotLibrary* Library;
   GOutput* Output;
   std::vector<GOutput*>* Input;
   int SolutionSize;
   int ViewerType;
   int Duration;
   int DurationBound;
   int MinCost;
   int CharacterAppearanceRateThreshold;
   int MainCharacter;
   std::vector<int> Costs;
   int VariantDistance;
   int BranchingBehaviour;
};

class IConstraintProfile
{
public:
   IConstraintProfile();
   virtual ~IConstraintProfile();

   virtual void Initialise(GWorker* worker, ProfilePackage* package) = 0;
   virtual void Constrain(GWorker* worker, ProfilePackage* package) = 0;
   virtual void Branch(GWorker* worker, ProfilePackage* package) = 0;
   virtual void ReturnOutput(std::ostream& os, const void* worker, const void* package) = 0;

protected:

	virtual void ApplyDomains(GWorker* worker, ProfilePackage* package) = 0;
	virtual void DefineValidShotData(GWorker* worker, ProfilePackage* package) = 0;

	void ApplyDomainToRow(int& currentRow,
								 Gecode::Space* pSpace,
								 Gecode::Matrix<Gecode::IntVarArray>* pMatrix,
								 Gecode::IntSet* pDomain);
};

