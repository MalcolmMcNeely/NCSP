//
// Configuration settings for the GWorker
//
#pragma once
#include "common.h"
#include "ConstraintEnums.h"

#include <map>

class ShotLibrary;
class GOutput;
class IConstraintProfile;

class GWorkerOptions : public Gecode::Options
{
public:
   GWorkerOptions(const char* pString);
   ~GWorkerOptions();

   void SetLibrary(ShotLibrary* lib) { library_ = lib; }
   void SetOutput(GOutput* out) { output_ = out; }
   void SetInput(std::vector<GOutput*>* in) { input_ = in; }
   void SetProfile(IConstraintProfile* profile) { profile_ = profile; }
   void SetCosts(std::vector<int> costs);

   ShotLibrary* GetLibrary() const { return library_; }
   std::vector<GOutput*>* GetInput() const { return input_; }
   GOutput* GetOutput() const { return output_; }
   IConstraintProfile* GetProfile() const { return profile_; }
   std::vector<int> GetCosts() const { return costs_; }

   void AddArgument(eSolverOption name, int value);
   bool GetArgument(eSolverOption name, int* out) const;

private:

   ShotLibrary* library_;
   std::vector<GOutput*>* input_;
   GOutput* output_;
   IConstraintProfile* profile_;

   std::map<eSolverOption, int> argMap_;
   std::vector<int> costs_;
};

