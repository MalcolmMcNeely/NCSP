using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NCSPgui
{
   public class ShotSolution  : INotifyPropertyChanged
   {
      public ShotSolution() { }

      private List<Shot> shots;
      public List<Shot> Shots
      {
         get { return shots; }
         set
         {
            shots = value;
            OnPropertyChanged("Shots");
         }
      }

      private String name;
      public String Name
      {
         get { return name; }
         set
         {
            if(name != value)
            {
               name = value;
               OnPropertyChanged("Name");
            }
         }
      }

      #region Implementation of INotifyPropertyChanged

      public event PropertyChangedEventHandler PropertyChanged;

      protected virtual void OnPropertyChanged(string propertyName)
      {
         PropertyChangedEventHandler handler = PropertyChanged;
         if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
      }

      #endregion
   }

   public class Shot
   {
      public Shot() {}
      public Shot(Shot other)
      {
         info = other.info;
         characters = other.characters;
         fileLocation = other.fileLocation;
      }
      
      private SHOTINFO info;
      public SHOTINFO Info
      {
         get { return info; }
         set { info = value; }
      }

      private SmartCollection<String> characters;
      public SmartCollection<String> Characters
      {
         get { return characters; }
         set { characters = value; }
      }

      private String fileLocation;
      public String FileLocation
      {
         get { return fileLocation; }
         set { fileLocation = value; }
      }
      
      public int ShotId
      {
         get { return Info.ID; }
      }

      public string ShotName 
      {
         get { return Info.ID.ToString(); }
      }      

      public int TimeDuration
      {
         get { return Info.Duration; }
      }

      public override string ToString()
      {
         String s = Convert.ToString(Info.ID);
         return s;
      }
   }
}
