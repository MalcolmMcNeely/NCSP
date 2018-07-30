//
// DLL for C#/C++ interoperability.
//
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

#include "SharedDefs.h"
#include "GOutput.h"

#ifdef CSPLIB_EXPORTS
#define CSPLIB_API __declspec(dllexport)
#else
#define CSPLIB_API __declspec(dllimport)
#endif

class ShotLibrary;
struct SolutionResult;

class LibObject
{
public:

   LibObject();
   ~LibObject();

	int Initialise();
   void Shutdown();
   void Reset();
	bool CanRun();
   int Run();

	void SetParameters(PARAMS params);
	void AddCost(int cost);

   int GetCharacterNameSize();
   const char* GetCharacterNameByIndex(int index);
	
	// Solution access functions
	bool GetSolutionFound();
	int GetAllResultsSize();
	int GetResultSize(int i);
	void GetResultElement(int i, int j, SHOTINFO& info);

	int GetNumCharactersInElement(int id);
	const char* GetCharacterInElement(int id, int i);
	const char* GetFileLocation(int id);
	const char* GetMainCharacterName();

private:

   bool CheckGlobalSettingsAreSet();

	ShotLibrary* shotLib_;
	SolutionResult* solutionResult_;
	std::vector<GOutput> concatenatedData_;

	PARAMS params_;
	std::vector<int> minCosts_;
	bool bParamsAreSet_;
	
	// Persistent storage for returning strings 
	// to managed code
	std::string cache_;
};