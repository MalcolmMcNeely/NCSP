//
// Entry points for both EXE and DLL
//
#pragma once

#include <vector>
#include "SharedDefs.h"
#include "CmdLnData.h"

class ShotLibrary;
struct SolutionResult;

// Command line entry
int GMain(CmdLnData& data);

// DLL entry using pointers provided and managed by invoking DLL
int GMain(PARAMS* params,
			 std::vector<int>* minCosts,
			 SolutionResult* solutionResult,
			 ShotLibrary* lib);

// Takes a pointer to an empty ShotLibrary and populates it with data
int GetShotLibrary(ShotLibrary* lib);