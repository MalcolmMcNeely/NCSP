////
//// First variant of the NarrativeInterface interface
////
//#pragma once
//
//#include "NarrativeInterface.h"
//#include <vector>
//
//class NarrativeF1 : public NarrativeInterface
//{
//public:
//
//   NarrativeF1();
//   ~NarrativeF1();
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
//   static void Post(Gecode::Space& space);
//};
//
