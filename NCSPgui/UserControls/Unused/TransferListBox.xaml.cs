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
using System.Collections;
using System.Collections.ObjectModel;
using NCSPgui;

namespace MControls
{
 /* Custom user control for being to MOVE elements of one collection to another
 * 
 * EXAMPLE USAGE:
 * <controls:TransferListBox Margin="0,5,0,5"
 *                           LeftListHeader="Vector A"
 *                           RightListHeader="Vector B"
 *                           LeftList="{Binding VectorA, UpdateSourceTrigger=PropertyChanged}"
 *                           RightList="{Binding VectorB, UpdateSourceTrigger=PropertyChanged}"
 *                           AddItemClick="OnCharacterListAddItem"
 *                           RemoveItemClick="OnCharacterListRemoveItem"/>
 *                           
 * In mainwindow.xaml
 * 
 *  private void OnCharacterListAddItem(object sender, MControls.TransferListBox.TransferListBoxAddArgs e)
 *  {
 *    (this.DataContext as ViewModel).CharacterSelected(e.AddedItem);
 *  }
 *
 * private void OnCharacterListRemoveItem(object sender, MControls.TransferListBox.TransferListBoxAddArgs e)
 * {
 *    (this.DataContext as ViewModel).CharacterDeselected(e.AddedItem);
 * }
 *                 
 */
   public partial class TransferListBox : UserControl
   {
      public TransferListBox()
      {
         InitializeComponent();
         LayoutRoot.DataContext = this;
      }

      #region Events

      public class TransferListBoxAddArgs : EventArgs
      {
         public TransferListBoxAddArgs(string item)
         {
            AddedItem = item;
         }
         public String AddedItem { get; set; }
      }

      public event EventHandler<TransferListBoxAddArgs> AddItemClick;
      private void OnAddButtonClick(object sender, RoutedEventArgs e)
      {
         if (AddItemClick != null)
         {
            if (LeftListBox.SelectedValue != null)
            {
               String itemText = LeftListBox.SelectedValue.ToString();
               AddItemClick(this, new TransferListBoxAddArgs(itemText));
            }
         }
      }

      public event EventHandler<TransferListBoxAddArgs> RemoveItemClick;
      private void OnRemoveButtonClick(object sender, RoutedEventArgs e)
      {
         if (RemoveItemClick != null)
         {
            if (RightListBox.SelectedValue != null)
            {
               String itemText = RightListBox.SelectedValue.ToString();
               RemoveItemClick(this, new TransferListBoxAddArgs(itemText));
            }
         }
      }

      #endregion

      #region Dependency Properties

      public SmartCollection<String> LeftList
      {
         get { return (SmartCollection<String>)GetValue(LeftListProperty); }
         set { SetValue(LeftListProperty, value); }
      }

      public static readonly DependencyProperty LeftListProperty =
          DependencyProperty.Register("LeftList", typeof(SmartCollection<String>),
            typeof(TransferListBox));//, new PropertyMetadata(new ObservableCollection<String>()));

      public SmartCollection<String> RightList
      {
         get { return (SmartCollection<String>)GetValue(RightListProperty); }
         set { SetValue(RightListProperty, value); }
      }

      public static readonly DependencyProperty RightListProperty =
          DependencyProperty.Register("RightList", typeof(SmartCollection<String>),
            typeof(TransferListBox));//, new PropertyMetadata(new ObservableCollection<String>()));

      public String LeftListHeader
      {
         get { return (String)GetValue(LeftListHeaderProperty); }
         set { SetValue(LeftListHeaderProperty, value); }
      }

      public static readonly DependencyProperty LeftListHeaderProperty =
         DependencyProperty.Register("LeftListHeader", typeof(String),
            typeof(TransferListBox), new PropertyMetadata("Left"));

      public String RightListHeader
      {
         get { return (String)GetValue(RightListHeaderProperty); }
         set { SetValue(RightListHeaderProperty, value); }
      }

      public static readonly DependencyProperty RightListHeaderProperty =
         DependencyProperty.Register("RightListHeader", typeof(String),
            typeof(TransferListBox), new PropertyMetadata("Right"));

      #endregion
   }
}
