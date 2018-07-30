//
// Implementation of CmdLnData.h
//
#include "CmdLnData.h"
#include "common.h"

CmdLnData::CmdLnData()
{
}

CmdLnData::~CmdLnData()
{
}

void CmdLnData::SetDefaults()
{
   viewerType_ = DEFAULTVIEWERTYPE;
   segments_ = DEFAULTSEGMENTS;
   duration_ = DEFAULTDURATION;
   durationBound_ = DEFAULTDURATIONBOUND;
   characterAppearanceRate_ = DEFAULTCHARACTERAPPEARANCERATE;
   minCosts_.resize(DEFAULTCOSTSIZE);
   if (DEFAULTCOSTSIZE > 0)
   {
      for (int i = 0; i < DEFAULTCOSTSIZE; i++)
      {
         minCosts_[i] = DEFAULTCOST;
      }
   }
   timeOut_ = DEFAULTTIMEOUT;
   maxVariance_ = DEFAULTMAXVARAINCE;
   printLibrary_ = DEFAULTPRINTLIBRARY;
   wait_ = DEFAULTWAIT;
   hierarchSolutions_ = DEFAULTHIERARCHSOLUTIONS;
   segmentSolutions_ = DEFAULTSEGMENTSOLUTIONS;
   variantSolutions_ = DEFAULTVARIANTSOLUTIONS;
   libraryMultiplier_ = DEFAULTLIBRARYMULTIPLIER;
}
