////
//// Implementation of NarrativeInterface.h
////
//#include "NarrativeInterface.h"
//#include "Shot.h"
//#include "ShotLibrary.h"
//#include "Character.h"
//#include "Utility.h"
//#include "SharedDefs.h"
////#include "NarrativeSpace.h"
//
//#include <iomanip>
//
//using namespace Gecode;
//using namespace std;
//using namespace NarrativeEnums;
//
//NarrativeInterface::NarrativeInterface() : pShotOutput_(nullptr), pShotLib_(nullptr), pSpace_(nullptr) {}
//NarrativeInterface::~NarrativeInterface() {}
//
//bool NarrativeInterface::Initialise(Space* pSpace,
//                                    ShotLibrary* pShotLib,
//                                    IntVarArray& vars,
//                                    int requestedNumShots)
//{
//   if (pSpace == NULL || pShotLib == NULL || !requestedNumShots || !pShotLib->GetIsFinalized())
//   {
//      return false;
//   }
//
//   // Initialise basic member variables 
//   vCols_ = requestedNumShots;
//   vRows_ = (pShotLib->GetMaxNumShotVariables()); // Zero-index
//   pShotLib_ = pShotLib;
//
//   pSpace_ = pSpace;
//
//   // Initialise Gecode Varaibles
//   int varCount = vCols_ * vRows_;
//   vars = IntVarArray(*pSpace, varCount);
//
//   ApplyDomains(pSpace, pShotLib, &vars);
//   DefineValidShotData(pSpace, pShotLib, &vars);
//
//   return true;
//}
//
//bool NarrativeInterface::Initialise(Space* pSpace, 
//                                    ShotLibrary* pShotLib, 
//                                    IntVarArray& vars, 
//                                    int requestedNumShots,
//                                    BoolVarArray& bv,
//                                    IntVar& ld,
//                                    IntVar& hammingDist,
//                                    IntVar& jaccardDist,
//                                    SetVar& prevSolSet,
//                                    IntSet& prevSolSetDomain,
//                                    SetVar& currSolSet,
//                                    IntSet& currSolSetDomain)
//{
//	if (pSpace == NULL || pShotLib == NULL || !requestedNumShots || !pShotLib->GetIsFinalized())
//   {
//      return false;
//   }
//
//   // Initialise basic member variables 
//   vCols_ = requestedNumShots; 
//	vRows_ = (pShotLib->GetMaxNumShotVariables()); // Zero-index 
//	pShotLib_ = pShotLib; 
//
//   pSpace_ = pSpace;
//
//   // Initialise Gecode Varaibles
//   int varCount = vCols_ * vRows_;
//	vars = IntVarArray(*pSpace, varCount);
//   
//   bv = BoolVarArray(*pSpace, vCols_);
//   ld = IntVar(*pSpace, 0, Gecode::Int::Limits::max);
//   hammingDist = IntVar(*pSpace, 0, Gecode::Int::Limits::max);
//   jaccardDist = IntVar(*pSpace, 0, Gecode::Int::Limits::max);
//   prevSolSet = SetVar(*pSpace, IntSet::empty, prevSolSetDomain);
//   currSolSet = SetVar(*pSpace, IntSet::empty, currSolSetDomain);
//
//	ApplyDomains(pSpace, pShotLib, &vars);
//	DefineValidShotData(pSpace, pShotLib, &vars);
//
//   return true;
//}
//
//void NarrativeInterface::ReturnOutput(std::ostream& os, const IntVarArray& vars, eViewerType vt, int cost)
//{
//   Matrix<IntVarArray> mat(vars, vCols_, vRows_);
//	vector<SHOTCONSTRAINTOUTPUT>* output = GetShotOutput();
//	ShotLibrary* lib = GetShotLibrary();
//   size_t fieldwidth = 3;
//
//   os << "Printing solution (Size: " << vCols_ << ")" << endl;
//   os << "Shot IDs:\t";
//   for (int i = 0; i < vCols_; i++)
//   {
//      //os << setw(fieldwidth) << (mat(i, rowKey_[eP_SHOTID]).val());
//      os << "\t" << (mat(i, rowKey_[eP_SHOTID]).val());
//
//      SHOTCONSTRAINTOUTPUT o;
//      o.Id = (mat(i, rowKey_[eP_SHOTID]).val());
//      pShotOutput_->push_back(o);
//   }
//
//   os << endl << "Shot Cost:\t";
//   for (int i = 0; i < vCols_; i++)
//   {
//      Shot shot = lib->GetShotById((mat(i, rowKey_[eP_SHOTID]).val()));
//
//      int cost = 0;
//      int gore = shot.GetGore() * (1 + shot.GetAttack() + shot.GetKill());
//      int thrill = shot.GetThrill() * (1 + shot.GetEmpathy() + shot.GetEscape() +
//         shot.GetHide() + shot.GetThreaten());
//      cost = vt == eV_GORE ? gore - thrill : thrill - gore;
//      
//      os << "\t" << cost;
//
//      (*pShotOutput_)[i].Cost = cost;
//   }
//
//   os << endl;
//   os << "Total Cost: " << setw(fieldwidth) << cost << endl;
//}
//
//void NarrativeInterface::ReturnOutput(std::ostream& os, vector<SHOTCONSTRAINTOUTPUT> output)
//{
//   size_t fieldwidth = 3;
//   int totalCost = 0;
//
//   os << "Printing solution (Size: " << output.size() << ")" << endl;
//   os << "Shot IDs:\t";
//   for (int i = 0; i < vCols_; i++)
//   {
//      //os << setw(fieldwidth) << output[i].Id;
//      os << "\t" << output[i].Id;
//   }
//
//   os << endl << "Shot Cost:\t";
//   for (int i = 0; i < vCols_; i++)
//   {
//      os << "\t" << output[i].Cost;
//      totalCost += output[i].Cost;
//   }
//
//   os << endl;
//   os << "Total Cost: " << setw(fieldwidth) << totalCost << endl;
//}
//
//void NarrativeInterface::SetShotOutput(vector<SHOTCONSTRAINTOUTPUT>* pOutput)
//{
//	pShotOutput_ = pOutput;
//}
//
//int NarrativeInterface::GetCharacterRow(eChar ch)
//{
//   int out = rowKey_[eP_CHARACTERSTARTROW];
//   out += ((int)ch * Character::NumMembers());
//   return out;
//}
//
//int NarrativeInterface::GetCharacterMoodRow(eChar ch)
//{
//   return GetCharacterRow(ch) + 1;
//}
//
//vector<SHOTCONSTRAINTOUTPUT>* NarrativeInterface::GetShotOutput()
//{
//	return pShotOutput_;
//}
//
//ShotLibrary* NarrativeInterface::GetShotLibrary()
//{
//	return pShotLib_;
//}
//
//void NarrativeInterface::ApplyDomainToRowAndIncrement(int& currentRow, Space* pSpace, Matrix<IntVarArray>* pMatrix, IntSet* pDomain)
//{
//	for (int i = 0; i < pMatrix->width(); i++)
//   {
//		(*pMatrix)(i, currentRow) = IntVar(*pSpace, *pDomain);;
//   }
//
//   currentRow++;
//}
//
//void NarrativeInterface::ApplyDomains(Gecode::Space* pSpace, ShotLibrary* pShotLib, Gecode::IntVarArray* pVars)
//{
//   // Superimpose matrix interface on gecode variables
//	Matrix<IntVarArray> mat(*pVars, vCols_, vRows_);
//	int numShots = pShotLib->GetShots().size();
//   int currentRow = 0;
//
//   // Shot ID == Shot Time now
//   rowKey_[eP_SHOTID] = currentRow;
//   {
//      auto domain = pShotLib->GetShotIds();
//      IntSet domainSet(domain.data(), domain.size());
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_STRUCTURE] = currentRow;
//   {
//      IntSet domainSet(0, 2);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_GORE] = currentRow;
//   {
//		auto domain = pShotLib->GetUniqueGoreValues();
//      IntSet domainSet(domain.data(), domain.size());
//		ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_THRILL] = currentRow;
//   {
//      auto domain = pShotLib->GetUniqueThrillValues();
//      IntSet domainSet(domain.data(), domain.size());
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_EMPATHY] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//		ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_THREATEN] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_CHASE] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_HIDE] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_KILL] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_ATTACK] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_ESCAPE] = currentRow;
//   {
//      IntSet domainSet(0, 1);
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_LOCATION] = currentRow;
//   {
//      auto domain = pShotLib->GetUniqueLocationValues();
//      IntSet domainSet(domain.data(), domain.size());
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//
//   rowKey_[eP_TIMEDURATION] = currentRow;
//   {
//      auto domain = pShotLib->GetUniqueTimeDurations();
//      IntSet domainSet(domain.data(), domain.size());
//      ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//   }
//   
//   // For unique each character
//   rowKey_[eP_CHARACTERSTARTROW] = currentRow;
//   for (int i = 0; i < (int)eC_SIZE; i++)
//   {
//      // row character is present (true/false)
//      {
//         IntSet domainSet(0, 1);
//			ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//      }
//      // Alive
//      {
//         IntSet domainSet(0, 1);
//         ApplyDomainToRowAndIncrement(currentRow, pSpace, &mat, &domainSet);
//      }
//   }
//
//   assert(currentRow == vRows_);
//}
//
//void NarrativeInterface::DefineValidShotData(Space* pSpace, ShotLibrary* pShotLib, IntVarArray* pVars)
//{
//   // Superimpose matrix interface on gecode variables
//	Matrix<IntVarArray> mat(*pVars, vCols_, vRows_);
//   int tArgSize = vRows_;
//   
//	vector<Shot> shots = pShotLib->GetShots();
//   vector<vector<int>> vArgs(shots.size());
//   for (int i = 0; i < shots.size(); i++)
//   {
//      vArgs[i].resize(vRows_);
//   }
//
//
//   for (unsigned int i = 0; i < shots.size(); i++)
//   {
//      vArgs[i][rowKey_[eP_SHOTID]] = shots[i].GetId();
//      vArgs[i][rowKey_[eP_STRUCTURE]] = shots[i].GetStructure();
//      vArgs[i][rowKey_[eP_GORE]] = shots[i].GetGore();
//      vArgs[i][rowKey_[eP_THRILL]] = shots[i].GetThrill();
//      vArgs[i][rowKey_[eP_EMPATHY]] = shots[i].GetEmpathy();
//      vArgs[i][rowKey_[eP_THREATEN]] = shots[i].GetThreaten();
//      vArgs[i][rowKey_[eP_CHASE]] = shots[i].GetChase();
//      vArgs[i][rowKey_[eP_HIDE]] = shots[i].GetHide();
//      vArgs[i][rowKey_[eP_KILL]] = shots[i].GetKill();
//      vArgs[i][rowKey_[eP_ATTACK]] = shots[i].GetAttack();
//      vArgs[i][rowKey_[eP_ESCAPE]] = shots[i].GetEscape();
//      vArgs[i][rowKey_[eP_LOCATION]] = shots[i].GetLocation();
//      vArgs[i][rowKey_[eP_TIMEDURATION]] = shots[i].GetTimeDuration();
//
//      // For each character in shot
//      auto charList = shots[i].GetCharacterList();
//      for (int j = 0; j < eC_SIZE; j++)
//      {
//         int currentCharacterRow = rowKey_[eP_CHARACTERSTARTROW] + (j * Character::NumMembers());
//
//         // If character is in the shot
//         if (Utility::VectorContains(charList, (eChar)j))
//         {
//            auto currentChar = shots[i].GetCharacter((eChar)j);
//            vArgs[i][currentCharacterRow + (int)eCP_NAME] = true;
//            vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = currentChar.GetAlive();
//         }
//         else
//         {
//            vArgs[i][currentCharacterRow + (int)eCP_NAME] = false;
//            vArgs[i][currentCharacterRow + (int)eCP_ALIVE] = 0;
//         }
//      }  
//   }
//
//   // Add and finalize data
//   TupleSet validData;
//   for (int i = 0; i < shots.size(); i++)
//   {
//      IntArgs args(vArgs[i]);
//      validData.add(args);
//   }
//   validData.finalize();
//
//   // Constrain columns to be valid shot data
//   for (int i = 0; i < vCols_; i++)
//   {
//      extensional(*pSpace, mat.col(i), validData);
//   }
//}