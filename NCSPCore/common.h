// 
// General Gecode includes and global options (not good to have these globally, but it is easier)
//
#pragma once

#include <gecode/int.hh>                              // Integer variable constraints
#include <gecode/search.hh>                           // Access seach engine
#include <gecode/gist.hh>

#include <gecode/driver.hh>
#include <gecode/minimodel.hh>
#include <gecode/set.hh>

#include "NarrativeEnums.h"

const int MAXCHARACTERS = eC_SIZE;

// Constraint defines
#define NOSHOTID 30000
#define COSTVARMIN 0
#define COSTVARMAX 120
#define DURATIONVARMIN 0
#define DURATIONVARMAX 1000
#define CHARACTERAPPEARANCEMIN 0
#define CHARACTERAPPEARANCEMAX 100
#define VARIANTDISTANCEMIN 0
#define VARIANTDISTANCEMAX 100

// Filepaths
#define SHOTLIBRARYLOGFILENAME "Library Log.txt"
#define MAX_GPATH 256
#define DATA_FOLDER_PATH "\\Data\\Library Analysis\\"
#define OUTPUT_FOLDER_PATH "\\Output\\"
#define SOLUTIONS_FOLDER_PATH "\\Output\\Solutions\\"
#define SOLUTIONS_FOLDER_DELETE_PATH "\\Output\\Solutions\\*.log"
#define VARIANT_SOLUTIONS_FOLDER_PATH "\\Output\\Solutions\\Variants\\"
#define VARIANT_SOLUTIONS_FOLDER_DELETE_PATH "\\Output\\Solutions\\Variants\\*.log"
#define DATAFILE_SUFFIX ".dat"
#define SCRIPTFILE_SUFFIX ".gp"

// Default configuration
#define DEFAULTVIEWERTYPE 0
#define DEFAULTSEGMENTS 0
#define DEFAULTDURATION 0
#define DEFAULTDURATIONBOUND 0
#define DEFAULTCHARACTERAPPEARANCERATE 0
#define DEFAULTCOST 0
#define DEFAULTCOSTSIZE 0
#define DEFAULTTIMEOUT 0
#define DEFAULTMAXVARAINCE 0
#define DEFAULTPRINTLIBRARY 0
#define DEFAULTWAIT 0
#define DEFAULTTIMERRESOLUTION 2
#define DEFAULTTIMERRESOLUTIONSTRING "ms"
#define DEFAULTHIERARCHSOLUTIONS 1
#define DEFAULTSEGMENTSOLUTIONS 1
#define DEFAULTVARIANTSOLUTIONS 0
#define DEFAULTLIBRARYMULTIPLIER 0
#define DEFAULTSEARCHTIMEOUT 3000

// Misc
#define PIPETIMEOUTTIME 5000

struct TwoInt
{
   TwoInt() {};
   TwoInt(int a, int b) { First = a, Second = b; }

   int First;
   int Second;
};
