using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Threading;
using System.Windows.Threading;
using System.Windows;

namespace NCSPgui
{
   public partial class ViewModel
   {
      private List<Shot> ShotCache = new List<Shot>();

      private void NCSPInitialise()
      {
         NCSPLibInitialise();
      }

      async void NCSPRunThread()
      {
         // Open communication pipe for reading
         HPipe = new NamedPipeServer(@"\\.\pipe\NCSPPipe1", 0);
         HPipe.MessageReceived += hPipe_MessageReceived;

         // Use Task to ensure this thread waits for instruction to return
         var startPipeTask = Task.Factory.StartNew(() => { HPipe.Start(); }).FailFastOnException();

         // Reset library settings
         Task resetTask = Task.Factory.StartNew(() => { NCSPLibReset(); }).FailFastOnException();
         resetTask.Wait();

         // Configure CSP
         Task setParamsTask = Task.Factory.StartNew(() => { NCSPSetParams(Model.ToParams()); }).FailFastOnException();
         setParamsTask.Wait();

         Task addCostsTask = Task.Factory.StartNew(() =>
         {
            foreach (var c in Model.MinimumCosts)
            {
               NCSPAddMinCost(c);
            }
         }).FailFastOnException();

         addCostsTask.Wait();

         // Ugly, but the pipe server will block (listening) if we use wait/await
         Thread.Sleep(500);

         // Run CSP
         var error = await Task<int>.Factory.StartNew(() => { return NCSPLibRun(); }).FailFastOnException();
         if (error == 0)
         {
            if (NCSPGetSolutionFound())
            {
               int numSolutions = NCSPGetAllResultsSize();
               SmartCollection<ShotSolution> allSolutions = new SmartCollection<ShotSolution>();

               for (int i = 0; i < numSolutions; i++)
               {
                  ShotSolution s = new ShotSolution();
                  List<Shot> shots = new List<Shot>();
                  int numShots = NCSPGetResultSize(i);

                  for (int j = 0; j < numShots; j++)
                  {
                     Shot shot = new Shot();
                     int noShotId = NCSPGetNoShotID();
                     SHOTINFO info = new SHOTINFO();
                     NCSPGetResultElement(i, j, out info);
                     int numCharacters = NCSPGetNumCharactersInElement(info.ID);

                     if (info.ID != noShotId)
                     {
                        if (numCharacters > 0)
                        {
                           List<String> characterNames = new List<String>();

                           for (int k = 0; k < numCharacters; k++)
                           {
                              var name = GetCharacterNameInShotByIndex(info.ID, k);
                              characterNames.Add(name);
                           }

                           shot.Characters = new SmartCollection<string>(characterNames);
                        }

                        String fileLoc = GetFileLocation(info.ID);
                        shot.FileLocation = fileLoc;
                        shot.Info = info;
                        shots.Add(shot);
                     }
                  }

                  s.Shots = shots;
                  s.Name = (i == 0) ? "Original" : "Variant " + i;
                  allSolutions.Add(s);
               }

               Solution = allSolutions;
               MainCharacterName = GetMainCharacterName();
            }
            else
            {
               // No solution found. Error message already displayed
               // the user via pipe comms
            }
         }
         else
         {
            // TODO: Provide well-defined error codes
            switch (error)
            {
               case 1:
                  DebugOutput += "Pipe could not be initialised (try again)\n";
                  break;
               default:
                  DebugOutput += "Error in running NCSP\n";
                  break;
            }

         }

         var stopPipeTask = await Task<int>.Factory.StartNew(() => { return HPipe.StopServer(); }).FailFastOnException();
         HPipe.MessageReceived -= hPipe_MessageReceived;
         RunButtonEnabled = true;
      }

      void hPipe_MessageReceived(object sender, MessageReceivedEventArgs e)
      {
         Application.Current.Dispatcher.BeginInvoke((Action)delegate()
         {
            DebugOutput += e.message_;
         });
      }

      /// <summary>
      /// Runs NCSPCore.exe in another thread
      /// </summary>
      public void NCSPRun()
      {
         RunButtonEnabled = false;
         Task.Factory.StartNew(() => { NCSPRunThread(); });
      }

      public void NCSPShutdown()
      {
         NCSPLibShutdown();
         if (HPipe != null)
         {
            HPipe.MessageReceived -= hPipe_MessageReceived;
            HPipe.StopServer();
         }
      }

      private String GetCharacterNameInShotByIndex(int id, int i)
      {
         return Marshal.PtrToStringAnsi(NCSPGetCharacterInElement(id, i));
      }

      private String GetFileLocation(int id)
      {
         return Marshal.PtrToStringAnsi(NCSPGetFileLocation(id));
      }

      private String GetMainCharacterName()
      {
         return Marshal.PtrToStringAnsi(NCSPGetMainCharacterName());
      }

      #region DLL Imports

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPLibInitialise();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPLibShutdown();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern void NCSPLibReset();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPLibRun();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern void NCSPSetParams(PARAMS p);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern void NCSPAddMinCost(int cost);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      [return: MarshalAs(UnmanagedType.I1)]
      public static extern bool NCSPGetSolutionFound();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPGetAllResultsSize();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPGetResultSize(int i);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern void NCSPGetResultElement(int i, int j, out SHOTINFO info);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPGetNumCharactersInElement(int id);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
      public static extern IntPtr NCSPGetCharacterInElement(int id, int i);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
      public static extern IntPtr NCSPGetFileLocation(int id);

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
      public static extern IntPtr NCSPGetMainCharacterName();

      [DllImport("NCSPLib.dll", CallingConvention = CallingConvention.Cdecl)]
      public static extern int NCSPGetNoShotID();

      #endregion
   }
}
