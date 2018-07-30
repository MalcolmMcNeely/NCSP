//
// Implementation of StoryProfileWithDistance.h
//
#include "StoryProfileVariant.h"
#include "GWorker.h"
#include "ShotLibrary.h"
#include "Shot.h"
#include "Character.h"
#include "Utility.h"
#include "GOutput.h"

using namespace Gecode;
using namespace std;
using namespace Utility;

StoryProfileVariant::StoryProfileVariant()
{
}

StoryProfileVariant::~StoryProfileVariant()
{
}

void StoryProfileVariant::Constrain(GWorker* worker, ProfilePackage* package)
{
   // Call Base function
   StoryProfile::Constrain(worker, package);
	
   // Initialise variables
   Space* home = (Space*)worker;
   auto lib = package->Library;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   int distance = package->VariantDistance;
   auto variantDistance = worker->GetVariantDistance();

   // Input variables
   auto gInput = *(package->Input);
   vector<int> processedInputUniqueValues;
   int averageSolutionSize = 0;
   int numberOfSets = 0;
   vector<vector<int>> processedInput = ProcessInput(&gInput, 
                                                     &processedInputUniqueValues,
                                                     &numberOfSets, 
                                                     &averageSolutionSize);

   // Initialise set variables
   IntSet previousSolutonDomainSet(processedInputUniqueValues.data(), processedInputUniqueValues.size());
   auto fullDomain = lib->GetShotIds();
   IntSet fullDomainSet(fullDomain.data(), fullDomain.size());
   auto s1 = SetVar(*home, IntSet::empty, fullDomainSet);
   auto s2 = SetVarArgs(*home, numberOfSets, IntSet::empty, previousSolutonDomainSet);

   // Map to current solution
   IntVarArgs inv(*home, vCols_, fullDomainSet);
   for (int i = 0; i < vCols_; i++)
   {
      rel(*home, inv[i], IRT_EQ, mat(i, 0));
   }
   channel(*home, inv, s1);

	// Constrain the number of distinct values WRT diversity distance + 1 offset for NOSHOT
	nvalues(*home, inv, IRT_GQ, std::max((averageSolutionSize - distance) + 1, 0));
	nvalues(*home, inv, IRT_LQ, std::min((averageSolutionSize + distance) + 1, vCols_));

   // Map to all previous solutions
   vector<IntVarArgs> inv2;
   for (int i = 0; i < numberOfSets; i++)
   {
      // Initialise and insert gecode variable
		int solutionSize = processedInput[i].size();
		inv2.push_back(IntVarArgs(*home, solutionSize, previousSolutonDomainSet));
		for (int j = 0; j < solutionSize; j++)
      {
         // Mapping
         rel(*home, inv2[i][j], IRT_EQ, processedInput[i][j]);
      }

      // Channel values of previous solution into Set
      channel(*home, inv2[i], s2[i]);
   }

   // Unique difference between current solution and all previous sets
   auto diffs = SetVarArgs(*home, numberOfSets, IntSet::empty, fullDomainSet);
   for (int i = 0; i < numberOfSets; i++)
   {
      rel(*home, diffs[i] == s2[i] - s1);
   }
   
   // Store difference size between sets
	auto diffv = IntVarArgs(*home, numberOfSets, 0, 20);//IntVarArgs(*home, numberOfSets, fullDomainSet);
   for (int i = 0; i < numberOfSets; i++)
   {
      cardinality(*home, diffs[i], diffv[i]);
   }

	// Specify the desired variant distance
	IntVar sumdiff(*home, 0, 100);
   rel(*home, sumdiff == (sum(diffv) / numberOfSets));
   rel(*home, *variantDistance == sumdiff);
   rel(*home, *variantDistance == distance);
	
}

void StoryProfileVariant::Branch(GWorker* worker, ProfilePackage* package)
{
   Space* home = (Space*)worker;
   auto vars = worker->GetVariables();
   Matrix<IntVarArray> mat(*vars, vCols_, vRows_);
   auto mainCharacterAppearance = worker->GetMainCharacterAppearance();

   Rnd r;
   r.time();
   Gecode::branch(*home, mat.row(0), INT_VAR_NONE(), INT_VAL_RND(r));
   Gecode::branch(*home, *mainCharacterAppearance, INT_VAL_MAX());
}

vector<vector<int>> StoryProfileVariant::ProcessInput(vector<GOutput*>* in,
                                                      std::vector<int>* uniqueValues,
                                                      int* numSets,
                                                      int* avgSolutionSize)
{
   vector<vector<int>> out;
   int avgSize = 0;

   // Gather data from all previous solutions
   for (auto g : *in)
   {
      vector<int> temp;

      if (g->GetData("Solution", &temp))
      {
			out.push_back(temp);
      }      
   }   

   // Remove NOSHOTs from previous solutions before processing and calculate
   // average solution length / max solution size
   for (auto& input : out)
   {
      input.erase(remove_if(begin(input), end(input),
         [](int i){return (i == NOSHOTID); }),
         end(input));

      avgSize += input.size();
   }

   *avgSolutionSize = avgSize / out.size();
   *numSets = out.size();
   *uniqueValues = ExtractUniqueValues(out);
   return out;
}
