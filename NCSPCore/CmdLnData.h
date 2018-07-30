//
// Simple class to hold any command line options specified
//
#pragma once
#include <vector>

class CmdLnData
{
public:
   CmdLnData();
   ~CmdLnData();

   void SetDefaults();

   void SetViewerType(int n) { viewerType_ = n; }
   void SetSegments(int n) { segments_ = n; }
   void SetDuration(int n) { duration_ = n; }
   void SetDurationBound(int n) { durationBound_ = n; }
   void SetCharacterAppearanceRate(int n) { characterAppearanceRate_ = n; }
   void SetMinCosts(std::vector<int> v) { minCosts_ = v; }
   void SetTimeout(int n) { timeOut_ = n; }
   void SetMaxVariance(int n) { maxVariance_ = n; }
   void SetPrintLibrary(bool b) { printLibrary_ = b; }
   void SetWait(bool b) { wait_ = b; }
   void SetHierarchSolutions(int i) { hierarchSolutions_ = i; }
   void SetSegmentSolutions(int i) { segmentSolutions_ = i; }
   void SetVariantSolutions(int i) { variantSolutions_ = i; }
   void SetLibraryMultiplier(int i) { libraryMultiplier_ = i; }

   int GetViewerType() { return viewerType_; }
   int GetSegments() { return segments_; }
   int GetDuration() { return duration_; }
   int GetDurationBound() { return durationBound_; }
   int GetCharacterAppearanceRate() { return characterAppearanceRate_; }
   std::vector<int> GetMinCosts() { return minCosts_; }
   int GetTimeout() { return timeOut_; }
   int GetMaxVariance() { return maxVariance_; }
   bool GetPrintLibrary() { return printLibrary_; }
   bool GetWait() { return wait_; }
   int GetHierarchSolutions() { return hierarchSolutions_; }
   int GetSegmentSolutions() { return segmentSolutions_; }
   int GetVariantSolutions() { return variantSolutions_; }
   int GetLibraryMultiplier() { return libraryMultiplier_; }

private:

   int viewerType_;
   int segments_;
   int duration_;
   int durationBound_;
   int characterAppearanceRate_;
   std::vector<int> minCosts_;
   int timeOut_;
   int maxVariance_;
   bool printLibrary_;
   bool wait_;
   int hierarchSolutions_;
   int segmentSolutions_;
   int variantSolutions_;
   int libraryMultiplier_;
};

