//#include "NarrativeOptions.h"
//
//#include "ShotLibrary.h"
//#include "NarrativeInterface.h"
//#include "SharedDefs.h"
//
//#include <fstream>
//
//#define OUTFILE "CSPOutfile.txt"
//#define LOGFILE "CSPLogFile.txt"
//
//using namespace std;
//
//NarrativeOptions::NarrativeOptions(const char* pString,
//											  ShotLibrary* pShotLib,
//											  NarrativeInterface* pNInterface,
//											  int numShots) : Options(pString)
//{
//	pShotLibrary_ = pShotLib;
//	pNInterface_ = pNInterface;
//   numShotsRequested_ = numShots;
//	pShotInput_ = nullptr;
//	pShotOutput_ = nullptr;
//	pSettings_ = nullptr;
//	SetPrintFiles();
//}
//
//NarrativeOptions::NarrativeOptions(const char* pString,
//											  ShotLibrary* pShotLib,
//											  NarrativeInterface* pNInterface,
//											  GLOBALSETTINGS* settings,
//                                   vector<SHOTCONSTRAINTOUTPUT>* pShotOutput,
//                                   CmdLnOptions* cmdOpt,
//                                   vector<SHOTCONSTRAINTINPUT>* pShotInput) : Options(pString)
//{
//	pSettings_ = settings;
//	pShotLibrary_ = pShotLib;
//	pNInterface_ = pNInterface;
//	numShotsRequested_ = settings ? settings->SolutionSize : 0;
//   pCmdOpt_ = cmdOpt;
//
//   // ???
//   //pNInterface_->SetLevenshteinDistance(lDistance);
//
//   if (pShotInput != nullptr) pShotInput_ = pShotInput;
//   if (pShotOutput != nullptr) pShotOutput_ = pShotOutput;
//
//	pNInterface_->SetShotOutput(pShotOutput_);
//	SetPrintFiles();
//}
//
//NarrativeOptions::~NarrativeOptions() {}
//
//ShotLibrary* NarrativeOptions::GetShotLibrary() const
//{
//	return pShotLibrary_;
//}
//
//NarrativeInterface* NarrativeOptions::GetNInterface() const
//{
//	return pNInterface_;
//}
//
//vector<SHOTCONSTRAINTINPUT>* NarrativeOptions::GetShotInput() const
//{
//	return pShotInput_;
//}
//
//vector<SHOTCONSTRAINTOUTPUT>* NarrativeOptions::GetShotOutput() const
//{
//	return pShotOutput_;
//}
//
//GLOBALSETTINGS* NarrativeOptions::GetGlobalSettings() const
//{
//   return pSettings_;
//}
//
//int NarrativeOptions::GetNumShotsRequested() const
//{
//   return numShotsRequested_;
//}
//
//CmdLnOptions* NarrativeOptions::GetCmdLnOptions() const
//{
//   return pCmdOpt_;
//}
//
//
//void NarrativeOptions::SetPrintFiles()
//{
//	//out_file(OUTFILE);
//	//log_file(LOGFILE);
//}