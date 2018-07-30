#pragma once

#include <stdio.h>
#include "NCSPCmdLnOpt.h"

struct Arg : public option::Arg
{
   static void printError(const char* msg1, const option::Option& opt, const char* msg2)
   {
      fprintf(stderr, "%s", msg1);
      fwrite(opt.name, opt.namelen, 1, stderr);
      fprintf(stderr, "%s", msg2);
   }

   static option::ArgStatus Unknown(const option::Option& option, bool msg)
   {
      if (msg) printError("Unknown option '", option, "'\n");
      return option::ARG_ILLEGAL;
   }

   static option::ArgStatus Required(const option::Option& option, bool msg)
   {
      if (option.arg != 0)
         return option::ARG_OK;

      if (msg) printError("Option '", option, "' requires an argument\n");
      return option::ARG_ILLEGAL;
   }

   static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
   {
      if (option.arg != 0 && option.arg[0] != 0)
         return option::ARG_OK;

      if (msg) printError("Option '", option, "' requires a non-empty argument\n");
      return option::ARG_ILLEGAL;
   }

   static option::ArgStatus Numeric(const option::Option& option, bool msg)
   {
      char* endptr = 0;
      if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
      if (endptr != option.arg && *endptr == 0)
         return option::ARG_OK;

      if (msg) printError("Option '", option, "' requires a numeric argument\n");
      return option::ARG_ILLEGAL;
   }
};

enum  optionIndex 
{ 
   UNKNOWN, 
   HELP, 
   VIEWERTYPE, 
   SEGMENTS, 
   DURATION, 
   DURATIONBOUND,
   CHARACTERAPPEARANCERATE,
   MINCOSTS,
   TIMEOUT,
   MAXIMUMVARIANCE,
   PRINTLIBRARY,
   WAIT,
   HIERARCHSOLUTIONS,
   SEGMENTSOLUTIONS,
   VARIANTSOLUTIONS,
   LIBMULTIPLIER
};

const option::Descriptor usage[] =
{
   { UNKNOWN,                 0, "", "", Arg::Unknown, "USAGE: example_arg [options]\n\n Options:" },
   { HELP,                    0, "", "help", Arg::None, "  --help  \tPrint usage and exit." },
   { VIEWERTYPE,              0, "Vv", "ViewerType", Arg::Numeric, "  --ViewerType, -V= [0(Thrill), 1(Gore)]" },
   { SEGMENTS,                0, "Ss", "Segments", Arg::Numeric, "  --Segments, -S=<arg>" },
   { DURATION,                0, "Dd", "Duration", Arg::Numeric, "  --Duration, -D=<arg>" },
   { DURATIONBOUND,           0, "Bb", "DurationBound", Arg::Numeric, "  --DurationBound, -B=<arg>" },
   { CHARACTERAPPEARANCERATE, 0, "Rr", "CharacterAppearanceRate", Arg::Numeric, "  --CharacterAppearanceRate, -R=<arg>" },
   { MINCOSTS,                0, "Cc", "MinCosts", Arg::Optional, "  --MinCosts, -C=<arg>" },
   { TIMEOUT,                 0, "Tt", "Timeout", Arg::Numeric, "  --Timeout, -T=<arg>" },
   { MAXIMUMVARIANCE,         0, "Mm", "MaximumVariance", Arg::Numeric, "  --MaximumVariance, -M=<arg>" },
   { PRINTLIBRARY,            0, "Pp", "PrintLibrary", Arg::Optional, "  --PrintLibrary, -P" },
   { WAIT,                    0, "Ww", "Wait", Arg::Optional, "  --Wait, -W" },
   { HIERARCHSOLUTIONS,       0, "Ii", "HierarchSolutions", Arg::Numeric, "  --HierarchSolutions, -I=<arg>" },
   { SEGMENTSOLUTIONS,        0, "Jj", "SegmentSolutions", Arg::Numeric, "  --SegmentSolutions, -J=<arg>" },
   { VARIANTSOLUTIONS,        0, "Kk", "VariantSolutions", Arg::Numeric, "  --VariantSolutions, -K=<arg>" },
   { LIBMULTIPLIER,           0, "Ll", "LibraryMultiplier", Arg::Numeric, "  --LibraryMultiplier, -L=<arg>" },
   { UNKNOWN,                 0, "", "", Arg::None, "\nExamples:\n  example --ViewerType=1 \n" },
   { 0, 0, 0, 0, 0, 0 }
};