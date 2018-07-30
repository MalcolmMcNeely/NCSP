////
//// Implementation of NarrativeF1.h
////
//#include "NarrativeF1.h"
//#include "Shot.h"
//#include "Character.h"
//#include "ShotLibrary.h"
//#include "Utility.h"
//#include "SharedDefs.h"
//
//#include "NarrativeScript.h"
//
//#include <algorithm>
//#include <gecode\int\arithmetic.hh>
//
//using namespace Gecode;
//using namespace Utility;
//using namespace NarrativeEnums;
//using namespace std;
//
//NarrativeF1::NarrativeF1() {}
//NarrativeF1::~NarrativeF1() {}
//
//void NarrativeF1::ApplyConstraints(Space* pSpace,
//											  ShotLibrary* pShotLib,
//                                   IntVarArray& vars,
//                                   IntVar& cost,
//                                   IntVar& duration,
//                                   GLOBALSETTINGS* globalSettings,
//                                   vector<SHOTCONSTRAINTINPUT>* pShotInput)
//{
//   Matrix<IntVarArray> mat(vars, vCols_, vRows_);
//
//   //SHOTCONSTRAINTINPUT c[] = { 40, 43, 44, 46, 48, 76, 75, 67, 70, 73, 72 };
//   //rel(*pSpace, mat(0, rowKey_[eP_SHOTID]) == 1332);
//   //rel(*pSpace, mat(1, rowKey_[eP_SHOTID]) == 2225);
//   //rel(*pSpace, mat(4, rowKey_[eP_SHOTID]) == 3127);
//   // Up to certain event
//   //rel(*pSpace, mat(9, rowKey_[eP_SHOTID]) == 12412); // Billy dies (Shot 73)
//   //rel(*pSpace, mat(10, rowKey_[eP_SHOTID]) == 14232); // Billy dies (Shot 72)
//   // Focus on character(s)
//   //count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Sidney"))), true, IRT_GR, 8);
//   //count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Billy"))), true, IRT_GR, 6);
//   
//
//#pragma region Cost Function
//
//   // Cost
//   IntVarArgs gore(*pSpace, vCols_, 0, 100);
//   IntVarArgs thrill(*pSpace, vCols_, 0, 100);
//      
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, gore[i] == (1 + (mat(i, rowKey_[eP_ATTACK]) + mat(i, rowKey_[eP_KILL]))) *  
//         mat(i, rowKey_[eP_GORE]) );
//
//      rel(*pSpace, thrill[i] == 
//            ( 1 + (mat(i, rowKey_[eP_EMPATHY]) + mat(i, rowKey_[eP_ESCAPE]) + 
//               mat(i, rowKey_[eP_HIDE]) + mat(i, rowKey_[eP_THREATEN])
//               + mat(i, rowKey_[eP_CHASE]))) *
//            mat(i, rowKey_[eP_THRILL]));
//   }
//
//   if (globalSettings->ViewerType == eV_GORE)
//   {
//      rel(*pSpace, cost == sum(gore) - sum(thrill));
//   }
//   else // Thrill
//   {
//      rel(*pSpace, cost == sum(thrill) - sum(gore));
//   }
//
//   // COST THRESHOLD
//   rel(*pSpace, cost > 20);
//   //rel(*pSpace, cost < 20);
//
//#pragma endregion
//
//#pragma region Duration
//   
//   int dur = 200;
//   int mind = 60;
//   int maxd = 80;
//
//   {
//      auto dDomain = pShotLib->GetUniqueTimeDurations();
//      IntSet dDomainSet(dDomain.data(), dDomain.size());
//      IntVarArgs d(*pSpace, vCols_, dDomainSet);
//
//      for (int i = 0; i < vCols_; i++)
//      {
//         rel(*pSpace, d[i] == mat(i, rowKey_[eP_TIMEDURATION]));
//      }
//      rel(*pSpace, duration == sum(d));
//
//      rel(*pSpace, duration > mind);
//      rel(*pSpace, duration < maxd);   
//   }
//
//#pragma endregion
//
//#pragma region Conditional Distinct
//
//   {
//      // Grab shots with a relevant shots (w/ duration > 0)
//      auto shotIDs = pShotLib->GetShotIds();   
//      int indexSizeWOZero = 0;
//
//      for (int i = 0; i < shotIDs.size(); i++)
//      {
//         if (shotIDs[i] != NOSHOTID)
//            indexSizeWOZero++;
//      }
//      IntArgs args(indexSizeWOZero);
//      int argSize = 0;
//      for (int i = 0; i < shotIDs.size(); i++)
//      {
//         if (shotIDs[i] != NOSHOTID)
//         {
//            args[argSize] = shotIDs[i];
//            argSize++;
//         }
//      }
//
//      // Relevant shots can only appear 0 or 1 times
//      for (int i = 0; i < argSize; i++)
//      {
//         count(*pSpace, mat.row(0), args[i], IRT_LQ, 1);
//      }
//   }
//
//#pragma endregion
//
//#pragma region Redundant Constraints
//
//   {
//      // Find min shots needed to fufill duration equirements
//      auto durations = pShotLib->GetRealTimeDurations();
//      int minShotsNeeded = 0;
//      int currentTime = 0;
//
//      sort(durations.begin(), durations.end(), [](int a, int b){ return b < a; });
//      for (int i = 0; i < durations.size(); i++)
//      {
//         currentTime += durations[i];
//         minShotsNeeded++;
//
//         if (currentTime > mind)
//            break;
//      }
//      minShotsNeeded = std::min(minShotsNeeded, vCols_);
//
//      // Both duration and ID of these shots cannot be empty
//      for (int i = 0; i < minShotsNeeded; i++)
//      {
//         rel(*pSpace, mat(i, rowKey_[eP_SHOTID]), IRT_NQ, NOSHOTID);
//         rel(*pSpace, mat(i, rowKey_[eP_TIMEDURATION]), IRT_NQ, 0);
//      }
//   }
//
//#pragma endregion
//
//#pragma region Ordering
//
//   {
//      for (int i = 1; i < vCols_; i++)
//      {
//         rel(*pSpace, mat(i, rowKey_[eP_SHOTID]), IRT_GQ, mat(i - 1, rowKey_[eP_SHOTID]));
//      }
//   }
//
//#pragma endregion
//
//#pragma region Character Selection
//
//   {
//      count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Sidney"))), true, IRT_GR, 5);
//      count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Billy"))), true, IRT_GR, 5);
//
//      //branch(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Sidney"))), INT_VAR_NONE(), INT_VAL_MAX());
//      //branch(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Billy"))), INT_VAR_NONE(), INT_VAL_MAX());
//   }
//
//#pragma endregion
//
//#pragma region Branching
//
//   Rnd r;
//   r.time();
//   //Gecode::branch(*pSpace, cost, INT_VAL_MAX());
//   //Gecode::branch(*pSpace, vars, INT_VAR_NONE(), INT_VAL_RND(r));
//
//   //Gecode::branch(*pSpace, mat.row(0), INT_VAR_NONE(), INT_VAL_RND(r));
//   Gecode::branch(*pSpace, mat.row(0), INT_VAR_NONE(), INT_VAL_MAX());
//   //Gecode::branch(*pSpace, mat.row(0), INT_VAR_NONE(), INT_VAL_MIN());
//
//   //Gecode::branch(*pSpace, &NarrativeF1::Post);
//
//#pragma endregion
//}
//
//void NarrativeF1::ApplyConstraints(Space* pSpace,
//                                   ShotLibrary* pShotLib,
//                                   IntVarArray& vars,
//                                   IntVar& cost,
//                                   BoolVarArray& bv,
//                                   IntVar& ld,
//                                   IntVar& hammingDist,
//                                   IntVar& jaccardDist,
//                                   SetVar& prevSolSet,
//                                   SetVar& currSolSet,
//                                   GLOBALSETTINGS* globalSettings,
//                                   vector<SHOTCONSTRAINTINPUT>* pShotInput)
//{
//
//}
//
//void NarrativeF1::Post(Gecode::Space& space)
//{
//   static_cast<NarrativeScript&>(space).More();
//}