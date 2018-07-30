//#include "NarrativeScript.h"
//#include "ShotLibrary.h"
//#include "NarrativeInterface.h"
//#include "SharedDefs.h"
//#include "Character.h"
//#include "Shot.h"
//
//#include <vector>
//#include <cmath>
//
//using namespace Gecode;
//using namespace std;
//
//NarrativeScript::NarrativeScript(const NarrativeOptions& options, NarrativeScript* other) : cost_(*this, 0, 10000), duration_(*this, 0, 10000), IntMaximizeScript(options)
//{
//   auto settings = options.GetGlobalSettings();
//   viewerType_ = (eViewerType)settings->ViewerType;
//   library_ = options.GetShotLibrary();
//   nInterface_ = options.GetNInterface();
//   cmdOpt_ = options.GetCmdLnOptions();
//   
//   if (cmdOpt_ != nullptr)
//   {
//      nInterface_->SetLevenshteinDistance(cmdOpt_->LevenDist);
//
//      std::vector<int> d;
//      for (auto s : *options.GetShotInput())
//      {
//         d.push_back(s.Id);
//      }
//      IntSet previousSolutonDomainSet(d.data(), options.GetShotInput()->size());
//      auto fullDomain = library_->GetShotIds();
//      IntSet fullDomainSet(fullDomain.data(), fullDomain.size());
//      
//      nInterface_->Initialise(this,
//                              library_,
//                              variables,
//                              options.GetNumShotsRequested(),
//                              matches_,
//                              levenshteinDistance_,
//                              hammingDist_,
//                              jaccardDist_,
//                              prevSolSet_,
//                              previousSolutonDomainSet,
//                              currSolSet_,
//                              fullDomainSet);
//
//      nInterface_->ApplyConstraints(this,
//                                    library_,
//                                    variables,
//                                    cost_,
//                                    matches_,
//                                    levenshteinDistance_,
//                                    hammingDist_,
//                                    jaccardDist_,
//                                    prevSolSet_,
//                                    currSolSet_,
//                                    settings,
//                                    options.GetShotInput());
//
//      bType2 = true;
//   }
//   else
//   {
//      nInterface_->Initialise(this,
//         library_,
//         variables,
//         options.GetNumShotsRequested());
//
//      nInterface_->ApplyConstraints(this,
//         library_,
//         variables,
//         cost_,
//         duration_,
//         settings,
//         options.GetShotInput());
//
//      bType2 = false;
//   }
//
//   cols_ = nInterface_->GetCols();
//   rows_ = nInterface_->GetRows();
//
//
//}
//
//NarrativeScript::NarrativeScript(bool share, NarrativeScript& s) : IntMaximizeScript(share, s)
//{
//   variables.update(*this, share, s.variables);
//   cost_.update(*this, share, s.cost_);
//
//   if (bType2 == true)
//   {
//      matches_.update(*this, share, s.matches_);
//      levenshteinDistance_.update(*this, share, s.levenshteinDistance_);
//      hammingDist_.update(*this, share, s.hammingDist_);
//      jaccardDist_.update(*this, share, s.jaccardDist_);
//      prevSolSet_.update(*this, share, s.prevSolSet_);
//      currSolSet_.update(*this, share, s.currSolSet_);
//   }
//
//   library_ = s.library_;
//   nInterface_ = s.nInterface_;
//   viewerType_ = s.viewerType_;
//   cols_ = s.cols_;
//   rows_ = s.rows_;
//
//   cmdOpt_ = s.cmdOpt_;
//   
//   bType2 = s.bType2;
//
//   //CalculateCost();
//}
//
//NarrativeScript::~NarrativeScript() { }
//
//Space* NarrativeScript::copy(bool share)
//{
//	return new NarrativeScript(share, *this);
//}
//
//Gecode::IntVar NarrativeScript::cost(void) const
//{
//   return cost_;
//}
//
//void NarrativeScript::print(std::ostream& os) const
//{
//   nInterface_->ReturnOutput(os, variables, viewerType_, cost_.val());
//}
//
//void NarrativeScript::ReturnOutput(std::ostream& os, vector<SHOTCONSTRAINTOUTPUT> output) const
//{
//   nInterface_->ReturnOutput(os, output);
//}
//
//void NarrativeScript::More()
//{
//   Matrix<IntVarArray> mat(variables, cols_, rows_);
//}