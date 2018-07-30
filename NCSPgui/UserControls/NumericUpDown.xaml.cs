using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Reflection;
using System.Text.RegularExpressions;

namespace MControls
{
   /// <summary>
   /// NTextBox with arrows for increment and decrement. Example use:
   /// <controls:NumericUpDown Validation.ErrorTemplate="{StaticResource validationErrorTemplate}">
   ///               <controls:NumericUpDown.Value>
   ///                  <Binding Path="Timeout" Mode="TwoWay" UpdateSourceTrigger="PropertyChanged">
   ///                     <Binding.ValidationRules>
   ///                        <controls:PositiveRangeNumberValidationRule>
   ///                           <controls:PositiveRangeNumberValidationRule.ValidRange>
   ///                              <helper:IntPair Minimum="0" Maximum="100000"/>
   ///                           </controls:PositiveRangeNumberValidationRule.ValidRange>
   ///                        </controls:PositiveRangeNumberValidationRule>
   ///                     </Binding.ValidationRules>
   ///                  </Binding>
   ///               </controls:NumericUpDown.Value>
   ///            </controls:NumericUpDown>
   /// </summary>
   public partial class NumericUpDown : UserControl
   {
      private int minvalue = Int32.MinValue, maxvalue = Int32.MaxValue;
      public NTextBox NumericTextBox { set; get; }

      public NumericUpDown()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
         NumericTextBox = this.NumericTextBoxControl;
      }

      public int Value
      {
         get { return (int)GetValue(ValueProperty); }
         set { SetValue(ValueProperty, value); }
      }

      public static readonly DependencyProperty ValueProperty =
          DependencyProperty.Register("Value", typeof(int),
            typeof(NumericUpDown), new PropertyMetadata(0));

      private static bool IsTextAllowed(string text)
      {
         Regex regex = new Regex("^[0-9]*$"); //regex for numbers only
         return !regex.IsMatch(text);
      }

      private void OnNUDButtonUPClick(object sender, RoutedEventArgs e)
      {
         if (Value < maxvalue)
            Value++;
      }

      private void OnNUDButtonDownClick(object sender, RoutedEventArgs e)
      {
         if (Value > minvalue)
            Value--;
      }
   }
}
