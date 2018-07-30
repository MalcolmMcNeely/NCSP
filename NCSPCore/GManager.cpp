//
// Implementation of GManager.h
//
#include "GManager.h"
#include "Utility.h"
#include "Shot.h"
#include "ShotLibrary.h"
#include "ShotLibraryFilter.h"
#include "GManagerOptions.h"
#include "GWorkerOptions.h"
#include "GWorker.h"
#include "HierarchProfile.h"
#include "StoryProfile.h"
#include "StoryProfileVariant.h"
#include "MinMaxProfile.h"
#include "ConstraintEnums.h"
#include "GTimer.h"
#include "GFileGenerator.h"
#include "Logger.h"

using namespace Utility;
using namespace Gecode;
using namespace std;

GManager::GManager(GManagerOptions* opt, ShotLibrary* lib) : options_(opt), library_(lib)
{
}

GManager::~GManager()
{
}

void GManager::Execute(SolutionResult* solutionResult, int mode, Logger* log)
{
   GTimer totalSolveTimer;
   int numSegments = options_->GetNumSegments();
   int duration = options_->GetDuration();
   int durationBound = options_->GetDurationBound();
   std::vector<int> minCosts = options_->GetMinCosts();
   int hierarchIterations = options_->GetNumHierarchSolutions();
   int numVariants = options_->GetNumVariantSolutions();	
	
	string messageOut;
	vector<GOutput> hierarchOutputs;
	totalSolveTimer.Start();

   if (SolveHierarch(&hierarchOutputs, log))
   {
      // Hierarch iteration < H Solutions < Segment Solutions > > >
		// Use DLL owned memory if possible
		SolutionResult* allSolutions = solutionResult == nullptr ? 
										new SolutionResult() : solutionResult;
      vector<vector<int>> scaledDurations;
      vector<vector<int>> scaledBounds;

      scaledDurations.resize(hierarchOutputs.size());
      scaledBounds.resize(hierarchOutputs.size());
      ScaleDurationsAndBounds(hierarchOutputs, &scaledDurations, &scaledBounds);

      // For each Hierarch solution (although there should only be 1 in current version)
      for (int i = 0; i < hierarchIterations; i++)
      {
         vector<int> outputLocations;
         if (!hierarchOutputs[i].GetData("Location", &outputLocations)) return;
         int mainCharacterId;
         if (!hierarchOutputs[i].GetDataAtPosition("Character", 0, &mainCharacterId)) return;
            
         // For each segment
         for (int j = 0; j < numSegments; j++)
         {         
            // Find solution and variants
            SegmentResult output;
            SolveSegment(outputLocations[j],
                         scaledDurations[i][j],
                         scaledBounds[i][j],
                         minCosts[j],
                         mainCharacterId,
                         output,
								 log);

            allSolutions->Results.push_back(output);
         }

			allSolutions->mainCharacterName = NarrativeEnums::GetCharName((eChar)mainCharacterId);
      }

		totalSolveTimer.Stop();
		auto totalTime = totalSolveTimer.GetTime();
		
		messageOut.clear();
		messageOut = "\nTotal time: " + GTimer::GetTimeString(totalTime) + "\n";
		OutputText(messageOut.c_str(), log);

      // If solution(s) have been found, generate files
      if (!allSolutions->IsEmpty())
      {
         GFileGenerator::GenerateFiles("output",
                                       GTimer::GetTimeString(totalTime),
                                       *allSolutions,
                                       options_->GetNumSegments(),
                                       options_->GetNumHierarchSolutions(),
                                       options_->GetNumVariantSolutions());
      }

		// Delete if this program owns the memory
		if (solutionResult == nullptr)
		{
			delete allSolutions;
		}
   }
   else
   {
      OutputText("### Hierarch Solution Not Found ###\n");
   }
}

