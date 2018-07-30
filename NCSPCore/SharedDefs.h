//
// Definitions for key structures shared to the GUI
//
#pragma once

#include <vector>
#include <map>
#include <string>

struct PARAMS
{
	int ViewerType;
	int NumSegments;
	int Duration;
	int DurationBound;
	int CharacterAppearanceRate;
	int Variance;
	int NumVariants;
	int LibraryMultiplier;
	int Timeout;
	int PrintLibraryDetails;
};

struct SHOTINFO
{
	int ID;
	int Cost;
	int Gore;
	int Thrill;
	int Empathy;
	int Threaten;
	int Chase;
	int Hide;
	int Kill;
	int Attack;
	int Escape;
	int Location;
	int Duration;
};