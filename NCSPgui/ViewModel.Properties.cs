using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NCSPgui
{
   public partial class ViewModel
   {
      public void SetDefaultProperties()
      {
         ViewerType = eViewerType.Thrill;
         NumSegments = 2;
         Duration = 30;
         DurationBound = 10;
         CharacterAppearanceRate = 70;
         MinimumCosts[0] = 0;
         MinimumCosts[1] = 5;
         Variance = 2;
         NumVariants = 3;
         LibraryMultiplier = 1;
         Timeout = 5000;
         PrintLibraryDetails = false;
      }

      #region Model Properties

      public IList<eViewerType> ViewerTypes
      {
         get
         {
            return Enum.GetValues(typeof(eViewerType)).Cast<eViewerType>().ToList<eViewerType>();
         }
      }

      public eViewerType ViewerType
      {
         get { return Model.ViewerType; }
         set
         {
            if(Model.ViewerType != value)
            {
               Model.ViewerType = value;
            }
         }
      }

      public int NumSegments
      {
         get { return Model.NumSegments; }
         set
         {
            if(Model.NumSegments != value)
            {
               if (Model.MinimumCosts.Resize(value))
               {
                  Model.NumSegments = value;
               }
            }
         }
      }

      public int Duration
      {
         get { return Model.Duration; }
         set
         {
            if(Model.Duration != value)
            {
               Model.Duration = value;
            }
         }
      }

      public int DurationBound
      {
         get { return Model.DurationBound; }
         set
         {
            if(Model.DurationBound != value)
            {
               Model.DurationBound = value;
            }
         }
      }

      public int CharacterAppearanceRate
      {
         get { return Model.CharacterAppearanceRate; }
         set
         {
            if(Model.CharacterAppearanceRate != value)
            {
               Model.CharacterAppearanceRate = value;
            }
         }
      }

      public SmartCollection<int> MinimumCosts
      {
         get { return Model.MinimumCosts; }
         set
         {
            if(Model.MinimumCosts != value)
            {
               Model.MinimumCosts = value;
            }
         }
      }

      public int Variance
      {
         get { return Model.Variance; }
         set 
         {
            if(Model.Variance != value)
            {
               Model.Variance = value;
            }
         }
      }

      public int NumVariants
      {
         get { return Model.NumVariants; }
         set
         {
            if(Model.NumVariants != value)
            {
               Model.NumVariants = value;
            }
         }
      }

      public int LibraryMultiplier
      {
         get { return Model.LibraryMultiplier; }
         set
         {
            if(Model.LibraryMultiplier != value)
            {
               Model.LibraryMultiplier = value;
            }
         }
      }

      public int Timeout
      {
         get { return Model.Timeout; }
         set
         {
            if(Model.Timeout != value)
            {
               Model.Timeout = value;
            }
         }
      }

      public bool PrintLibraryDetails
      {
         get { return Model.PrintLibraryDetails; }
         set
         {
            if(Model.PrintLibraryDetails != value)
            {
               Model.PrintLibraryDetails = value;
            }
         }
      }

      public SmartCollection<ShotSolution> Solution
      {
         get { return Model.Solution; }
         set
         {
            Model.Solution = value;
         }
      }

      public String MainCharacterName
      {
         get { return Model.MainCharacterName; }
         set
         {
            if(Model.MainCharacterName != value)
            {
               Model.MainCharacterName = value;
            }
         }
      }

      #endregion

      private bool runButtonEnabled = true;
      public bool RunButtonEnabled
      {
         get { return runButtonEnabled; }
         set
         {
            if(runButtonEnabled != value)
            {
               runButtonEnabled = value;
               OnPropertyChanged("RunButtonEnabled");
            }
         }
      }

      private string debugOutput = String.Empty;
      public string DebugOutput
      {
         get { return debugOutput; }
         set 
         {
            if (debugOutput != value)
            {
               debugOutput = value;
               OnPropertyChanged("DebugOutput");
            }
         }
      }

      private int shotCollectionSelectedId;
      public int ShotCollectionSelectedId
      {
         get { return shotCollectionSelectedId; }
         set 
         { 
            if(shotCollectionSelectedId != value)
            {
               shotCollectionSelectedId = value;
               OnPropertyChanged("ShotCollectionSelectedId");
            }
         }
      }

   }
}
