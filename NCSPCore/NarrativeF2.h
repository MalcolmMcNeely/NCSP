////
//// Variant of the NarrativeInterface interface handles Levenshtein Distance to find different solutions
////
//#pragma once
//
//#include "NarrativeInterface.h"
//#include <vector>
//
//class NarrativeF2 : public NarrativeInterface
//{
//public:
//
//   NarrativeF2();
//   ~NarrativeF2();
//
//   void ApplyConstraints(Gecode::Space* pSpace,
//                         ShotLibrary* pShotLib,
//                         Gecode::IntVarArray& vars,
//                         Gecode::IntVar& cost,
//                         Gecode::IntVar& duration,
//                         GLOBALSETTINGS* globalSettings = nullptr,
//                         std::vector<SHOTCONSTRAINTINPUT>* pShotInput = nullptr) override;
//
//   void ApplyConstraints(Gecode::Space* pSpace,
//                         ShotLibrary* pShotLib,
//                         Gecode::IntVarArray& vars,
//                         Gecode::IntVar& cost,
//                         Gecode::BoolVarArray& bv,
//                         Gecode::IntVar& ld,
//                         Gecode::IntVar& hammingDist,
//                         Gecode::IntVar& jaccardDist,
//                         Gecode::SetVar& prevSolSet,
//                         Gecode::SetVar& currSolSet,
//                         GLOBALSETTINGS* globalSettings = nullptr,
//                         std::vector<SHOTCONSTRAINTINPUT>* pShotInput = nullptr) override;
//
//   void SetLevenshteinDistance(int ld) override;
//
//private:
//
//   bool bLevenshteinDistanceSet_;
//   int levenshteinDistance_;
//   std::vector<int> previousSolutionShotIds_;
//};
//
