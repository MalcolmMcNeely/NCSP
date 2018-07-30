#include <algorithm>
#include <string>
#include <sstream>

#include "common.h"
#include "NCSPEntry.h"
#include "ShotLibrary.h"
#include "Shot.h"
#include "Character.h"
#include "ScreamData.h"
#include "SharedDefs.h"
#include "NCSPCmdLnArgs.h"
#include "GManager.h"
#include "GManagerOptions.h"
#include "GWorker.h"
#include "GWorkerOptions.h"
#include "Utility.h"
#include "CmdLnData.h"
#include "Logger.h"

using namespace Gecode;
using namespace std;
using namespace Utility;

int GetShotLibrary(ShotLibrary* lib)
{
   //lib = new ShotLibrary();
   lib->Clear();
   std::vector<Shot> shots;

   if (!ScreamData::GetData(shots))
   {
      // Error has occured in parsing shot data
      std::cout << "\n\n\t##### ERROR IN PARSING SHOT LIBRARY #####\n\n" << std::endl;
      return 1;
   }

   for (auto& s : shots)
   {
      if (!lib->AddShot(s))
      {
         std::cout << "Adding shot " << s.GetId() << " failed. Exiting." << std::endl;
         return 1;
      }
   }

   lib->Finalize();

   return 0;
}

// Entry point via command line
int GMain(CmdLnData& data)
{
   ShotLibrary lib;
   lib.SetViewerType(data.GetViewerType());
   GetShotLibrary(&lib);

   if (lib.GetIsFinalized())
   {
      OutputText("\n\nManager initialising...");
      GManagerOptions managerOptions(data);
      GManager manager(&managerOptions, &lib);      

      // Generate synthetic shot library if specified
		if (data.GetLibraryMultiplier() > 0)
		{
			OutputText("Generating Library...");
			lib.BeginEdit();
			manager.CreateDummyLibrary(data.GetViewerType(), &lib, data.GetLibraryMultiplier());
			lib.EndEdit();
		}

		// Print library if specified
      if (data.GetPrintLibrary())
      {
         manager.PrintLibrary();
      }      

      manager.Execute();
   }

   return 0;
}

// Entry point via NCSPLib API
int GMain(PARAMS* params, 
		    vector<int>* minCosts, 
			 SolutionResult* solutionResult,
			 ShotLibrary* lib)
{
	// Set up logging/pipe communication
	Logger log("Debug Output", 1);

	if (!log.InitialisePipe())
	{
		cout << "Can not initialise pipe" << endl;
		return 1;
	}

	OutputText("Communication with NCSP established\n", &log);

	lib->SetViewerType(params->ViewerType);
	GetShotLibrary(lib);

	CmdLnData data;
	data.SetDefaults();
	data.SetViewerType(params->ViewerType);
	data.SetSegments(params->NumSegments);
	data.SetDuration(params->Duration);
	data.SetDurationBound(params->DurationBound);
	data.SetCharacterAppearanceRate(params->CharacterAppearanceRate);	
	data.SetMinCosts(*minCosts);
	data.SetTimeout(params->Timeout);
	data.SetMaxVariance(params->Variance);
	data.SetVariantSolutions(params->NumVariants);
	data.SetLibraryMultiplier(params->LibraryMultiplier);
	data.SetPrintLibrary((bool)params->PrintLibraryDetails);
	data.SetWait(false);

	if (lib->GetIsFinalized())
	{
		OutputText("Manager initialising...\n", &log);
		GManagerOptions managerOptions(data);
		GManager manager(&managerOptions, lib);

		// Generate synthetic shot library if specified
		if (data.GetLibraryMultiplier() > 0)
		{
			OutputText("Generating Library...\n", &log);
			lib->BeginEdit();
			manager.CreateDummyLibrary(data.GetViewerType(), lib, data.GetLibraryMultiplier());
			lib->EndEdit();
		}		

		// Print library if specified
		if (data.GetPrintLibrary())
		{
			OutputText("Printing Library Analysis...\n", &log);
			manager.PrintLibrary();
		}

		// Output configuration
		manager.Initialise(&log);

		// Execute manager while it write to pipe
		manager.Execute(solutionResult, 1, &log);
	}

	// Close the pipe
	log.Dispose();

	return 0;
}