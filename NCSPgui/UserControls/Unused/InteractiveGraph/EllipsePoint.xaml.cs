using NCSPgui;
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
   /// UserControl for interactive point for use in InteractiveLineGraph
   /// </summary>
   public partial class EllipsePoint : UserControl
   {
      public EllipsePoint()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
         this.IsHitTestVisible = true;
      }

      #region Dependency properties

      public InteractiveLineGraph ParentGraph
      {
         get { return (InteractiveLineGraph)GetValue(ParentGraphProperty); }
         set { SetValue(ParentGraphProperty, value); }
      }

      public static readonly DependencyProperty ParentGraphProperty =
          DependencyProperty.Register("ParentGraph", typeof(InteractiveLineGraph),
            typeof(EllipsePoint), new PropertyMetadata(null));

      public SmartCollection<int> DataCollection
      {
         get { return (SmartCollection<int>)GetValue(DataCollectionProperty); }
         set { SetValue(DataCollectionProperty, value); }
      }

      public static readonly DependencyProperty DataCollectionProperty =
         DependencyProperty.Register("DataCollection", typeof(SmartCollection<int>),
            typeof(EllipsePoint), new PropertyMetadata(null));

      public int DataIndex
      {
         get { return (int)GetValue(DataIndexProperty); }
         set { SetValue(DataIndexProperty, value); }
      }

      public static readonly DependencyProperty DataIndexProperty =
         DependencyProperty.Register("DataIndex", typeof(int),
            typeof(EllipsePoint), new PropertyMetadata(0));

      public bool IsDragging
      {
         get { return (bool)GetValue(IsDraggingProperty); }
         set { SetValue(IsDraggingProperty, value); }
      }

      public static readonly DependencyProperty IsDraggingProperty =
         DependencyProperty.Register("IsDragging", typeof(bool),
            typeof(EllipsePoint), new PropertyMetadata(false));

      public double DragToX
      {
         get { return (double)GetValue(DragToXProperty); }
         set { SetValue(DragToXProperty, value); }
      }

      public static readonly DependencyProperty DragToXProperty =
         DependencyProperty.Register("DragToX", typeof(double),
            typeof(EllipsePoint), new PropertyMetadata(0.0D));

      public double DragToY
      {
         get { return (double)GetValue(DragToYProperty); }
         set { SetValue(DragToYProperty, value); }
      }

      public static readonly DependencyProperty DragToYProperty =
         DependencyProperty.Register("DragToY", typeof(double),
            typeof(EllipsePoint), new PropertyMetadata(0.0D));

      #endregion

      public Point? GetNormalizedPosition()
      {
         if (ParentGraph != null)
         {
            if (ParentGraph.MainCanvas != null)
            {
               Point p = ParentGraph.NormalizePointToScreen(new Point(DataIndex, DataCollection[DataIndex]));
               p.X = p.X - thumb.Width / 2;
               p.Y = p.Y - thumb.Height / 2;
               return p;
            }
         }

         return null;
      }

      #region Events

      public event EventHandler<MouseButtonEventArgs> EPMouseLeftDown;
      private void OnMouseLeftDown(object sender, MouseButtonEventArgs e)
      {
         var handler = EPMouseLeftDown;
         if (handler != null) handler(this, e);
      }

      public event EventHandler<MouseButtonEventArgs> EPMouseLeftUp;
      private void OnMouseLeftUp(object sender, MouseButtonEventArgs e)
      {
         var handler = EPMouseLeftUp;
         if (handler != null) handler(this, e);
      }

      public event EventHandler<MouseEventArgs> EPMouseMove;
      private void OnMouseMove(object sender, MouseEventArgs e)
      {
         var handler = EPMouseMove;
         if (handler != null) handler(this, e);
      }

      #endregion
   }
}
