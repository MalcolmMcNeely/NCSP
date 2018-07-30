#pragma once
#include "IConstraintProfile.h"

class MinMaxProfile : public IConstraintProfile
{
public:
   MinMaxProfile();
   ~MinMaxProfile();

   void Initialise(GWorker* worker, ProfilePackage* package) override;
   void Constrain(GWorker* worker, ProfilePackage* package) override;
   void Branch(GWorker* worker, ProfilePackage* package) override;
   void ReturnOutput(std::ostream& os, const void* worker, const void* package) override;

private:

   void ApplyDomains(GWorker* worker, ProfilePackage* package);
   void DefineValidShotData(GWorker* worker, ProfilePackage* package);
   int GetCharacterRow(eChar ch);

   int vCols_;
   int vRows_;
   int charStartRow_;

   std::vector<int> costs_;
   std::vector<int> durations_;
   std::vector<int> shotIds_;
};

