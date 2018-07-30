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

namespace MControls
{
   /// <summary>
   /// Singular representation of a shot for use with ShotPlaybackControl
   /// </summary>
   public partial class ShotPlaybackSegment : UserControl
   {
      public ShotPlaybackSegment()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
      }

      public int ShotIdentifier
      {
         get { return (int)GetValue(ShotIdentifierProperty); }
         set { SetValue(ShotIdentifierProperty, value); }
      }

      public static readonly DependencyProperty ShotIdentifierProperty =
         DependencyProperty.Register("ShotIdentifier", typeof(int),
            typeof(ShotPlaybackSegment));

      public String ShotNameDisplayed
      {
         get { return (String)GetValue(ShotNameDisplayedProperty); }
         set { SetValue(ShotNameDisplayedProperty, value); }
      }

      public static readonly DependencyProperty ShotNameDisplayedProperty =
         DependencyProperty.Register("ShotNameDisplayed", typeof(String),
            typeof(ShotPlaybackSegment), new PropertyMetadata("Shot Name Display"));

      #region Events

      private void LayoutRoot_MouseDown(object sender, MouseButtonEventArgs e)
      {
         RoutedEventArgs args = new RoutedEventArgs(ShotPlaybackSegment.SelectedEvent, this);
         RaiseEvent(args);
      }

      public static readonly RoutedEvent SelectedEvent = 
          EventManager.RegisterRoutedEvent("Selected", RoutingStrategy.Bubble, 
          typeof(RoutedEventHandler), typeof(ShotPlaybackSegment));
 
      // .NET Wrapper
      public event RoutedEventHandler Selected
      {
          add { AddHandler(SelectedEvent, value); } 
          remove { RemoveHandler(SelectedEvent, value); }
      }

      #endregion
   }
}
