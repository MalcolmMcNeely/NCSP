using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace NCSPgui
{
   public class Model : INotifyPropertyChanged
   {
      public Model()
      {
         AttachEvents();
      }

      public void Dispose()
      {
         DetachEvents();
      }

      private eViewerType viewerType;
      public eViewerType ViewerType
      {
         get { return viewerType; }
         set 
         { 
            viewerType = value;
            OnPropertyChanged("ViewerType");
         }
      }

      private int numSegments;
      public int NumSegments
      {
         get { return numSegments; }
         set 
         { 
            numSegments = value;
            OnPropertyChanged("NumSegments");
         }
      }

      private int duration;
      public int Duration
      {
         get { return duration; }
         set 
         { 
            duration = value;
            OnPropertyChanged("Duration");
         }
      }

      private int durationBound;
      public int DurationBound
      {
         get { return durationBound; }
         set 
         { 
            durationBound = value;
            OnPropertyChanged("DurationBound");
         }
      }

      private int characterAppearanceRate;
      public int CharacterAppearanceRate
      {
         get { return characterAppearanceRate; }
         set 
         { 
            characterAppearanceRate = value;
            OnPropertyChanged("CharacterAppearanceRate");
         }
      }

      private SmartCollection<int> minimumCosts = 
         new SmartCollection<int>();
      public SmartCollection<int> MinimumCosts
      {
         get { return minimumCosts; }
         set
         {
            minimumCosts = value;
            OnPropertyChanged("MinimumCosts");
         }
      }

      private int variance;
      public int Variance
      {
         get { return variance; }
         set
         {
            variance = value;
            OnPropertyChanged("Variance");
         }
      }

      private int numVariants;
      public int NumVariants
      {
         get { return numVariants; }
         set
         {
            numVariants = value;
            OnPropertyChanged("NumVariants");
         }
      }

      private int libraryMultiplier;
      public int LibraryMultiplier
      {
         get { return libraryMultiplier; }
         set 
         { 
            libraryMultiplier = value;
            OnPropertyChanged("LibraryMultiplier");
         }
      }

      private int timeout;
      public int Timeout
      {
         get { return timeout; }
         set
         {
            timeout = value;
            OnPropertyChanged("Timeout");
         }
      }

      private bool printLibraryDetails;
      public bool PrintLibraryDetails
      {
         get { return printLibraryDetails; }
         set
         {
            printLibraryDetails = value;
            OnPropertyChanged("PrintLibraryDetails");
         }
      }

      private SmartCollection<ShotSolution> solution = new SmartCollection<ShotSolution>();
      public SmartCollection<ShotSolution> Solution
      {
         get { return solution; }
         set
         {
            solution = value;
            OnPropertyChanged("Solution");
         }
      }

      private String mainCharacterName;
      public String MainCharacterName
      {
         get { return mainCharacterName; }
         set
         {
            if(mainCharacterName != value)
            {
               mainCharacterName = value;
               OnPropertyChanged("MainCharacterName");
            }
         }
      }

      #region Events

      private void AttachEvents()
      {
         minimumCosts.CollectionChanged += minimumCosts_CollectionChanged;}

      void minimumCosts_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
      {
         OnPropertyChanged("MinimumCosts");
      }

      private void DetachEvents()
      {
         minimumCosts.CollectionChanged -= minimumCosts_CollectionChanged;
      }

      #endregion

      #region Implementation of INotifyPropertyChanged

      public event PropertyChangedEventHandler PropertyChanged;

      protected virtual void OnPropertyChanged(string propertyName)
      {
         PropertyChangedEventHandler handler = PropertyChanged;
         if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
      }

      #endregion
   }
}
