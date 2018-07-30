//
// Configuration settings for the GManager
//
#pragma once
#include "NarrativeEnums.h"
#include "CmdLnData.h"

class GManagerOptions
{
public:
   GManagerOptions(CmdLnData& data);
   ~GManagerOptions();

   eViewerType GetViewerType() { return vType_; }
   int GetNumSegments() { return numSegments_; }
   int GetDuration() { return duration_; }
   int GetDurationBound() { return durationBound_; }
   int GetCharacterAppearanceRate() { return characterAppearanceRate_; }
   std::vector<int> GetMinCosts() { return minCosts_; }
   int GetTimeout() { return timeout_; }
   int GetMaxVariance() { return maxVariance_; }
   int GetNumHierarchSolutions() { return numHierarchSolutons_; }
   int GetNumSegmentSolutions() { return numSegmentSolutions_; }
   int GetNumVariantSolutions() { return numVariantSolutions_; }

private:

   eViewerType vType_;
   int numSegments_;
   int duration_;
   int durationBound_;
   int characterAppearanceRate_;
   std::vector<int> minCosts_;
   int timeout_;
   int maxVariance_;
   int numHierarchSolutons_;
   int numSegmentSolutions_;
   int numVariantSolutions_;
};

