//
// A general class for solving a gecode problem according to it's
// configuratin options and constraint profile
//
#pragma once
#include "common.h"
#include "GWorkerOptions.h"
#include "IConstraintProfile.h"

class GWorker : public Gecode::Space
{

public:
   GWorker(const GWorkerOptions &opt);
   GWorker(bool share, GWorker& s);
   ~GWorker();

   virtual Space* copy(bool share) override;
   void print(std::ostream& os) const;

   Gecode::IntVarArray* GetVariables() { return &variables_; }
   Gecode::IntVar* GetCost() { return &cost_; }
   Gecode::IntVar* GetDuration() { return &duration_; }
   Gecode::IntVar* GetMainCharacterAppearance() { return &mainCharacterAppearance_; }
   Gecode::IntVar* GetVariantDistance() { return &variantDistance_; }

private:

   Gecode::IntVarArray variables_;
   Gecode::IntVar cost_;
   Gecode::IntVar duration_;
   Gecode::IntVar mainCharacterAppearance_;
   Gecode::IntVar variantDistance_;

   IConstraintProfile* profile_;
   ProfilePackage profilePackage_;
};

