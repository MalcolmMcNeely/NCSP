//
// Constraint profile for determining a segment of a story
//
#pragma once
#include "IConstraintProfile.h"
#include "NarrativeEnums.h"
#include <map>

class StoryProfile : public IConstraintProfile
{
public:
   StoryProfile();
   ~StoryProfile();

   void Initialise(GWorker* worker, ProfilePackage* package) override;
   virtual void Constrain(GWorker* worker, ProfilePackage* package) override;
   virtual void Branch(GWorker* worker, ProfilePackage* package) override;
   void ReturnOutput(std::ostream& os, const void* worker, const void* package) override;

protected:

   int GetCharacterRow(eChar ch);

   int vRows_, vCols_;
   std::map<eRowProperties, int> rowKey_;

private:

   virtual void ApplyDomains(GWorker* worker, ProfilePackage* package) override;
   virtual void DefineValidShotData(GWorker* worker, ProfilePackage* package) override;

};

