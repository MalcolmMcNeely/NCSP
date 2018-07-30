////
//// Specialized Gecode Script for a Narrative Model
//// within the Gecode Framework
////
//#pragma once
//
//#include "common.h"
//#include "NarrativeOptions.h"
//
//class Shot;
//
//class NarrativeScript : public Gecode::IntMaximizeScript
//{
//public:
//   NarrativeScript(const NarrativeOptions& options, NarrativeScript* other = nullptr);
//	NarrativeScript(bool share, NarrativeScript& s);
//	~NarrativeScript();
//
//   virtual Space* copy(bool share) override;
//   virtual Gecode::IntVar cost(void) const override;
//   virtual void print(std::ostream& os) const override;
//   void ReturnOutput(std::ostream& os, std::vector<SHOTCONSTRAINTOUTPUT> output) const;
//
//   std::vector<int> GetPreviousSolutionShotIDs() { return previousSolutionShotIDs_; }
//
//   void More();
//
//protected:
//
//   //int ShotRelevance(Shot* shot, int shotNum, eViewerType vt, int shotIDRow);
//   //void CalculateCost();
//
//   Gecode::IntVarArray variables;
//   Gecode::IntVar cost_;
//   Gecode::IntVar duration_;
//
//   Gecode::BoolVarArray matches_;
//   Gecode::IntVar levenshteinDistance_;
//   Gecode::IntVar hammingDist_;
//   Gecode::IntVar jaccardDist_;
//   Gecode::SetVar prevSolSet_;
//   Gecode::SetVar currSolSet_;
//
//   CmdLnOptions* cmdOpt_;
//
//   ShotLibrary* library_;
//   NarrativeInterface* nInterface_;
//   eViewerType viewerType_;
//
//   int rows_;
//   int cols_;
//
//   std::vector<int> previousSolutionShotIDs_;
//
//   bool bType2;
//};
//
