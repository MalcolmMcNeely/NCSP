using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Diagnostics;
using System.Threading;

namespace NCSPgui
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      private Queue<Shot> playlist = new Queue<Shot>();

      public MainWindow()
      {
         InitializeComponent();

         var viewModel = new ViewModel();
         this.DataContext = viewModel;
         Closing += viewModel.OnWindowClosing;

         //// VLC Player
         //string VLCDir;
         //if (IntPtr.Size == 4)
         //{
         //   // Use 32 bits library
         //   VLCDir = System.IO.Path.Combine(Environment.CurrentDirectory, "LibVLC\\x86");
         //}
         //else
         //{
         //   // Use 64 bits library
         //   VLCDir = System.IO.Path.Combine(Environment.CurrentDirectory, "LibVLC\\x64");
         //}

         //VLCMediaControl.MediaPlayer.VlcLibDirectory = new DirectoryInfo(VLCDir);
         //VLCMediaControl.MediaPlayer.EndInit();
      }

      #region Events

      private void ExitButtonClick(object sender, RoutedEventArgs e)
      {
         this.Close();
      }

      private void RunButtonClick(object sender, RoutedEventArgs e)
      {
         (this.DataContext as ViewModel).NCSPRun();
      }

      private void DefaultValuesButtonClick(object sender, RoutedEventArgs e)
      {
         (this.DataContext as ViewModel).SetDefaultProperties();
      }

      #endregion

      #region VLC Control

      private void PlaylistThread()
      {
         bool playing = true;

         while (playing)
         {
            Shot s = playlist.Dequeue();            
            bool playFinished = false;
            string filename = s.FileLocation;

            if (filename != null)
            {
               VLCMediaControl.MediaPlayer.EncounteredError += (sender, e) =>
               {
                  MediaOutputTextBox.AppendText("VLC Error");
                  playFinished = true;
               };

               VLCMediaControl.MediaPlayer.EndReached += (sender, e) =>
               {
                  playFinished = true;
               };

               VLCMediaControl.MediaPlayer.Play(new FileInfo(filename));

               // Playback from stream example:
               //VLCMediaControl.MediaPlayer.Play(new Uri("http://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_surround-fix.avi"));        
               // Playback from file example:
               //VLCMediaControl.MediaPlayer.Play(new FileInfo(@"..\..\..\..\Media\FateApocrypha.mp4"));

               while (!playFinished)
               {
                  Thread.Sleep(TimeSpan.FromMilliseconds(100));
               }

               if (playlist.Count == 0)
               {
                  playing = false;
               }
            }
         }
      }

      // VLC.NET has no playlist functionality, so we make our own by
      // unloading a queue in another thread
      private void OnPlayButtonClick(object sender, RoutedEventArgs e)
      {
         VLCCleanUp();
         StringBuilder sb = new StringBuilder();

         // Get selected collection of shot to play
         var vm = (ViewModel)this.DataContext;
         if (vm != null)
         {
            if (vm.Solution != null)
            {
               if (vm.Solution.Count > vm.ShotCollectionSelectedId)
               {
                  var shots = vm.Solution[vm.ShotCollectionSelectedId];

                  if (shots.Shots.Count > 0)
                  {
                     // Construct playlist
                     //bool flipFlop = true;
                     playlist.Clear();
                     //foreach (var s in shots.Shots)
                     //{
                     //   // Since we lack proper content, use samples for now
                     //   if (flipFlop)
                     //   {
                     //      s.FileLocation = System.IO.Path.Combine(Environment.CurrentDirectory, "Media\\FateApocrypha.mp4");
                     //   }
                     //   else
                     //   {
                     //      s.FileLocation = System.IO.Path.Combine(Environment.CurrentDirectory, "Media\\MaidensSalvationTheory.mp4");
                     //   }
                     //   flipFlop = !flipFlop;
                     //   playlist.Enqueue(s);
                     //}

                     // Output shots to be played to user                  
                     sb.Append("Playing shots: ");
                     foreach (var s in shots.Shots)
                     {
                        sb.Append(String.Format("{0}, ", s.ShotId));
                     }
                     sb.Append("\n");

                     // Start playing in different thread
                     //ThreadStart threadStart = new ThreadStart(PlaylistThread);
                     //Thread thread = new Thread(threadStart);
                     //thread.Start();
                  }
                  else
                  {
                     sb.AppendFormat("Error: ViewModel.Solution.Shots[{0}].Count is < 1",
                        vm.ShotCollectionSelectedId);
                  }
               }
               else
               {
                  sb.AppendFormat("Error: ViewModel.Solution is size {0} while ViewModel.ShotCollectionSelectedId is {1}\n",
                     vm.Solution.Count, vm.ShotCollectionSelectedId);
               }
            }
            else
            {
               sb.AppendLine("Error: ViewModel.Solution is null");
            }
         }
         else
         {
            sb.AppendLine("Error: ViewModel is null");
         }

         MediaOutputTextBox.AppendText(sb.ToString());
      }

      private void OnStopButtonClick(object sender, RoutedEventArgs e)
      {
         VLCCleanUp();
      }

      private void OnPauseButtonClick(object sender, RoutedEventArgs e)
      {
         //VLCMediaControl.MediaPlayer.Pause();
      }

      private void VLCCleanUp()
      {
         //if (VLCMediaControl.MediaPlayer.State == Vlc.DotNet.Core.Interops.Signatures.MediaStates.Playing)
         //{
         //   // Trying to solve intermittent deadlock issue
         //   var thread = new System.Threading.Thread(delegate() 
         //      { 
         //         VLCMediaControl.MediaPlayer.Stop();
         //      });
         //   thread.Start();
         //}
      }

      #endregion

   }
}
