// 
// Constraint profile for determining the top level of the story:
// locations, main characters
//
#pragma once
#include "IConstraintProfile.h"

class HierarchProfile : public IConstraintProfile
{
public:
   HierarchProfile();
   ~HierarchProfile();

   void Initialise(GWorker* worker, ProfilePackage* package) override;
   void Constrain(GWorker* worker, ProfilePackage* package) override;
   void Branch(GWorker* worker, ProfilePackage* package) override;
   void ReturnOutput(std::ostream& os, const void* worker, const void* package) override;

private:

   void ApplyDomains(GWorker* worker, ProfilePackage* package) override;
   void DefineValidShotData(GWorker* worker, ProfilePackage* package) override;

   int vRows_, vCols_;
};

