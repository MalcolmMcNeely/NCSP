using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Markup;

namespace NCSPgui
{
   public static class NCSPExtensions
   {
      /// <summary>
      /// Ensures fast fail on task exceptions
      /// </summary>
      public static Task FailFastOnException(this Task t)
      {
         t.ContinueWith(c => Environment.FailFast("Task faulted", c.Exception),
              TaskContinuationOptions.OnlyOnFaulted |
              TaskContinuationOptions.ExecuteSynchronously);
         return t;
      }

      public static Task<int> FailFastOnException(this Task<int> t)
      {
         t.ContinueWith(c => Environment.FailFast("Task faulted", c.Exception),
              TaskContinuationOptions.OnlyOnFaulted |
              TaskContinuationOptions.ExecuteSynchronously);
         return t;
      }

      /// <summary>
      /// Extension for SmartCollections to resize themselves
      /// </summary>
      /// <returns>false if resizing is not possible, true if resizing was done</returns>
      public static bool Resize(this SmartCollection<int> c, int newSize)
      {
         int currentSize = c.Count;

         if (newSize >= 0 && currentSize != newSize)
         {
            int diff = newSize - currentSize;
            if (diff > 0)
            {
               // Add items
               var items = new List<int>();
               for (int i = 0; i < diff; i++)
               {
                  items.Add(new int());
               }

               c.AddRange(items);
               return true;
            }
            else
            {
               // Remove items
               c.DownsizeCollection(Math.Abs(diff));
               return true;
            }
         }

         return false;
      }

      /// <summary>
      /// Extension for Model to convert it's config settings to a data structure
      /// suitable for interoperability
      /// </summary>
      /// <returns>PARAMS</returns>
      public static PARAMS ToParams(this Model model)
      {
         PARAMS output = new PARAMS();
         output.ViewerType = (int)model.ViewerType;
         output.NumSegments = model.NumSegments;
         output.Duration = model.Duration;
         output.DurationBound = model.DurationBound;
         output.CharacterAppearanceRate = model.CharacterAppearanceRate;
         output.Variance = model.Variance;
         output.NumVariants = model.NumVariants;
         output.LibraryMultiplier = model.LibraryMultiplier;
         output.Timeout = model.Timeout;
         output.PrintLibraryDetails = Convert.ToInt32(model.PrintLibraryDetails);
         return output;
      }
   }

   // Depreciated - kept for future reference for how to set up a wpf value converter
   [MarkupExtensionReturnType(typeof(IValueConverter))]
   public class CharacterMoodToIntConverter : MarkupExtension, IValueConverter
   {
      private static CharacterMoodToIntConverter converter_;
      public override object ProvideValue(IServiceProvider serviceProvider)
      {
         if (converter_ == null)
         {
            converter_ = new CharacterMoodToIntConverter();
         }
         return converter_;
      }

      #region IValueConverter Overrides

      public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
      {
         if (value == null)
         {
            return Binding.DoNothing;
         }

         //switch((eCharMood)value)
         //{
         //   case eCharMood.Any:
         //      return 0;
         //   case eCharMood.Negative:
         //      return -1;
         //   case eCharMood.Positive:
         //      return 1;
         //   default: 
         //      break;
         //}

         return null;
      }

      public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
      {
         if (value != null)
         {
            //switch((int)value)
            //{
            //   case 0:
            //      return eCharMood.Any;
            //   case -1:
            //      return eCharMood.Negative;
            //   case 1:
            //      return eCharMood.Positive;
            //   default: 
            //      break;

            //}
         }

         return null;
      }

      #endregion
   }
}
