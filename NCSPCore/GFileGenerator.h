//
// Static class used for generating files after
// solving has been completed
//
#pragma once
#include "common.h"
#include "GOutput.h"
#include "GResultContainers.h"

class GFileGenerator
{
public:
   GFileGenerator();
   ~GFileGenerator();

   static void GenerateFiles(std::string filename,
      std::string timeString,
      SolutionResult& data,
      int numSegments,
      int numHierarchSolutions,
      int numVariants);

private:

   static std::string CreateDatafileName(std::string s);
   static std::string CreateScriptfileName(std::string s);
   static void FindMinMaxOutputs(std::vector<GOutput> outputs,
                                 GOutput& min,
                                 GOutput& max,
                                 int& diff);
   static int GetDifference(GOutput& a, GOutput& b);
};

