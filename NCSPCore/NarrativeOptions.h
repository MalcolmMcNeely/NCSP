////
//// Configuration settings for a Narrative Space or Narrative Script
//// within the Gecode Framework
////
//#pragma once
//#include "common.h"
//
//class ShotLibrary;
//class NarrativeInterface;
//struct SHOTCONSTRAINTINPUT;
//struct SHOTCONSTRAINTOUTPUT;
//struct GLOBALSETTINGS;
//
//class NarrativeOptions : public Gecode::Options
//{
//public:
//   NarrativeOptions(const char* pString, 
//                    ShotLibrary* pShotLib, 
//                    NarrativeInterface* pNInterface,
//                    int numShots);
//	NarrativeOptions(const char* pString,
//						  ShotLibrary* pShotLib,
//						  NarrativeInterface* pNInterface,
//						  GLOBALSETTINGS* settings,
//                    std::vector<SHOTCONSTRAINTOUTPUT>* pShotOutput,
//                    CmdLnOptions* cmdOpt = nullptr,
//                    std::vector<SHOTCONSTRAINTINPUT>* pShotInput = nullptr);
//   ~NarrativeOptions();
//
//   ShotLibrary* GetShotLibrary() const;
//   NarrativeInterface* GetNInterface() const;
//   std::vector<SHOTCONSTRAINTINPUT>* GetShotInput() const;
//	std::vector<SHOTCONSTRAINTOUTPUT>* GetShotOutput() const;
//   GLOBALSETTINGS* GetGlobalSettings() const;
//   int GetNumShotsRequested() const;
//   CmdLnOptions* GetCmdLnOptions() const;
//
//private:
//
//	void SetPrintFiles();
//
//   ShotLibrary* pShotLibrary_;
//   NarrativeInterface* pNInterface_;
//   std::vector<SHOTCONSTRAINTINPUT>* pShotInput_;
//	std::vector<SHOTCONSTRAINTOUTPUT>* pShotOutput_;
//   CmdLnOptions* pCmdOpt_;
//	GLOBALSETTINGS* pSettings_;
//   int numShotsRequested_;
//};
//
