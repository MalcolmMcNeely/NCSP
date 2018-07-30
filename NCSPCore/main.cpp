//
// main.cpp
//
#include "common.h"
#include "NCSPEntry.h"
#include "CmdLnData.h"
#include "NCSPCmdLnArgs.h"
#include "Utility.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace Utility;

// Copy/paste test debug options
// -V 0 -S 1 -D 30 -B 10 -R 70 -C="5 0" -I 1 -J 1 -K 5 -M 2 -L 1 -T 10000 -W
int main(int argc, char* argv[])
{
   int error = 0;

   CmdLnData cmdData;
   cmdData.SetDefaults();

   argc -= (argc > 0); argv += (argc > 0); // skip program name argv[0] if present
   option::Stats stats(usage, argc, argv);

#ifdef __GNUC__
   // GCC supports C99 VLAs for C++ with proper constructor calls.
   option::Option options[stats.options_max], buffer[stats.buffer_max];
#else
   // use calloc() to allocate 0-initialized memory. It's not the same
   // as properly constructed elements, but good enough. Obviously in an
   // ordinary C++ program you'd use new[], but this file demonstrates that
   // TLMC++OP can be used without any dependency on the C++ standard library.
   option::Option* options = (option::Option*)calloc(stats.options_max, sizeof(option::Option));
   option::Option* buffer = (option::Option*)calloc(stats.buffer_max, sizeof(option::Option));
#endif

   option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

   if (parse.error())
   {
      cout << "Error parsing command line arguments" << endl;
      return 1;
   }

   if (options[HELP] || argc == 0)
   {
      int columns = getenv("COLUMNS") ? atoi(getenv("COLUMNS")) : 80;
      option::printUsage(fwrite, stdout, usage, columns);
      return 0;
   }

   // Handle command line arguments
   for (int i = 0; i < parse.optionsCount(); ++i)
   {
      option::Option& opt = buffer[i];
      fprintf(stdout, "\nArgument #%d is ", i);
      switch (opt.index())
      {
      case HELP:
         // not possible, because handled further above and exits the program
      case VIEWERTYPE:
         fprintf(stdout, "--ViewerType with argument '%s'", opt.arg);
         cmdData.SetViewerType(std::atoi(opt.arg));
         break;
      case SEGMENTS:
         fprintf(stdout, "--Segment with argument '%s'", opt.arg);
         cmdData.SetSegments(std::atoi(opt.arg));
         break;
      case DURATION:
         fprintf(stdout, "--Duration with argument '%s'", opt.arg);
         cmdData.SetDuration(std::atoi(opt.arg));
         break;
      case DURATIONBOUND:
         fprintf(stdout, "--DurationBound with argument '%s'", opt.arg);
         cmdData.SetDurationBound(std::atoi(opt.arg));
         break;
      case CHARACTERAPPEARANCERATE:
         fprintf(stdout, "--CharacterAppearanceRate with argument '%s'", opt.arg);
         cmdData.SetCharacterAppearanceRate(std::atoi(opt.arg));
         break;
      case MINCOSTS:
      {
         fprintf(stdout, "--MinCosts with argument '%s'", opt.arg);
         std::vector<int> values;

         stringstream ss(opt.arg);
         char invalidChars[] = "=";
         string delimiter = " ";

         string s = ss.str();
         size_t pos = 0;
         string token;
         size_t sz;

         // Remove invalid characters
         for (unsigned int i = 0; i < strlen(invalidChars); ++i)
         {
            s.erase(remove(s.begin(), s.end(), invalidChars[i]), s.end());
         }

         // Parse characters to integers
         while ((pos = s.find(delimiter)) != string::npos)
         {
            token = s.substr(0, pos);
            int i = stoi(token, &sz);
            values.push_back(i);
            s.erase(0, pos + delimiter.length());
         }

         cmdData.SetMinCosts(values);
         break;
      }
      case TIMEOUT:
         fprintf(stdout, "--Timeout with argument '%s'", opt.arg);
         cmdData.SetTimeout(std::atoi(opt.arg));
         break;
      case MAXIMUMVARIANCE:
         fprintf(stdout, "--MaximumVariance with argument '%s'", opt.arg);
         cmdData.SetMaxVariance(std::atoi(opt.arg));
         break;
      case PRINTLIBRARY:
         cmdData.SetPrintLibrary(true);
         break;
      case WAIT:
         cmdData.SetWait(true);
         break;
      case HIERARCHSOLUTIONS:
         fprintf(stdout, "--HierarchSolutions with argument '%s'", opt.arg);
         cmdData.SetHierarchSolutions(std::atoi(opt.arg));
         break;
      case SEGMENTSOLUTIONS:
         fprintf(stdout, "--SegmentSolutions with argument '%s'", opt.arg);
         cmdData.SetSegmentSolutions(std::atoi(opt.arg));
         break;
      case VARIANTSOLUTIONS:
         fprintf(stdout, "--VariantSolutions with argument '%s'", opt.arg);
         cmdData.SetVariantSolutions(std::atoi(opt.arg));
         break;
      case LIBMULTIPLIER:
         fprintf(stdout, "--LibraryMultiplier with argument '%s'", opt.arg);
         cmdData.SetLibraryMultiplier(std::atoi(opt.arg));
         break;
      case UNKNOWN:
         // not possible because Arg::Unknown returns ARG_ILLEGAL
         // which aborts the parse with an error
         break;
      }
   }

   fprintf(stdout, "\n");

   for (int i = 0; i < parse.nonOptionsCount(); ++i)
   {
      fprintf(stdout, "Non-option argument #%d is %s", i, parse.nonOption(i));
   }

   // Validation
   if (cmdData.GetMinCosts().size() != cmdData.GetSegments())
   {
      OutputText("Min Costs is not the same as Segments");
      system("pause");
      return 1;
   }

   if (cmdData.GetWait())
   {
      system("pause");
   }

	// Entry into NCSP
   error = GMain(cmdData);

   if (cmdData.GetWait())
   {
      system("pause");
   }

   return error;
}
