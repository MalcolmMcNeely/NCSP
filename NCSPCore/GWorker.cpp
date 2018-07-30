//
// Implementation of GWorker.h
//
#include "GWorker.h"
#include "IConstraintProfile.h"

#include <vector>

using namespace Gecode;

GWorker::GWorker(const GWorkerOptions &opt) : cost_(*this, COSTVARMIN, COSTVARMAX),
                                              duration_(*this, DURATIONVARMIN, DURATIONVARMAX),
                                              mainCharacterAppearance_(*this, CHARACTERAPPEARANCEMIN, CHARACTERAPPEARANCEMAX),
                                              variantDistance_(*this, VARIANTDISTANCEMIN, VARIANTDISTANCEMAX),
                                              Space()
{
   profile_ = opt.GetProfile();
   profilePackage_.Library = opt.GetLibrary();
   profilePackage_.Output = opt.GetOutput();
   profilePackage_.Input = opt.GetInput();
   profilePackage_.Costs = opt.GetCosts();
   
   int temp = 0;
   if (opt.GetArgument(eOpt_SolutionSize, &temp))
   {
      profilePackage_.SolutionSize = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_Duration, &temp))
   {
      profilePackage_.Duration = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_DurationBound, &temp))
   {
      profilePackage_.DurationBound = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_ViewerType, &temp))
   {
      profilePackage_.ViewerType = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_MinCost, &temp))
   {
      profilePackage_.MinCost = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_MainCharacter, &temp))
   {
      profilePackage_.MainCharacter = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_MainCharAppearanceRate, &temp))
   {
      profilePackage_.CharacterAppearanceRateThreshold = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_VariantDistance, &temp))
   {
      profilePackage_.VariantDistance = temp;
   }
   temp = 0;

   if (opt.GetArgument(eOpt_MinMaxSolution, &temp))
   {
      if (temp == (int)eSType_Min)
      {
         profilePackage_.BranchingBehaviour = eBB_Min;         
      }
      else if (temp == (int)eSType_Max)
      {
         profilePackage_.BranchingBehaviour = eBB_Max;
      }      
   }
   temp = 0;

   profile_->Initialise(this, &profilePackage_);
   profile_->Constrain(this, &profilePackage_);
   profile_->Branch(this, &profilePackage_);
}

GWorker::GWorker(bool share, GWorker& s) : Space(share, s)
{
   // Update variables when this space is copied in Gecode
   variables_.update(*this, share, s.variables_);
   cost_.update(*this, share, s.cost_);
   duration_.update(*this, share, s.duration_);
   mainCharacterAppearance_.update(*this, share, s.mainCharacterAppearance_);
   
   variantDistance_.update(*this, share, s.variantDistance_);

   profile_ = s.profile_;
   profilePackage_.Overwrite(s.profilePackage_);
}

GWorker::~GWorker()
{
}

Space* GWorker::copy(bool share)
{
   return new GWorker(share, *this);
}

void GWorker::print(std::ostream& os) const
{
   profile_->ReturnOutput(os, this, &profilePackage_);
}
