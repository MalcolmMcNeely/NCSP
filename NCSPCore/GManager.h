//
// Class for handling the top level of the gecode solver
//
#pragma once
#include "GOutput.h"
#include "GResultContainers.h"

class GManagerOptions;
class ShotLibrary;
class Logger;

class GManager
{
public:
   GManager(GManagerOptions* opt, ShotLibrary* lib);
   ~GManager();

	// Runs the currently configured constraint problem.
	// If a pointer to solution result is passed in, the one
	// who called the function is responsible for it's destruction
	// Mode (optional) refers to how information is outputted:
	//		0 = normal logging to file
	//		1 = will try and output text to a pipe
	// Log (optional) passes a logging object that will be used
	// throughout the entire process
	void Execute(SolutionResult* solutionResult = nullptr, 
					 int mode = 0,
					 Logger* log = nullptr);
	// Creates a directory using DATA_FOLDER_PATH and writes
	// one file per location containing: each individual shot
	// and some aggregate measurements
   bool PrintLibrary();
	// Create synthetic shot library with values somewhat evenly 
	// distributed in multiples of (32 * eLocation::eL_SIZE).
	// All created shots have positive costs for the specified 
	// viewer type
   void CreateDummyLibrary(int vt, 
                           ShotLibrary* out, 
                           int sizeMultiplier = 1);
	// Right now this function just spits out info for logging
	void Initialise(Logger* log = nullptr);

private:

   bool SolveHierarch(std::vector<GOutput>* out,
							 Logger* log = nullptr);
   bool SolveSegment(int loc, 
                     int duration, 
                     int durationBound,
                     int minCost,
                     int charId,
                     SegmentResult& output,
							Logger* log = nullptr);
   bool SolveVariants(int duration,
                      int durationBound,
                      int minCost,
                      int charId,
                      ShotLibrary* lib,
                      SegmentResult& output,
							 Logger* log = nullptr);

	// If a problem has specified options of duration "x", "n" number 
	// of segments, and duration of all shots at one location "m" then 
	// the actual duration spent in each location will be:
	//				scaled duration = (x / n) * (x / m)
	// (something similiar happens to the duration bounds)
	void ScaleDurationsAndBounds(std::vector<GOutput>& hierarchOutputs,
										  std::vector<std::vector<int>>* durations,
										  std::vector<std::vector<int>>* bounds);

   GManagerOptions* options_;
   ShotLibrary* library_;
};

