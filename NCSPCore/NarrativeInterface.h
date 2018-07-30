////
//// Generalized interface to describe a narrative for use with
//// a Gecode CSP solver
////
//#pragma once
//
//#include "common.h"
//#include <map>
//
//class ShotLibrary;
//class BackgroundMusic;
//class NarrativeSpace;
//struct SHOTCONSTRAINTINPUT;
//struct SHOTCONSTRAINTOUTPUT;
//struct GLOBALSETTINGS;
//
//class NarrativeInterface
//{
//public:
//   NarrativeInterface();
//   virtual ~NarrativeInterface();
//
//   bool Initialise(Gecode::Space* pSpace, 
//                   ShotLibrary* pShotLib, 
//                   Gecode::IntVarArray& vars,
//                   int requestedNumShots);
//   bool Initialise(Gecode::Space* pSpace,
//                   ShotLibrary* pShotLib,
//                   Gecode::IntVarArray& vars,
//                   int requestedNumShots,
//                   Gecode::BoolVarArray& bv,
//                   Gecode::IntVar& ld,
//                   Gecode::IntVar& hammingDist,
//                   Gecode::IntVar& jaccardDist,
//                   Gecode::SetVar& prevSolSet,
//                   Gecode::IntSet& prevSolSetDomain,
//                   Gecode::SetVar& currSolSet,
//                   Gecode::IntSet& currSolSetDomain);
//   void ReturnOutput(std::ostream& os, const Gecode::IntVarArray& vars, eViewerType vt, int cost = 0);
//   void ReturnOutput(std::ostream& os, std::vector<SHOTCONSTRAINTOUTPUT> output);
//   virtual void ApplyConstraints(Gecode::Space* pSpace,
//                                 ShotLibrary* pShotLib,
//                                 Gecode::IntVarArray& vars,
//                                 Gecode::IntVar& cost,
//                                 Gecode::IntVar& duration,
//                                 GLOBALSETTINGS* globalSettings = nullptr,
//                                 std::vector<SHOTCONSTRAINTINPUT>* pShotInput = nullptr) = 0;
//   virtual void ApplyConstraints(Gecode::Space* pSpace,
//                                 ShotLibrary* pShotLib,
//                                 Gecode::IntVarArray& vars,
//                                 Gecode::IntVar& cost,
//                                 Gecode::BoolVarArray& bv,
//                                 Gecode::IntVar& ld,
//                                 Gecode::IntVar& hammingDist,
//                                 Gecode::IntVar& jaccardDist,
//                                 Gecode::SetVar& prevSolSet,
//                                 Gecode::SetVar& currSolSet,
//                                 GLOBALSETTINGS* globalSettings = nullptr,
//                                 std::vector<SHOTCONSTRAINTINPUT>* pShotInput = nullptr) = 0;
//	void SetShotOutput(std::vector<SHOTCONSTRAINTOUTPUT>* pOutput);
//
//   int GetRows() const { return vRows_; }
//   int GetCols() const { return vCols_; }
//   std::map<eRowProperties, int> GetRowKeyMap() const { return rowKey_; }
//   int GetCharacterRow(eChar ch);
//   int GetCharacterMoodRow(eChar ch);
//
//   virtual void SetLevenshteinDistance(int ld) {};
//
//protected:
//
//	std::vector<SHOTCONSTRAINTOUTPUT>* GetShotOutput();
//	ShotLibrary* GetShotLibrary();
//   int GetSolutionCost(NarrativeSpace* n);
//
//   std::map<eRowProperties, int> rowKey_;
//   int vRows_, vCols_;
//	std::vector<SHOTCONSTRAINTOUTPUT>* pShotOutput_;
//	ShotLibrary* pShotLib_;
//
//   Gecode::Space* pSpace_;
//
//private:
//
//   void ApplyDomainToRowAndIncrement(int& currentRow, 
//                                     Gecode::Space* pSpace,
//                                     Gecode::Matrix<Gecode::IntVarArray>* pMatrix,
//                                     Gecode::IntSet* pDomain);
//   void ApplyDomains(Gecode::Space* pSpace, 
//                     ShotLibrary* pShotLib, 
//                     Gecode::IntVarArray* pVars);
//   void DefineValidShotData(Gecode::Space* pSpace, 
//                            ShotLibrary* pShotLib, 
//                            Gecode::IntVarArray* pVars);
//};
//
