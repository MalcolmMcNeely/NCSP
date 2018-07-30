//
// Implementation of GFileGenerator.h
//
#include "GFileGenerator.h"
#include "Logger.h"
#include "GTimer.h"

#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

GFileGenerator::GFileGenerator() {}
GFileGenerator::~GFileGenerator() {}

void GFileGenerator::GenerateFiles(string filename,
                                   std::string timeString,
                                   SolutionResult& data,
                                   int numSegments,
                                   int numHierarchSolutions,
                                   int numVariants)
{
   stringstream ss;
   int idCount = 1;

   // Create output directory (if needed)
   {
      string outputFolder;
      char buffer[MAX_GPATH];
      GetCurrentDirectory(MAX_GPATH, buffer);

      stringstream ssDir;
      ssDir << buffer << OUTPUT_FOLDER_PATH;
      outputFolder = ssDir.str();

      CreateDirectory(outputFolder.c_str(), NULL);
   }

	vector<GOutput> concatOutputs = GOutput::ConcatCompleteSolutions(data);

   // Print individual solutions
   {
      // Create solution directory
      string outputFolder;
      char buffer[MAX_GPATH];
      GetCurrentDirectory(MAX_GPATH, buffer);

      stringstream ssDir;
      ssDir << buffer << SOLUTIONS_FOLDER_PATH;
      outputFolder = ssDir.str();

      stringstream deleteDir;
      deleteDir << buffer << SOLUTIONS_FOLDER_DELETE_PATH;

      if (CreateDirectory(outputFolder.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
      {
         // Delete all files in directory
         string command = "del /Q ";
         system(command.append(deleteDir.str()).c_str());

         // Write new files
         for (int i = 0; i < concatOutputs.size(); i++)
         {
            ss.str(string());
            ss << buffer << SOLUTIONS_FOLDER_PATH << filename << " " << i << ".log";

            string fileName = ss.str();
            Logger log(fileName.c_str());

            log << "# Auto-generated data file: " << fileName << endl;

            vector<int> shotIds;
            vector<int> durations;
            concatOutputs[i].GetData("Solution", &shotIds);
            concatOutputs[i].GetData("Durations", &durations);

            vector<TwoInt> idDurations;
            for (int j = 0; j < shotIds.size(); j++)
            {
               idDurations.push_back(TwoInt(shotIds[j], durations[j]));
            }

            // Remove NO-SHOTs before printing if exists
            idDurations.erase(remove_if(begin(idDurations), end(idDurations),
               [](TwoInt i){return (i.First == NOSHOTID); }),
               end(idDurations));

            // Write line by line: ID, Duration
            for (auto i : idDurations)
            {
               log << i.First << "\t";
            }
            log << endl;
            for (auto i : idDurations)
            {
               log << i.Second << "\t";
            }
         }
      }
      else
      {
         cout << "### Failed to create solution output directory. Error: " << GetLastError() << endl;
      }
   }
   
   // Concat each variant of each solutions
   /*
   for (int i = 0; i < numHierarchSolutions; i++)
   {
      for (int j = 0; j < numSegmentSolutions; j++)
      {
         for (int x = 0; x < numVariants; x++)
         {
            GOutput output;

            for (int k = 0; k < numSegments; k++)
            {
               // If it's a complete solution
               if (j < data[i][k].size())
               {
                  output.Concat(data[i][k][j].Result);
               }
            }

            if (output.GetNumConcatenations() == numSegments) // Compare to 0-index
            {
               output.SetID(idCount);
               output.SetSolutionFound(true);
               concatOutputs.push_back(GOutput(output));
               idCount++;
            }
         }
      }
   }
   */
   /*
   // Print all variant solutions
   {
      string outputFolder;
      char buffer[MAX_GPATH];
      GetCurrentDirectory(MAX_GPATH, buffer);

      stringstream ssDir;
      ssDir << buffer << VARIANT_SOLUTIONS_FOLDER_PATH;
      outputFolder = ssDir.str();

      stringstream deleteDir;
      deleteDir << buffer << VARIANT_SOLUTIONS_FOLDER_DELETE_PATH;

      if (CreateDirectory(outputFolder.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
      {
      }
   }
   */

   // Generate .dat file for GnuPlot
   {
      int dArr[6];
      int temp = 0;
      bool status = true;
      string solveTime;

      string datafilename = CreateDatafileName(filename);
      Logger dLog(datafilename.c_str());
      dLog << "# Auto-generated data file: " << filename << endl;
      dLog << "# Total solve time: " << timeString << endl;
      dLog << "# ID, Library Size, Solution Size, Duration, Solve Time, Cost," \
         << " Difference from min, Difference from max, Locations(" \
         << numSegments << ")" << endl;

      // Find min max solutions      
      GOutput minMax[2];
      int diff = 0;
      FindMinMaxOutputs(concatOutputs, minMax[0], minMax[1], diff);
      dLog << endl;
      dLog << "# Min and max solutions: difference is " << diff << endl;

      for (auto& o : minMax)
      {
         if (o.GetSolutionFound())
         {
            memset(dArr, 0, sizeof(dArr));
            temp = 0;
            int id = o.GetID();

            status = o.GetData("Library Size", &temp);
            dArr[0] = temp;
            temp = 0;

            status = o.GetData("Solution Size", &temp);
            dArr[1] = temp;
            temp = 0;

            status = o.GetData("TotalDuration", &temp);
            dArr[2] = temp;
            temp = 0;

            solveTime = GTimer::GetTimeStringSeconds(o.GetSolveTime());

            status = o.GetData("Cost", &temp);
            dArr[3] = temp;
            temp = 0;

            dLog << id << " " << dArr[0] << " " << dArr[1] << " " \
               << dArr[2] << " " << solveTime << " " << dArr[3];

            vector<int> locs = o.GetLocations();
            for (auto i : locs)
            {
               dLog << " " << i;
            }
            dLog << endl;

            if (!status)
            {
               cout << "### Error create datafile... ###";
               goto end;
            }
         }
         else
         {
            dLog << "# Min or Max solution not found... \n";
         }
         
      }
      

      dLog << endl;

      // Print out all solutions
      if (concatOutputs.size() > 0)
      {
         for (auto& o : concatOutputs)
         {
            memset(dArr, 0, sizeof(dArr));
            temp = 0;
            int id = o.GetID();

            status = o.GetData("Library Size", &temp);
            dArr[0] = temp;
            temp = 0;

            status = o.GetData("Solution Size", &temp);
            dArr[1] = temp;
            temp = 0;

            status = o.GetData("TotalDuration", &temp);
            dArr[2] = temp;
            temp = 0;

            solveTime = GTimer::GetTimeStringSeconds(o.GetSolveTime());

            status = o.GetData("Cost", &temp);
            dArr[3] = temp;
            temp = 0;

            dArr[4] = GetDifference(minMax[0], o);
            dArr[5] = GetDifference(o, minMax[1]);

            dLog << id << " " << dArr[0] << " " << dArr[1] << " " \
               << dArr[2] << " " << solveTime << " " << dArr[3] \
               << " " << dArr[4] << " " << dArr[5];

            vector<int> locs = o.GetLocations();
            for (auto i : locs)
            {
               dLog << " " << i;
            }
            dLog << endl;

            if (!status)
            {
               cout << "### Error create datafile... ###";
               goto end;
            }
         }
      }
      else
      {
         dLog << "# No solutions not found... \n";
      }
   }

   // Auto generated .p (Gnuplot) script
   {
      //string scriptfilename = CreateScriptfileName(filename);
      //Logger sLog(scriptfilename.c_str());

      //sLog << "# Auto-generated Gnuplot script file: " << scriptfilename;
      //sLog << " to accompany " << datafilename << endl;
      //sLog << "set autoscale" << "\t\t # scale axes automatically\n";
      //sLog << "unset log" << "\t\t # remove any log-scaling\n";
      //sLog << "unset label" << "\t\t # remove any previous labels\n";
      //sLog << "set xtic auto" << "\t\t # set xtics automatically\n";
      //sLog << "set ytic auto" << "\t\t # set ytics automatically\n";
      //sLog << "set title \"Force Deflection Data for a Beam and a Column\"\n";
      //sLog << "set xlabel \"Deflection (meters)\"\n";
      //sLog << "set ylabel \"Force(kN)\"\n";
      //sLog << "set key 0.01, 100\n";
      //sLog << "set label \"Yield Point\" at 0.003, 260\n";
      //sLog << "set arrow from 0.0028, 250 to 0.003, 280\n";
      //sLog << "set xr[0.0:0.022]\n";
      //sLog << "set yr[0:325]\n";
      //sLog << "plot\t\"" << datafilename << "\" using 1:2 title 'Column' with linespoints, \\" \
            //   << "\n\t\t\"" << datafilename << "\" using 1:3 title 'Beam' with points";
   }

end:

   return;
}

string GFileGenerator::CreateDatafileName(string s)
{
   string outputFolder;
   char buffer[MAX_GPATH];
   GetCurrentDirectory(MAX_GPATH, buffer);

   stringstream ss;
   ss.str(string());
   ss << buffer << OUTPUT_FOLDER_PATH << s << DATAFILE_SUFFIX;
   return ss.str();
}

string GFileGenerator::CreateScriptfileName(string s)
{
   string outputFolder;
   char buffer[MAX_GPATH];
   GetCurrentDirectory(MAX_GPATH, buffer);

   stringstream ss;
   ss.str(string());
   ss << buffer << OUTPUT_FOLDER_PATH << s << SCRIPTFILE_SUFFIX;
   return ss.str();
}

void GFileGenerator::FindMinMaxOutputs(vector<GOutput> outputs,
   GOutput& min,
   GOutput& max,
   int& diff)
{
   vector<GOutput*> minOutputs;
   vector<GOutput*> maxOutputs;
   int minSize = INT_MAX;
   int maxSize = INT_MIN;
   int temp = 0;
   int d = 0;
   min.Clear();
   max.Clear();

   // Find min and max sizes
   for (auto& o : outputs)
   {
      if (o.GetData("Solution Size", &temp))
      {
         if (temp > maxSize)
            maxSize = temp;
         if (temp < minSize)
            minSize = temp;

         temp = 0;
      }
   }

   // Collect all min and max size solutions
   for (auto& o : outputs)
   {
      if (o.GetData("Solution Size", &temp))
      {
         if (temp == minSize)
         {
            minOutputs.push_back(&o);
         }
         else if (temp == maxSize)
         {
            maxOutputs.push_back(&o);
         }

         temp = 0;
      }
   }

   for (auto& i : minOutputs)
   {
      for (auto& j : maxOutputs)
      {
         temp = GetDifference(*i, *j);

         if (temp > d)
         {
            d = temp;
            min = *i;
            max = *j;
         }
      }
   }

   diff = d;
}

int GFileGenerator::GetDifference(GOutput& a, GOutput& b)
{
   vector<int> va;
   vector<int> vb;

   if (a.GetData("Solution", &va))
   {
      if (b.GetData("Solution", &vb))
      {
         sort(va.begin(), va.end());
         sort(vb.begin(), vb.end());

         vector<int> v;

         // B - A
         set_difference(vb.begin(), vb.end(),
            va.begin(), va.end(),
            back_inserter(v));

         return v.size();
      }
   }

   return 0;
}
