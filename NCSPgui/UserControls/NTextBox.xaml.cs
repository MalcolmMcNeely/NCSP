using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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

namespace MControls
{
   /// <summary>
   /// TextBox UserControl specialised for (integer) numeric input
   /// </summary>
   public partial class NTextBox : UserControl
   {
      public TextBox NumberBox { get; set; }

      public NTextBox()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
         NumberBox = this.TextBox;
      }

      public int NValue
      {
         get { return (int)GetValue(NValueProperty); }
         set { SetValue(NValueProperty, value); }
      }

      public static readonly DependencyProperty NValueProperty =
          DependencyProperty.Register("NValue", typeof(int),
            typeof(NTextBox), new PropertyMetadata(0));
   }
}
