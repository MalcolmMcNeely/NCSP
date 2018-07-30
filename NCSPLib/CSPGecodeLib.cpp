#define CSPLIB_EXPORTS

#include <algorithm>

#include "CSPGecodeLib.h"
#include "NCSPEntry.h"
#include "NarrativeEnums.h"
#include "Utility.h"
#include "ShotLibrary.h"
#include "GResultContainers.h"
#include "Shot.h"

using namespace std;
using namespace Utility;

LibObject::LibObject()
{
   Reset();
}
LibObject::~LibObject() {}

int LibObject::Initialise()
{
	// TODO: anything that needs shutting down. Remove if needed.
   return 0;
}

void LibObject::Shutdown()
{
	Reset();
}

void LibObject::Reset()
{
	minCosts_.clear();
	bParamsAreSet_ = false;
	delete shotLib_;
	delete solutionResult_;
}

bool LibObject::CanRun()
{
	return (bParamsAreSet_ && (minCosts_.size() > 0));
}

int LibObject::Run()
{
   int result = 0;

	if (!CanRun())
   {
      return 1;
   }

	shotLib_ = new ShotLibrary();
	solutionResult_ = new SolutionResult();
   result = GMain(&params_, &minCosts_, solutionResult_, shotLib_);

	if (!solutionResult_->IsEmpty())
	{
		concatenatedData_ = GOutput::ConcatCompleteSolutions(*solutionResult_);
	}

   return result;
}

void LibObject::SetParameters(PARAMS params)
{
	params_.ViewerType = params.ViewerType;
	params_.NumSegments = params.NumSegments;
	params_.Duration = params.Duration;
	params_.DurationBound = params.DurationBound;
	params_.CharacterAppearanceRate = params.CharacterAppearanceRate;
	params_.Variance = params.Variance;
	params_.NumVariants = params.NumVariants;
	params_.LibraryMultiplier = params.LibraryMultiplier;
	params_.Timeout = params.Timeout;
	params_.PrintLibraryDetails = params.PrintLibraryDetails;
	bParamsAreSet_ = true;
}

void LibObject::AddCost(int cost)
{
	minCosts_.push_back(cost);
}

bool LibObject::GetSolutionFound()
{
	if (solutionResult_ == nullptr)
	{
		return false;
	}

	return !concatenatedData_.empty();
}

int LibObject::GetAllResultsSize()
{
	return concatenatedData_.size();
}

int LibObject::GetResultSize(int i)
{
	int temp = 0;
	return concatenatedData_[i].GetData("Solution Size", &temp) ? temp : 0;
}

void LibObject::GetResultElement(int i, int j, SHOTINFO& info)
{
	vector<int> temp;
	if (concatenatedData_[i].GetData("Solution", &temp))
	{
		if (temp.size() > j)
		{
			Shot s = shotLib_->GetShotById(temp[j]);
			info.ID = s.GetId();
			info.Gore = s.GetGore();
			info.Thrill = s.GetThrill();
			info.Empathy = s.GetEmpathy();
			info.Threaten = s.GetThreaten();
			info.Chase = s.GetChase();
			info.Hide = s.GetHide();
			info.Kill = s.GetKill();
			info.Attack = s.GetAttack();
			info.Escape = s.GetEscape();
			info.Location = s.GetLocation();
			info.Duration = s.GetTimeDuration();

			if (shotLib_->GetViewerType() == (int)eV_THRILL)
			{
				info.Cost = (1 + info.Empathy + info.Escape + info.Hide +
					info.Threaten + info.Chase) * info.Thrill;
			}
			else
			{
				info.Cost = (1 + info.Attack + info.Kill) * info.Gore;
			}
		}
	}
}

int LibObject::GetNumCharactersInElement(int id)
{
	Shot s = shotLib_->GetShotById(id);
	return s.GetCharacters().size();
}

const char* LibObject::GetCharacterInElement(int id, int i)
{
	cache_.clear();
	Shot s = shotLib_->GetShotById(id);
	auto charList = s.GetCharacters();

	if (charList.size() > 0)
	{
		cache_ = NarrativeEnums::GetCharName(charList[i].GetCharName());
		return cache_.c_str();
	}

	return nullptr;
}

const char* LibObject::GetFileLocation(int id)
{
	cache_.clear();
	Shot s = shotLib_->GetShotById(id);
	cache_ = s.GetFileLocation();
	return cache_.c_str();
}

const char* LibObject::GetMainCharacterName()
{
	cache_.clear();
	cache_ = solutionResult_->mainCharacterName;
	return cache_.c_str();
}

#pragma region DLL Exports

// Global LibObject for export usage
LibObject lib;

extern "C"
{
	CSPLIB_API int NCSPLibInitialise()
	{
		return lib.Initialise();
	}

   CSPLIB_API void NCSPLibShutdown()
   {
      lib.Shutdown();
   }
   
   CSPLIB_API void NCSPLibReset()
   {
      lib.Reset();
   }

   CSPLIB_API int NCSPLibRun()
   {
      return lib.Run();
   }

	CSPLIB_API void NCSPSetParams(PARAMS params)
	{
		lib.SetParameters(params);
	}

	CSPLIB_API void NCSPAddMinCost(int cost)
	{
		lib.AddCost(cost);
	}

	CSPLIB_API bool NCSPGetSolutionFound() // going to have to marshal in C#
	{
		return lib.GetSolutionFound();
	}

	CSPLIB_API int NCSPGetAllResultsSize()
	{
		return lib.GetAllResultsSize();
	}

	CSPLIB_API int NCSPGetResultSize(int i)
	{
		return lib.GetResultSize(i);
	}

	CSPLIB_API void NCSPGetResultElement(int i, int j, SHOTINFO info)
	{
		lib.GetResultElement(i, j, info);
	}

	CSPLIB_API int NCSPGetNumCharactersInElement(int id)
	{
		return lib.GetNumCharactersInElement(id);
	}

	CSPLIB_API const char* NCSPGetCharacterInElement(int id, int i)
	{
		return lib.GetCharacterInElement(id, i);
	}
						 
	CSPLIB_API const char* NCSPGetFileLocation(int id)
	{
		return lib.GetFileLocation(id);
	}

	CSPLIB_API const char* NCSPGetMainCharacterName()
	{
		return lib.GetMainCharacterName();
	}

	CSPLIB_API int NCSPGetNoShotID()
	{
		return NOSHOTID;
	}
}
#pragma endregion