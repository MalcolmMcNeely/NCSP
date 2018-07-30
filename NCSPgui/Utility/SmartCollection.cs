using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NCSPgui
{
   /// <summary>
   /// Observable Collection that can add ranges of items and suppress notifications
   /// </summary>
   public class SmartCollection<T> : ObservableCollection<T>
   {
      private bool suppressNotification = false;
      private bool addedItems = false;
      private bool removedItems = false;
      private List<T> modifiedItems = new List<T>();
      private NotifyCollectionChangedEventArgs eventArgs = null;

      public SmartCollection() : base() { }
      public SmartCollection(IEnumerable<T> collection) : base(collection) { }
      public SmartCollection(List<T> list) : base(list) { }

      public void RaisePropertyChangedEvent()
      {
         this.OnPropertyChanged(new PropertyChangedEventArgs("Count"));
         this.OnPropertyChanged(new PropertyChangedEventArgs("Item[]"));
         this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(
               NotifyCollectionChangedAction.Reset));
      }

      public void AddRange(IEnumerable<T> range)
      {
         if (range == null)
         {
            throw new ArgumentNullException("range is null");
         }

         BeginEdit();
         addedItems = true;
         foreach (var item in range)
         {
            modifiedItems.Add(item);
            Items.Add(item);
         }
         EndEdit();
      }

      public void DownsizeCollection(int numItems)
      {
         if(numItems <= 0)
         {
            throw new ArgumentException("Can not remove 0 or less items from SmartCollection<T>");
         }

         BeginEdit();
         removedItems = true;
         for(int i = 0; i < numItems; i++)
         {
            modifiedItems.Add(this[Count - 1]);
            this.RemoveAt(this.Count - 1);
         }
         EndEdit();
      }

      public void Reset(IEnumerable<T> range)
      {
         this.Items.Clear();
         AddRange(range);
      }

      public void BeginEdit() 
      { 
         suppressNotification = true;
         modifiedItems.Clear();
         addedItems = false;
         removedItems = false;
         eventArgs = null;
      }

      public void EndEdit()
      { 
         suppressNotification = false;
         this.OnPropertyChanged(new PropertyChangedEventArgs("Count"));
         this.OnPropertyChanged(new PropertyChangedEventArgs("Item[]"));

         // Fire appropriate modification event; reset by default
         if(addedItems)
         {
            eventArgs = new NotifyCollectionChangedEventArgs(
               NotifyCollectionChangedAction.Add, 
               modifiedItems);
         }
         else if(removedItems)
         {
            eventArgs = new NotifyCollectionChangedEventArgs(
               NotifyCollectionChangedAction.Remove,
               modifiedItems);
         }

         if(eventArgs != null)
         {
            this.OnCollectionChanged(eventArgs);
         }
         else
         {
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(
               NotifyCollectionChangedAction.Reset));
         }
      }

      protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
      {
         if (!suppressNotification)
         {
            try
            {
               base.OnCollectionChanged(e);
            }
            catch (System.NotSupportedException)
            {
               // Workaround: this is the default behaviour of observable collection being
               // unable to deal with ranges. Problem resides in WPF framework.
               NotifyCollectionChangedEventArgs alternativeEventArgs = 
                  new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset);
               OnCollectionChanged(alternativeEventArgs);
            }            
         }
      }

      protected override void OnPropertyChanged(PropertyChangedEventArgs e)
      {
         base.OnPropertyChanged(e);
      }
   }
}