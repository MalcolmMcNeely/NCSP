//
//	Enumeration values for solver variables (some of these 
// may be depreciated)
//
#pragma once

enum eSolverOption
{
   eOpt_Error,
   eOpt_SolutionSize,
   eOpt_Duration,
   eOpt_DurationBound,
   eOpt_ViewerType,
   eOpt_MinCost,
   eOpt_MainCharacter,
   eOpt_MainCharAppearanceRate,
   eOpt_VariantDistance,
   eOpt_MinMaxSolution
};

enum eBranchingBehaviour
{
   eBB_Min,
   eBB_Max
};

enum eSolutionType
{
   eSType_Normal = 0,
   eSType_Min = 1,
   eSType_Max = 2
};