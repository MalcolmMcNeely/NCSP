using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows;
using MControls;

namespace NCSPgui
{
   public partial class ViewModel : INotifyPropertyChanged
   {
      public ViewModel()
      {
         Intialize();
         AttachEvents();
      }

      private void Intialize()
      {
         NCSPInitialise();
      }

      private Model _model = new Model();
      public Model Model
      {
         get { return _model; }
         set
         {
            if(_model != value)
            {
               if (_model != null) DetachEvents();
               _model = value;
               if(_model != null) AttachEvents();               
               OnPropertyChanged(String.Empty);
            }
         }
      }

      private NamedPipeServer hPipe_ = null;
      public NamedPipeServer HPipe
      {
         get { return hPipe_; }
         set { hPipe_ = value; }
      }

      #region Events

      private void AttachEvents()
      {
         Model.PropertyChanged += OnModelPropertyChanged;         
      }

      private void DetachEvents()
      {
         Model.PropertyChanged -= OnModelPropertyChanged;
      }

      private void OnModelPropertyChanged(object sender, PropertyChangedEventArgs e)
      {
         switch(e.PropertyName)
         {
            // Special cases in future
            default:
               break;
         }

         OnPropertyChanged(e.PropertyName);
      }

      public void OnWindowClosing(object sender, CancelEventArgs e)
      {
         // Unregister from all events
         DetachEvents();
         Model.Dispose();
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
