//
// Implementation of GWorkerOptions.h
//
#include "GWorkerOptions.h"
#include "ShotLibrary.h"
#include "GOutput.h"
#include "IConstraintProfile.h"
#include "ConstraintEnums.h"

#include <assert.h>

using namespace std;

GWorkerOptions::GWorkerOptions(const char* pString) 
                               : Options(pString)
{
}


GWorkerOptions::~GWorkerOptions()
{
}

void GWorkerOptions::SetCosts(std::vector<int> costs)
{
   costs_.clear();
   for (int i : costs)
   {
      costs_.push_back(i);
   }
}

void GWorkerOptions::AddArgument(eSolverOption name, int value)
{
   assert(name != eOpt_Error);
   argMap_[name] = value;
}

bool GWorkerOptions::GetArgument(eSolverOption name, int* out) const
{
   assert(name != eOpt_Error);

   try
   {
      *out = argMap_.at(name);
   }
   catch (const std::out_of_range exception)
   {
      *out = 0;
      return false;
   }

   return true;
}