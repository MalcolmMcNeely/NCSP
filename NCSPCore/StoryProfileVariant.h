//
// Constraint profile for determining variation of a previously generated story segment
//
#pragma once
#include "IConstraintProfile.h"
#include "StoryProfile.h"

class StoryProfileVariant : public StoryProfile
{
public:
   StoryProfileVariant();
   ~StoryProfileVariant();

   void Constrain(GWorker* worker, ProfilePackage* package) override;
   void Branch(GWorker* worker, ProfilePackage* package) override;

private:
   std::vector<std::vector<int>> ProcessInput(std::vector<GOutput*>* in,
                                              std::vector<int>* uniqueValues,
                                              int* numSets,
                                              int* avgSolutionSize);
};

