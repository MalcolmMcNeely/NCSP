using NCSPgui;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace NCSPgui
{
   /// <summary>
   /// Interaction logic for App.xaml
   /// </summary>
   public partial class App : Application
   {
      private void ApplicationStartup(object sender, StartupEventArgs e)
      {
         new MainWindow().Show();
         Application.Current.MainWindow.Title = "Narrative Constraint Satisfaction Problem Solver";
         Application.Current.MainWindow.Closed += OnMainWindowClosed;
      }

      private void OnMainWindowClosed(object sender, EventArgs e)
      {
         var window = sender as MainWindow;
         if (window != null)
         {
            var viewModel = window.DataContext as ViewModel;
            if (viewModel != null)
            {
               viewModel.NCSPShutdown();
            }

            window.Closed -= OnMainWindowClosed;
         }
         Shutdown();
      }

      private void ApplicationExit(object sender, ExitEventArgs e)
      {
         var window = Application.Current.MainWindow as MainWindow;
         if (window != null)
         {
            OnMainWindowClosed(window, new EventArgs());
         }
      }
   }
}
