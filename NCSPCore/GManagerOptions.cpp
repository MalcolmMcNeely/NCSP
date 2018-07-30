//
// Implementation of GManagerOptions.h
//
#include "GManagerOptions.h"


GManagerOptions::GManagerOptions(CmdLnData& data)
{
   vType_ = (eViewerType)data.GetViewerType();
   numSegments_ = data.GetSegments();
   duration_ = data.GetDuration();
   durationBound_ = data.GetDurationBound();
   characterAppearanceRate_ = data.GetCharacterAppearanceRate();
   minCosts_ = data.GetMinCosts();
   timeout_ = data.GetTimeout();
   maxVariance_ = data.GetMaxVariance();
   numHierarchSolutons_ = data.GetHierarchSolutions();
   numSegmentSolutions_ = data.GetSegmentSolutions();
   numVariantSolutions_ = data.GetVariantSolutions();
}

GManagerOptions::~GManagerOptions()
{
}
