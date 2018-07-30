using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace MHelper
{
   public static class MHelperMethods
   {
      /// <summary>
      /// Function for dependency objects in the visual tree to fine other dependency objects
      /// </summary>
      public static T FindVisualChild<T>(DependencyObject depObj) where T : DependencyObject
      {
         if (depObj != null)
         {
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(depObj); i++)
            {
               DependencyObject child = VisualTreeHelper.GetChild(depObj, i);
               if (child != null && child is T)
               {
                  return (T)child;
               }

               T childItem = FindVisualChild<T>(child);
               if (childItem != null) return childItem;
            }
         }
         return null;
      }
   }

   public class IntPair : DependencyObject
   {
      public int Minimum
      {
         get { return (int)GetValue(MinimumProperty); }
         set { SetValue(MinimumProperty, value); }
      }
      public static readonly DependencyProperty MinimumProperty =
          DependencyProperty.Register("Minimum", typeof(int), typeof(IntPair), new UIPropertyMetadata(int.MinValue));

      public int Maximum
      {
         get { return (int)GetValue(MaximumProperty); }
         set { SetValue(MaximumProperty, value); }
      }
      public static readonly DependencyProperty MaximumProperty =
          DependencyProperty.Register("Maximum", typeof(int), typeof(IntPair), new UIPropertyMetadata(int.MaxValue));

   }

   public class PlaybackShotSegmentBrushColorConverter : IValueConverter
   {
      public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
      {
         if ((bool)value)
         {
            {
               return new SolidColorBrush(Colors.Green);
            }
         }

         return new SolidColorBrush((Color)ColorConverter.ConvertFromString("#cccccc"));
      }

      public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
      {
         throw new NotImplementedException();
      }
   }

   public class ValueToPercentConverter : IMultiValueConverter
   {
      public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
      {
         double value = System.Convert.ToDouble(values[0]);
         double maximum = System.Convert.ToDouble(values[1]);
         return (value / maximum) * 100;
      }

      public object[] ConvertBack(object value, Type[] targetTypes, object parameter, System.Globalization.CultureInfo culture)
      {
         throw new NotImplementedException();
      }
   }

   public class EnumMatchToBooleanConverter : IValueConverter
   {
      public object Convert(object value, Type targetType,
                            object parameter, CultureInfo culture)
      {
         if (value == null || parameter == null)
            return false;

         string checkValue = value.ToString();
         string targetValue = parameter.ToString();
         return checkValue.Equals(targetValue,
                  StringComparison.InvariantCultureIgnoreCase);
      }

      public object ConvertBack(object value, Type targetType,
                                object parameter, CultureInfo culture)
      {
         if (value == null || parameter == null)
            return null;

         bool useValue = (bool)value;
         string targetValue = parameter.ToString();
         if (useValue)
            return Enum.Parse(targetType, targetValue);

         return null;
      }
   }

}
