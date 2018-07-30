//
// Implementation of GOutput.h
//
#include "GOutput.h"
#include "common.h"
#include "GResultContainers.h"

using namespace std;

GOutput::GOutput() : solutionFound_(false), 
                     solveTime_(0), 
                     id_(0), 
                     numConcatenations_(0)
{
}

GOutput::GOutput(const GOutput& other)
{
   vMap_ = other.vMap_;
   iMap_ = other.iMap_;
   solveTime_ = other.solveTime_;
   solutionFound_ = other.solutionFound_;
   solutionType_ = other.solutionType_;
   locations_ = other.locations_;
   id_ = other.id_;
   numConcatenations_ = other.numConcatenations_;
}

GOutput::~GOutput()
{
}

void GOutput::Clear()
{
   vMap_.clear();
   iMap_.clear();
   locations_.clear();
   solveTime_ = 0;
   solutionFound_ = false;
   id_ = 0;
   numConcatenations_ = 0;
}

std::vector<GOutput> GOutput::ConcatCompleteSolutions(SolutionResult& data)
{
	// Calculate "complete" solutions (all segments generated)
	int numSolutions = INT_MAX;
	int outputSize = 0;
	for (int i = 0; i < data.Size(); i++)
	{
		numSolutions = min(data[i].VariantsSize(), numSolutions);
	}
	outputSize = numSolutions + 1;

	// Concat each complete solution
	vector<GOutput> concatOutputs;
	concatOutputs.resize(outputSize); // +1 to include original

	for (int i = 0; i < data.Size(); i++)
	{
		// Original solution
		concatOutputs[0].Concat(data[i].Result);

		// Variant solutions
		for (int j = 0; j < numSolutions; j++)
		{
			concatOutputs[j + 1].Concat(data[i][j]);
		}
	}

	return concatOutputs;
}

void GOutput::Concat(GOutput& other)
{
   // Concat known elements in vectors
   for (auto& v1 : vMap_)
   {
      for (auto& v2 : other.vMap_)
      {
         if (strcmp(v1.first.c_str(), v2.first.c_str()) == 0)
         {
            for (auto& v : v2.second)
            {
               vMap_[v2.first].push_back(v);
            }
         }
      }
   }

   // Insert unknown elements
   vMap_.insert(other.vMap_.begin(), other.vMap_.end());

   // Concat known individual elements
   for (auto& i1 : iMap_)
   {
      for (auto& i2 : other.iMap_)
      {
         if (strcmp(i1.first.c_str(), i2.first.c_str()) == 0)
         {
            iMap_[i2.first] += i2.second;
         }
      }
   }
   // Insert unknown elements
   iMap_.insert(other.iMap_.begin(), other.iMap_.end());

   // Locations
   for (auto& l : other.locations_)
   {
      locations_.push_back(l);
   }

   // Solve time
   solveTime_ += other.solveTime_;

   numConcatenations_++;
}

void GOutput::AddData(std::string name, std::vector<int> data)
{
   vMap_[name] = std::vector<int>(data);
}

void GOutput::AddData(std::string name, int data)
{
   iMap_[name] = data;
}

bool GOutput::GetData(std::string name, std::vector<int>* out)
{
   try
   {
      out->clear();
      *out = vMap_.at(name);
   }
   catch (const std::out_of_range exception)
   {
      return false;
   }

   return true;
}

bool GOutput::GetData(std::string name, int *out)
{
   try
   {
      *out = -1;
      *out = iMap_.at(name);
   }
   catch (const std::out_of_range exception)
   {
      return false;
   }

   return true;
}

bool GOutput::GetDataSize(std::string name, int* out)
{
   try
   {
      *out = vMap_.at(name).size();
   }
   catch (const std::out_of_range exception)
   {
      return false;
   }

   return true;
}

bool GOutput::GetDataAtPosition(std::string name, int pos, int *out)
{
   try
   {
      *out = vMap_.at(name)[pos];
   }
   catch (const std::out_of_range exception)
   {
      return false;
   }

   return true;
}