using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace NCSPgui
{
   public enum eViewerType
   {
      Thrill = 0,
      Gore = 1
   }

   #region Interop specific structures

   [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
   public struct PARAMS
   {
      public int ViewerType;
      public int NumSegments;
      public int Duration;
      public int DurationBound;
      public int CharacterAppearanceRate;
      public int Variance;
      public int NumVariants;
      public int LibraryMultiplier;
      public int Timeout;
      public int PrintLibraryDetails;
   }
   
   [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
   public struct SHOTINFO
   {
      public int ID;
      public int Cost;
      public int Gore;
      public int Thrill;
      public int Empathy;
      public int Threaten;
      public int Chase;
      public int Hide;
      public int Kill;
      public int Attack;
      public int Escape;
      public int Location;
      public int Duration;
   }

   #endregion
}
