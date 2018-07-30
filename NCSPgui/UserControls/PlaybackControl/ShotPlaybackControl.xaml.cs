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
using MHelper;

namespace MControls
{
   /// <summary>
   /// UserControl for displaying the collection of collection of shots. Example use:
   ///             <controls:ShotPlaybackControl Items="{Binding Solution, UpdateSourceTrigger=PropertyChanged}"
   ///                                           SelectedCollectionIndex="{Binding ShotCollectionSelectedId}"/>
   /// </summary>
   public partial class ShotPlaybackControl : UserControl
   {
      public ShotPlaybackControl()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
      }

      public SmartCollection<ShotSolution> Items
      {
         get { return (SmartCollection<ShotSolution>)GetValue(ItemsProperty); }
         set { SetValue(ItemsProperty, value); }
      }

      public static readonly DependencyProperty ItemsProperty =
         DependencyProperty.Register("Items", typeof(SmartCollection<ShotSolution>),
            typeof(ShotPlaybackControl));

      public int SelectedCollectionIndex
      {
         get { return (int)GetValue(SelectedCollectionIndexProperty); }
         set { SetValue(SelectedCollectionIndexProperty, value); }
      }

      public static readonly DependencyProperty SelectedCollectionIndexProperty =
         DependencyProperty.Register("SelectedCollectionIndex", typeof(int),
            typeof(ShotPlaybackControl), 
            new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

      private void OnPlaybackSegmentSelected(object sender, RoutedEventArgs e)
      {
         var control = sender as ShotPlaybackSegment;
         if(control != null)
         {
            if (Items != null)
            {
               Shot shot = null;
               int shotId = Int32.Parse(control.ShotNameDisplayed);
               int? collectionId = null;

               for(int i = 0; i < Items.Count; i++)
               {
                  foreach (var j in Items[i].Shots)
                  {
                     if(shotId == j.Info.ID)
                     {
                        shot = j;
                        collectionId = i;
                        break;
                     }
                  }

                  if (shot != null)
                  {
                     // Display shot details in ShotDetailsTextBox
                     StringBuilder sb = new StringBuilder();
                     sb.AppendLine(String.Format("Displaying details for {0}:", shot.ShotName));
                     sb.AppendLine(String.Format("Cost: {0}", shot.Info.Cost));
                     sb.AppendLine(String.Format("Gore: {0}", shot.Info.Gore));
                     sb.AppendLine(String.Format("Thrill: {0}", shot.Info.Thrill));
                     sb.AppendLine(String.Format("Empathy: {0}", (shot.Info.Empathy == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Threaten: {0}", (shot.Info.Threaten == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Chase: {0}", (shot.Info.Chase == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Hide: {0}", (shot.Info.Hide == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Kill: {0}", (shot.Info.Kill == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Attack: {0}", (shot.Info.Attack == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Escape: {0}", (shot.Info.Escape == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Location: {0}", (shot.Info.Location == 1 ? "Yes" : "No")));
                     sb.AppendLine(String.Format("Duration: {0}", (shot.Info.Duration == 1 ? "Yes" : "No")));

                     if (shot.Characters != null)
                     {
                        sb.AppendLine("Characters: ");
                        foreach (var c in shot.Characters)
                        {
                           sb.Append(c + " ");
                        }
                        sb.Append("\r\n");
                     }
                     else
                     {
                        sb.AppendLine("No characters");
                     }

                     sb.AppendLine(String.Format("File: {0}", shot.FileLocation));
                     ShotDetailsTextBox.Text = sb.ToString();
                     SelectedCollectionIndex = (int)collectionId;
                     break;
                  }
               }
            }
            else
            {
               ShotDetailsTextBox.Text = "Error: Items are null";
            }
         }

         // Custom event is handled
         e.Handled = true;
      }
   }
}
