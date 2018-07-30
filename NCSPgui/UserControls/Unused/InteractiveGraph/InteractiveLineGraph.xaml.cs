using NCSPgui;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
/* Custom user control for graph representation of a collection of integers
 * that can be change by moving points on the graph
 * EXAMPLE USAGE:
 *  <controls:InteractiveLineGraph Width="400" 
 *                                 Height="200"
 *                                 GraphHeader="My Header"
 *                                 Data="{Binding SmartCollection<int>, 
 *                                                Mode=TwoWay, 
 *                                                UpdateSourceTrigger=PropertyChanged}"
 *                                 YMaxValue="2"
 *                                 XTick="1"
 *                                 YTick="1"/>
 */

   public partial class InteractiveLineGraph : UserControl
   {
      #region Dependency properties

      public SmartCollection<int> Data
      {
         get { return (SmartCollection<int>)GetValue(DataProperty); }
         set { SetValue(DataProperty, value); }
      }

      public static readonly DependencyProperty DataProperty =
         DependencyProperty.Register("Data", typeof(SmartCollection<int>),
            typeof(InteractiveLineGraph), new FrameworkPropertyMetadata(null, 
               FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

      public int YMaxValue
      {
         get { return (int)GetValue(YMaxProperty); }
         set { SetValue(YMaxProperty, value); }
      }

      public static readonly DependencyProperty YMaxProperty =
         DependencyProperty.Register("YMax", typeof(int),
            typeof(InteractiveLineGraph), new PropertyMetadata(0));

      public int XTick
      {
         get { return (int)GetValue(XTickProperty); }
         set { SetValue(XTickProperty, value); }
      }

      public static readonly DependencyProperty XTickProperty =
         DependencyProperty.Register("XTick", typeof(int),
            typeof(InteractiveLineGraph), new PropertyMetadata(0));

      public int YTick
      {
         get { return (int)GetValue(YTickProperty); }
         set { SetValue(YTickProperty, value); }
      }

      public static readonly DependencyProperty YTickProperty =
         DependencyProperty.Register("YTick", typeof(int),
            typeof(InteractiveLineGraph), new PropertyMetadata(0));

      public String GraphHeader
      {
         get { return (String)GetValue(GraphHeaderProperty); }
         set { SetValue(GraphHeaderProperty, value); }
      }

      public static readonly DependencyProperty GraphHeaderProperty =
         DependencyProperty.Register("GraphHeader", typeof(String),
            typeof(TransferListBox), new PropertyMetadata("Graph Header"));

      #endregion

      #region Variables and properties

      public Canvas MainCanvas
      {
         get { return ChartCanvas; }
      }

      private List<Point> dataPoints_ = new List<Point>();
      private List<EllipsePoint> EllipsePoints = new List<EllipsePoint>();
      
      private double ChartWidth;
      private double ChartHeight;

      // Data range values
      private double Xmin;
      private double Xmax;
      private double Ymin;
      private double Ymax { get { return YMaxValue + 0.5; } } // + padding

      // Ellipse drawing variables
      private Point originalElementPosition;
      private Point elementPosition;
      private Point mousePosition;

      private List<double> validYPositions = new List<double>();
      public List<double> ValidYPositions
      {
         get { return validYPositions; }
         set { validYPositions = value; }
      }

      #endregion

      public InteractiveLineGraph()
      {
         InitializeComponent();

         DependencyPropertyDescriptor dpd = 
            DependencyPropertyDescriptor.FromProperty(DataProperty, typeof(InteractiveLineGraph));
         if(dpd != null)
         {
            dpd.AddValueChanged(this, OnDataPropertyChanged);
         }

         LayoutRoot.DataContext = this;
      }

      // Translate to screen space
      public Point NormalizePointToScreen(Point to)
      {
         var res = new Point();
         res.X = (to.X - Xmin) * ChartWidth / (Xmax - Xmin);
         res.Y = ChartHeight - (to.Y - Ymin) * ChartHeight / (Ymax - Ymin);
         return res;
      }

      // Transalte from screen space
      public Point NormalizePointFromScreen(Point from)
      {
         var res = new Point();
         res.X = from.X * (Xmax - Xmin) / ChartWidth + Xmin;
         res.Y = ((Ymin * from.Y) + (Ymax * ChartHeight) - (Ymax * from.Y)) / ChartHeight;
         return res;
      }

      private void Initialise()
      {
         DeregisterEvents();
         EllipsePoints.Clear();

         if (Data.Count > 0)
         {
            // Create ellipse points and register to events
            for (int i = 0; i < Data.Count; i++)
            {
               EllipsePoint ep = new EllipsePoint();
               ep.ParentGraph = this;
               ep.DataCollection = Data;
               ep.DataIndex = i;
               EllipsePoints.Add(ep);
            }

            Xmin = 0;
            Xmax = Data.Count;
            Ymin = -0.1;
            //Ymax = Data.Max() + 0.5; // + Padding

            RegisterEvents();
         }

         Draw();
      }

      private void Draw()
      {
         // Clear previously calculated information
         ChartCanvas.Children.Clear();
         dataPoints_.Clear();

         if (Data.Count > 0)
         {
            // Calculate point position
            for (int i = 0; i < Data.Count; i++)
            {
               double x = i;
               double y = Data[i];
               dataPoints_.Add(NormalizePointToScreen(new Point(x, y)));
            }

            // Calculate lines
            List<Line> graphLines = new List<Line>();
            for (int i = 1; i < dataPoints_.Count; i++)
            {
               graphLines.Add(new Line
               {
                  X1 = dataPoints_[i - 1].X,
                  Y1 = dataPoints_[i - 1].Y,
                  X2 = dataPoints_[i].X,
                  Y2 = dataPoints_[i].Y,
                  StrokeThickness = 2,
                  Stroke = Brushes.Black
               });
            }

            // Render lines
            foreach (var line in graphLines)
            {
               ChartCanvas.Children.Add(line);
            }

            // Render EllipsePoints
            foreach (var ep in EllipsePoints)
            {
               Point? p = ep.GetNormalizedPosition();
               if (p.HasValue)
               {
                  Canvas.SetLeft(ep, p.Value.X);
                  Canvas.SetTop(ep, p.Value.Y);
                  ChartCanvas.Children.Add(ep);
               }
            }
         }
      }
      
      #region Events

      private void RegisterEvents()
      {
         foreach (var ep in EllipsePoints)
         {
            ep.EPMouseLeftDown += OnEPMouseLeftDown;
            ep.EPMouseLeftUp += OnEPMouseLeftUp;
            ep.EPMouseMove += OnEPMouseMove;
         }
      }

      private void DeregisterEvents()
      {
         foreach (var ep in EllipsePoints)
         {
            ep.EPMouseLeftDown -= OnEPMouseLeftDown;
            ep.EPMouseLeftUp -= OnEPMouseLeftUp;
            ep.EPMouseMove -= OnEPMouseMove;
         }
      }

      private void OnDataPropertyChanged(object sender, EventArgs e)
      {
         Initialise();
      }
            
      private void ChartCanvasSizeChanged(object sender, SizeChangedEventArgs e)
      {
         ChartHeight = ChartCanvas.ActualHeight;
         ChartWidth = ChartCanvas.ActualWidth;
      }

      private void ChartCanvasLoaded(object sender, RoutedEventArgs e)
      {
         //Initialise();
      }

      private void ChartCanvasUnloaded(object sender, RoutedEventArgs e)
      {
         DeregisterEvents();
      }

      #region EllipsePoints

      private void OnEPMouseLeftDown(object sender, MouseButtonEventArgs e)
      {
         var ep = sender as EllipsePoint;
         if (ep != null)
         {
            Mouse.Capture((UIElement)ep);
            ep.IsDragging = true;
            elementPosition.X = Canvas.GetLeft((UIElement)ep);
            elementPosition.Y = Canvas.GetTop((UIElement)ep);
            originalElementPosition = new Point(elementPosition.X, elementPosition.Y);
            mousePosition = e.GetPosition(ChartCanvas);
         }
         else
         {
            throw new Exception("InteractiveLineGraph received event from non-EllipsePoint element.");
         }
      }

      private void OnEPMouseLeftUp(object sender, MouseButtonEventArgs e)
      {
         var ep = sender as EllipsePoint;
         if (ep != null)
         {
            Mouse.Capture(null);
            ep.IsDragging = false;
            // Snap value to int and update bound value
            Point graphPos = NormalizePointFromScreen(elementPosition);
            int newValue = Math.Max((int)Math.Round(graphPos.Y), 0);
            if(newValue != Data[ep.DataIndex])
            {
               Data[ep.DataIndex] = newValue;
            }
            else
            {
               Canvas.SetLeft(ep, originalElementPosition.X);
               Canvas.SetTop(ep, originalElementPosition.Y);
            }
         }
         else
         {
            throw new Exception("InteractiveLineGraph received event from non-EllipsePoint element.");
         }
      }

      private void OnEPMouseMove(object sender, MouseEventArgs e)
      {
         var ep = sender as EllipsePoint;
         if (ep != null)
         {
            if (ep.IsDragging)
            {
               Point newPosition = e.GetPosition(ChartCanvas);
               // Move X
               elementPosition.X += newPosition.X - mousePosition.X;
               // X moving disabled
               //Canvas.SetLeft((UIElement)ep, elementPosition.X);
               mousePosition.X = newPosition.X;
               // Move Y
               elementPosition.Y += newPosition.Y - mousePosition.Y;
               Canvas.SetTop((UIElement)ep, elementPosition.Y);
               mousePosition.Y = newPosition.Y;
            }
         }
         else
         {
            throw new Exception("InteractiveLineGraph received event from non-EllipsePoint element.");
         }
      }

      #endregion

      #endregion
   }
}
