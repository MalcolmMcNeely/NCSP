#pragma once

#include "GOutput.h"

#include <vector>
#include <string>

struct SegmentResult
{
   GOutput& operator[] (int x)
   {
      return Variants[x];
   }
   void PushBackVariant(GOutput& o) { Variants.push_back(o); }
   int VariantsSize() { return (int)Variants.size(); }

   GOutput Result;
   std::vector<GOutput> Variants;
};

struct SolutionResult
{
   SegmentResult& operator[] (int x)
   {
      return Results[x];
   }
   bool IsEmpty() { return Results.empty(); }
   int Size() { return (int)Results.size(); }

   std::vector<SegmentResult> Results;
	std::string mainCharacterName;
};