bool GManager::PrintLibrary()
{
   string outputFolder;
   char buffer[MAX_GPATH];
   GetCurrentDirectory(MAX_GPATH, buffer);

   stringstream ssDir;
   ssDir << buffer << DATA_FOLDER_PATH;
   outputFolder = ssDir.str();

   if (CreateDirectory(outputFolder.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
   {
      for (int i = 0; i < (int)eL_SIZE; i++)
      {
         stringstream ss;
         ss.str(string());
         ss << buffer << DATA_FOLDER_PATH;
         ss << NarrativeEnums::GetLocationName((eLocation)i);
         ss << ".txt";
         library_->LogShotsAtLocation(i, ss.str());
         cout << "\n\n";
      }

      return true;
   }

   return false;
}

void GManager::CreateDummyLibrary(int vt, ShotLibrary* out, int sizeMultiplier)
{
   vector<Shot> shots;
   int idBase = 16000;
   int count = 0;
   int size = max(0, sizeMultiplier);

   if (size == 0) return;

   for (int m = 0; m < size; m++)
   {
      for (int i = 1; i < (int)eL_SIZE; i++)
      {
         for (int c = 0; c < 4; c++)
         {
            for (int j = 0; j < 2; j++)
            {
               for (int k = 0; k < 4; k++)
               {
                  Shot shot;
                  int id = idBase + count;
                  shot.SetID(id);
                  shot.SetLocation1((eLocation)i);
                  shot.SetTimeDuration(3 + c + j + k);

                  if ((eViewerType)vt == eV_GORE)
                  {
                     shot.SetGore(k);

                     if (j == 0)
                     {
                        shot.SetKill(1);
                     }
                     else if (j == 1)
                     {
                        shot.SetKill(1);
                        shot.SetAttack(1);
                     }
                  }
                  else
                  {
                     shot.SetThrill(k);

                     if (j == 0)
                     {
                        shot.SetEmpathy(1);
                     }
                     else if (j == 1)
                     {
                        shot.SetThreaten(1);
                        shot.SetChase(1);
                     }
                     else if (j == 2)
                     {
                        shot.SetHide(1);
                        shot.SetEscape(1);
                     }
                  }

                  if (c == 0)
                  {
                     Character c1(eC_SIDNEY);
                     Character c2(eC_BILLY);

                     shot.AddCharacter(c1);
                     shot.AddCharacter(c2);
                  }
                  else if (c == 1)
                  {
                     Character c1(eC_DEWEY);
                     Character c2(eC_TATUM);

                     shot.AddCharacter(c1);
                     shot.AddCharacter(c2);
                  }
                  else if (c == 2)
                  {
                     Character c1(eC_DEWEY);
                     Character c2(eC_GALE);

                     shot.AddCharacter(c1);
                     shot.AddCharacter(c2);
                  }
                  else if (c == 3)
                  {
                     Character c1(eC_TATUM);
                     Character c2(eC_SIDNEY);

                     shot.AddCharacter(c1);
                     shot.AddCharacter(c2);
                  }

                  shots.push_back(shot);
                  count++;
               }
            }
         }
      }
   }

   for (auto& s : shots)
   {
      if (!out->AddShot(s))
      {
         std::cout << "Adding shot " << s.GetId() << " failed." << std::endl;
      }
   }
}

void GManager::Initialise(Logger* log)
{
   stringstream ss;
   OutputText("\nBegin Manager Execute...", log);

   eViewerType vType = options_->GetViewerType();
   int numSegments = options_->GetNumSegments();
   int duration = options_->GetDuration();
   int durationBound = options_->GetDurationBound();
   int timeout = options_->GetTimeout();
   int characterAppearanceRate = options_->GetCharacterAppearanceRate();
   std::vector<int> minCosts = options_->GetMinCosts();

	ss << "\nNumSegments: " << numSegments <<
		       "\nDuration: " << duration <<
		       "\nDuration Bound: " << durationBound <<
		       "\nTimeout: " << timeout <<
		       "\nCharacter Appearance Rate: " << characterAppearanceRate <<
		       "\nMin Costs: ";

   for (int i : minCosts)
   {
		ss << i << "\t";
   }
	ss << "\n";

	string out;
	out = ss.str();
	OutputText(out.c_str(), log);
}

bool GManager::SolveHierarch(vector<GOutput>* out, Logger* log)
{
   OutputText("Begin Heirarch Solving...\n", log);
   HierarchProfile hProfile;
   GOutput hProfileOut;
   int timeout = options_->GetTimeout();
   int iterations = options_->GetNumHierarchSolutions();
   int solutionCount = 0;
   GTimer totalTimer;
   GTimer solveTimer;
	stringstream ss;
	string message;
   
   GWorkerOptions hierarchOptions("Heirarch Solution");
   hierarchOptions.SetProfile(&hProfile);
   hierarchOptions.SetOutput(&hProfileOut);
   hierarchOptions.SetLibrary(library_);
   hierarchOptions.SetCosts(options_->GetMinCosts());
   hierarchOptions.AddArgument(eOpt_SolutionSize, options_->GetNumSegments());
   hierarchOptions.AddArgument(eOpt_Duration, options_->GetDuration());
   hierarchOptions.AddArgument(eOpt_DurationBound, options_->GetDurationBound());
   hierarchOptions.AddArgument(eOpt_MainCharAppearanceRate, options_->GetCharacterAppearanceRate());
   if (timeout) hierarchOptions.time(timeout);

   GWorker* worker = new GWorker(hierarchOptions);
   BAB<GWorker> e(worker);
   
	totalTimer.Start();
   for (int i = 0; i < iterations; i++)
   {
		solveTimer.Start();
      GWorker* s = e.next();
		solveTimer.Stop();

      if (s != nullptr)
      {         
			auto solveTime = solveTimer.GetTime();
         hProfileOut.SetSolveTime(solveTime);
         s->print(std::cout);         
         out->push_back(GOutput(hProfileOut));
         solutionCount++;
         delete s;
      }
      else
      {
			ss.str(string());
			message.clear();

			ss << "Hierarch search stopped at iteration " << i << " of " 
				<< iterations << ".\n";			
			message = ss.str();
			OutputText(message.c_str(), log);
         break;
      }
   }

	totalTimer.Stop();
	auto totalTime = totalTimer.GetTime();

	ss.str(string());
	message.clear();
	ss << "\n" << solutionCount << " Hierarch solutions found in " << GTimer::GetTimeString(totalTime) << "\n";
	message = ss.str();
	OutputText(message.c_str(), log);

	delete worker;
   return (solutionCount > 0);
}

void GManager::ScaleDurationsAndBounds(vector<GOutput>& hierarchOutputs,
                                       vector<vector<int>>* durations,
                                       vector<vector<int>>* bounds)
{
   // Indivdual scale factors for the duration of each segment
   stringstream ss;
   int numSegments = options_->GetNumSegments();
   int duration = options_->GetDuration();
   int hSize = hierarchOutputs.size();
   int temp;

   for (int i = 0; i < hSize; i++)
   {
      float maxDuration = 0.0f;
      for (int j = 0; j < numSegments; j++)
      {
         temp = 0;
         hierarchOutputs[i].GetDataAtPosition("TotalDuration", j, &temp);
         maxDuration += temp;
      }
      float scaleFactor = duration / maxDuration;

      // Scale duration and bounds
      for (int j = 0; j < numSegments; j++)
      {
         temp = 0;
         hierarchOutputs[i].GetDataAtPosition("TotalDuration", j, &temp);
         (*durations)[i].push_back((int)(temp * scaleFactor));

         float x = maxDuration / temp;
         float y = 100 / x;
         float z = y * 0.01f;
         float b = z * 10;
         (*bounds)[i].push_back((int)(b));
      }
   }
}

bool GManager::SolveSegment(int loc, 
                            int duration, 
                            int durationBound,
                            int minCost,
                            int charId,
                            SegmentResult& output,
									 Logger* log)
{
   GTimer timer;
   long long solveTime;

   ShotLibraryFilter libFilter;
   ShotLibrary storyLib;
   libFilter.FilterByLocation(library_, &storyLib, (eLocation)loc);

   eViewerType vType = options_->GetViewerType();
   int characterAppearanceRate = options_->GetCharacterAppearanceRate();
   int timeout = options_->GetTimeout();
   int segmentIterations = options_->GetNumSegmentSolutions();

	stringstream ss;
	string message;

   ss.str(string());
   ss << "## Segment Solution\n";
   string segmentName = ss.str();
   OutputText(segmentName.c_str(), log);
   const char *pSegmentName = segmentName.c_str();

   GOutput sOutput;
   StoryProfile sProfile;
   GWorkerOptions segmentOptions(pSegmentName);
   segmentOptions.SetProfile(&sProfile);
   segmentOptions.SetOutput(&sOutput);
   segmentOptions.SetLibrary(&storyLib);
   segmentOptions.AddArgument(eOpt_SolutionSize, storyLib.GetShotLibSize());
   segmentOptions.AddArgument(eOpt_Duration, duration);
   segmentOptions.AddArgument(eOpt_DurationBound, durationBound);
   segmentOptions.AddArgument(eOpt_MinCost, minCost);
   segmentOptions.AddArgument(eOpt_ViewerType, (int)vType);
   segmentOptions.AddArgument(eOpt_MainCharacter, charId);
   segmentOptions.AddArgument(eOpt_MainCharAppearanceRate, characterAppearanceRate);
   if (timeout) segmentOptions.time(timeout);

   GWorker* segmentWorker = new GWorker(segmentOptions);
   BAB<GWorker> segmentSolver(segmentWorker);

   // Solve first solution
   timer.Start();
   GWorker* s = segmentSolver.next();
   timer.Stop();

   if (s != nullptr)
   {
      solveTime = timer.GetTime();

		ss.str(string());
		message.clear();
      ss << "Solve time: " << GTimer::GetTimeString(solveTime) << "\n";
		message = ss.str();
		OutputText(message.c_str(), log);

      sOutput.SetSolveTime(solveTime);
      s->print(std::cout);

      // Store result
      output.Result = GOutput(sOutput);

      // Solve variants
      SolveVariants(duration,
                    durationBound,
                    minCost,
                    charId,
                    &storyLib,
                    output,
						  log);
   }
   else
   {
		ss.str(string());
		message.clear();
		ss << "### Segment not found ###\n";
		message = ss.str();
		OutputText(message.c_str(), log);
   }

   sOutput.Clear();
   delete s;

   delete segmentWorker;
   return true;
}

bool GManager::SolveVariants(int duration,
                             int durationBound,
                             int minCost,
                             int charId,
                             ShotLibrary* lib,
                             SegmentResult& output,
									  Logger* log)
{
   GTimer timer;
   stringstream ss;
	string message;
   long long solveTime;

   eViewerType vType = options_->GetViewerType();
   int characterAppearanceRate = options_->GetCharacterAppearanceRate();
   int variantIterations = options_->GetNumVariantSolutions();
   int timeout = options_->GetTimeout() == 0 ? DEFAULTSEARCHTIMEOUT : options_->GetTimeout();
   int variance = options_->GetMaxVariance();
   bool bStopped = false;

   ss.str(string());
   ss << "## Variant Solution\n";
   string variantName = ss.str();
   OutputText(variantName.c_str(), log);
   const char *pVariantName = variantName.c_str();

   // Prune library and solve variants
   for (int i = 0; i < variantIterations; i++)
   {
      GOutput sOutput;
      StoryProfileVariant sProfile;
      vector<GOutput*> input;

      // Prep previous data for input      
      input.push_back(&output.Result);
      for (int j = 0; j < output.VariantsSize(); j++)
      {
         input.push_back(&output.Variants[j]);
      }


      // Space Options
      GWorkerOptions variantOptions(pVariantName);
      variantOptions.SetProfile(&sProfile);
      variantOptions.SetOutput(&sOutput);
      variantOptions.SetLibrary(lib);
      variantOptions.SetInput(&input);
      variantOptions.AddArgument(eOpt_SolutionSize, lib->GetShotLibSize());
      variantOptions.AddArgument(eOpt_Duration, duration);
      variantOptions.AddArgument(eOpt_DurationBound, durationBound);
      variantOptions.AddArgument(eOpt_MinCost, minCost);
      variantOptions.AddArgument(eOpt_ViewerType, (int)vType);
      variantOptions.AddArgument(eOpt_MainCharacter, charId);
      variantOptions.AddArgument(eOpt_MainCharAppearanceRate, characterAppearanceRate);
      variantOptions.AddArgument(eOpt_VariantDistance, variance);
      
      // Search Options
      Search::Options searchOptions;
      Search::TimeStop* stopObject = nullptr;
      stopObject = new Search::TimeStop(timeout);
      searchOptions.stop = stopObject;

      GWorker* variantWorker = new GWorker(variantOptions);
      BAB<GWorker> variantSolver(variantWorker, searchOptions);

      GWorker* s = nullptr;
      int count = 0;
      int retries = 20;
      bool bFound = false;

      do
      {
         timer.Start();
         s = variantSolver.next();
         timer.Stop();
         count++;

         if (s != nullptr)
         {
            solveTime = timer.GetTime();

				ss.str(string());
				message.clear();
				ss << "Solve time: " << GTimer::GetTimeString(solveTime) << "\n";
				message = ss.str();
				OutputText(message.c_str(), log);

            sOutput.SetSolveTime(solveTime);
            s->print(std::cout);
            output.Variants.push_back(GOutput(sOutput));
            bFound = true;
         }
         else
         {
				ss.str(string());
				message.clear();

            if (stopObject != nullptr)
            {
               bool t1 = variantSolver.stopped();
               if (t1)
               {
                  // Timeout
                  ss << "### Search TIME OUT ";
                  if (count > 0)
                  {
							ss << "- Retry " << count << " of " << retries << " ###";
                  }
                  else
                  {
                     ss << "###";
                  }
						ss << "\n";

                  // Reset time for next search
                  stopObject->reset();
               }
               else
               {
                  // Failed to find any solution
                  bStopped = true;
						ss << "### Search could not find a solution ###\n";
                  break;
               }
            }
            else
            {
               // Search failed (should not get to this point)
               ss << "### Stop Object is NULL ###\n";
               bStopped = true;
            }

				message = ss.str();
				OutputText(message.c_str(), log);
         }

         sOutput.Clear();
         delete s;

      } while ((!bFound) && ((!variantSolver.stopped()) || (count < retries)));

      // Release resources
      delete variantWorker;
      if (stopObject != nullptr) delete stopObject;

      // Search stop condition
      if (bStopped) break;
   }
   
   return true;
}