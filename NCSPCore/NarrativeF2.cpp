//#include "NarrativeF2.h"
//#include "Shot.h"
//#include "ShotLibrary.h"
//#include "Character.h"
//#include "Utility.h"
//#include "SharedDefs.h"
//
//#include <algorithm>
//#include <gecode\int\arithmetic.hh>
//
//using namespace Gecode;
//using namespace Utility;
//using namespace NarrativeEnums;
//using namespace std;
//
//NarrativeF2::NarrativeF2() {}
//NarrativeF2::~NarrativeF2() {}
//
//void NarrativeF2::ApplyConstraints(Gecode::Space* pSpace,
//                      ShotLibrary* pShotLib,
//                      Gecode::IntVarArray& vars,
//                      Gecode::IntVar& cost,
//                      Gecode::IntVar& duration,
//                      GLOBALSETTINGS* globalSettings,
//                      std::vector<SHOTCONSTRAINTINPUT>* pShotInput)
//{
//
//}
//
//void NarrativeF2::ApplyConstraints(Space* pSpace,
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
//   Matrix<IntVarArray> mat(vars, vCols_, vRows_);
//
//   // Distinct
//   distinct(*pSpace, mat.row(0));
//   
//   // Temporality
//   //vector<Shot> sortedShots;
//   //for (auto s : pShotLib->GetShots())
//   //{
//   //   sortedShots.push_back(s);
//   //}
//
//   //sort(sortedShots.begin(), sortedShots.end(), [&](Shot a, Shot b){ return a.GetTimeIndex() < b.GetTimeIndex(); });
//
//   //vector<int> sortedShotIds;
//   //for (auto s : sortedShots)
//   //{
//   //   sortedShotIds.push_back(s.GetId());
//   //}
//   //IntArgs ssidargs = IntArgs(sortedShotIds);
//   //precede(*pSpace, mat.row(0), ssidargs);
//
//   for (int i = 1; i < vCols_; i++)
//   {
//      rel(*pSpace, mat(i, rowKey_[eP_SHOTID]) > mat(i - 1, rowKey_[eP_SHOTID]));
//   }
//
//   /*
//   rel(*pSpace, mat(0, rowKey_[eP_STRUCTURE]), IRT_EQ, 1);
//   rel(*pSpace, mat(0, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(0, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_OUT_HOUSE2);
//
//   rel(*pSpace, mat(1, rowKey_[eP_EMPATHY]), IRT_EQ, 1);
//   rel(*pSpace, mat(1, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(1, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE2);
//
//   rel(*pSpace, mat(2, rowKey_[eP_THREATEN]), IRT_EQ, 1);
//   rel(*pSpace, mat(2, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(2, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_GROUND_HOUSE2);
//
//   rel(*pSpace, mat(3, rowKey_[eP_ATTACK]), IRT_EQ, 1);
//   rel(*pSpace, mat(3, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(3, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_GROUND_HOUSE2);
//
//   rel(*pSpace, mat(4, rowKey_[eP_CHASE]), IRT_EQ, 1);
//   rel(*pSpace, mat(4, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(4, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_GROUND_HOUSE2);
//
//   rel(*pSpace, mat(5, rowKey_[eP_THREATEN]), IRT_EQ, 1);
//   rel(*pSpace, mat(5, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(5, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//
//   rel(*pSpace, mat(6, rowKey_[eP_THREATEN]), IRT_EQ, 1);
//   rel(*pSpace, mat(6, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(6, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//
//   rel(*pSpace, mat(7, rowKey_[eP_ATTACK]), IRT_EQ, 1);
//   rel(*pSpace, mat(7, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(7, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//
//   rel(*pSpace, mat(8, rowKey_[eP_ATTACK]), IRT_EQ, 1);
//   rel(*pSpace, mat(8, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(8, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//
//   rel(*pSpace, mat(9, rowKey_[eP_ATTACK]), IRT_EQ, 1);
//   rel(*pSpace, mat(9, GetCharacterRow(GetCharFromString("Billy"))), IRT_EQ, true);
//   rel(*pSpace, mat(9, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//
//   rel(*pSpace, mat(10, rowKey_[eP_KILL]), IRT_EQ, 1);
//   rel(*pSpace, mat(10, rowKey_[eP_STRUCTURE]), IRT_EQ, 2);
//   rel(*pSpace, mat(10, GetCharacterRow(GetCharFromString("Sidney"))), IRT_EQ, true);
//   rel(*pSpace, mat(10, rowKey_[eP_LOCATION1]), IRT_EQ, (int)eL_IN_HOUSE3);
//   /**/
//
//   rel(*pSpace, mat(0, rowKey_[eP_SHOTID]) == 1332);
//   rel(*pSpace, mat(1, rowKey_[eP_SHOTID]) == 2225);
//   rel(*pSpace, mat(4, rowKey_[eP_SHOTID]) == 3127);
//   // Up to certain event
//   rel(*pSpace, mat(9, rowKey_[eP_SHOTID]), IRT_EQ, 12412); // Billy dies (Shot 73)
//   rel(*pSpace, mat(10, rowKey_[eP_SHOTID]) == 14232); // Billy dies (Shot 72)
//   //// Focus on character
//   count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Sidney"))), true, IRT_GR, 5);
//   count(*pSpace, mat.row(GetCharacterRow(GetCharFromString("Billy"))), true, IRT_GR, 5);
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
//         mat(i, rowKey_[eP_GORE]));
//
//      rel(*pSpace, thrill[i] ==
//         (1 + (mat(i, rowKey_[eP_EMPATHY]) + mat(i, rowKey_[eP_ESCAPE]) +
//         mat(i, rowKey_[eP_HIDE]) + mat(i, rowKey_[eP_THREATEN])
//         + mat(i, rowKey_[eP_CHASE]))) *
//         mat(i, rowKey_[eP_THRILL]));
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
//#pragma endregion
//
//#pragma region Hamming/Unique Distance
//   
//   // Initialise variables
//   std::vector<int> previousSolutions;
//   for (auto s : *pShotInput)
//   {
//      previousSolutions.push_back(s.Id);
//   }
//   IntSet previousSolutonDomainSet(previousSolutions.data(), previousSolutions.size());   
//   auto fullDomain = pShotLib->GetShotIds();
//   IntSet fullDomainSet(fullDomain.data(), fullDomain.size());
//
//   //// Hamming Distance
//   //IntArgs shotIDArgs(previousSolutions);
//   //BoolVarArgs bvh(*pSpace, vCols_, 0, 1);
//   //// Register all matches
//   //for (int i = 0; i < vCols_; i++)
//   //{
//   //   rel(*pSpace, mat(i, 0), IRT_EQ, shotIDArgs[i], bvh[i]);
//   //}
//   //// Find the differences
//   //rel(*pSpace, vCols_ - sum(bvh) == hammingDist);
//
//   // Unique difference
//   IntVarArgs inv(*pSpace, vCols_, fullDomainSet);
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, inv[i], IRT_EQ, mat(i, 0));
//   }
//   SetVar sv(*pSpace, IntSet::empty, fullDomainSet);
//   channel(*pSpace, inv, sv);
//
//   IntVarArgs inv2(*pSpace, vCols_, previousSolutonDomainSet);
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, inv2[i], IRT_EQ, previousSolutions[i]);
//   }
//   SetVar sv2(*pSpace, IntSet::empty, fullDomainSet);
//   channel(*pSpace, inv2, sv2);
//
//   SetVar diff(*pSpace, IntSet::empty, fullDomainSet);
//   rel(*pSpace, diff == sv - sv2);
//
//   IntVar diffv(*pSpace, 0, vCols_);
//   cardinality(*pSpace, diff, diffv);
//
//   // End
//   rel(*pSpace, levenshteinDistance_ == ld);
//   rel(*pSpace, ld == diffv);
//   
//   // Both implementations below are incomplete
//   /*
//   // Jaccard Distance   
//   SetVarArgs ox(vCols_);
//   for (int i = 0; i < vCols_; i++)
//   {
//      ox[i] = singleton(previousSolutions[i]);
//      rel(*pSpace, ox[i], IRT_EQ, previousSolutions[i]);
//   }   
//   
//   SetVarArgs oy(vCols_);
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, oy[i] == (mat(i, 0)));
//   }
//
//   IntArgs px(vCols_);
//   rel(*pSpace, ox, IRT_EQ, px);
//
//   IntArgs cy(vCols_);
//
//   channel(*pSpace, ox, prevSolSet);
//   channel(*pSpace, oy, currSolSet);
//   
//   //jaccard = (|px \inter cy| - |px \union cy|) * w
//   int w = 2;
//   rel(*pSpace, jaccardDist == ((cardinality(prevSolSet & currSolSet) -
//      cardinality(prevSolSet | currSolSet)) * w));
//   */
//   /*
//   // Jaccard Distance   
//   IntVarArgs ox(*pSpace, vCols_, previousSolutonDomainSet);
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, ox[i] == previousSolutions[i]);
//   }
//
//   IntVarArgs oy(*pSpace, vCols_, fullDomainSet);
//   for (int i = 0; i < vCols_; i++)
//   {
//      rel(*pSpace, oy[i] == (mat(i, 0)));
//   }
//   channel(*pSpace, ox, prevSolSet);
//   channel(*pSpace, oy, currSolSet);
//
//   //jaccard = (|px \inter cy| - |px \union cy|) * w
//   int w = 2;
//   rel(*pSpace, jaccardDist == ((cardinality(prevSolSet & currSolSet) -
//      cardinality(prevSolSet | currSolSet)) * w));
//      */
//
//   // Hamming-Jaccard End
//   //rel(*pSpace, ld == hammingDist + jaccardDist);
//   //rel(*pSpace, levenshteinDistance_ == ld);
//   
//#pragma endregion
//
//#pragma region Branching
//
//   Rnd r;
//   r.time();
//   Gecode::branch(*pSpace, cost, INT_VAL_MAX());
//   Gecode::branch(*pSpace, mat.row(0), INT_VAR_NONE(), INT_VAL_RND(r));
//
//#pragma endregion
//}
//
//void NarrativeF2::SetLevenshteinDistance(int ld)
//{
//   levenshteinDistance_ = ld;
//   bLevenshteinDistanceSet_ = true;
//}
