//
// A class for holding the output of a solved gecode problem
//
#pragma once
#include "ConstraintEnums.h"

#include <map>
#include <vector>
#include <string>

struct SolutionResult;
//
// I am bad and haven't really defined what strings are used with this class
// anywhere. Here is a list of what strings are currently used:
//
// HierarchProfile.cpp
// "Location"
// "TotalDuration"
// "Character"
// 
// StoryProfile.cpp
// "Solution"
// "Durations"
// "Solution Size"
// "Library Size"
// "TotalDuration"
// "Cost"
//
class GOutput
{
public:
   
   GOutput();
   GOutput(const GOutput& other);
   ~GOutput();
	   	
   // Merges output with another. Does not copy SolutionFound member.
   void Concat(GOutput& other);
	void Clear();

	// Takes raw solution data, removes NOSHOTS, discards incomplete
	// solutions (where not all segments have been solved), concatenates 
	// the remainder, then converts it into a vector.
	static std::vector<GOutput> ConcatCompleteSolutions(SolutionResult& data);

   void AddData(std::string name, std::vector<int> data);
   void AddData(std::string name, int data);
   bool GetData(std::string name, std::vector<int> *out);
   bool GetData(std::string name, int *out);
   bool GetDataSize(std::string name, int* out);
   bool GetDataAtPosition(std::string name, int pos, int *out);

   void SetSolutionFound(bool b) { solutionFound_ = b; }
   void SetSolveTime(long long l) { solveTime_ = l; }
   void SetSolutionType(eSolutionType t) { solutionType_ = t; }
   void SetID(int i) { id_ = i; }

   bool GetSolutionFound() { return solutionFound_; }
   long long GetSolveTime() { return solveTime_; }
   eSolutionType GetSolutionType() { return solutionType_; }
   int GetID() { return id_; }
   int GetNumConcatenations() { return numConcatenations_; }

   void AddLocation(int l) { locations_.push_back(l); }
   std::vector<int> GetLocations() { return locations_; }

private:

   std::map<std::string, std::vector<int>> vMap_;
   std::map<std::string, int> iMap_;
   std::vector<int> locations_;
   long long solveTime_;
   bool solutionFound_;
   eSolutionType solutionType_;
   int id_;
   int numConcatenations_;
};