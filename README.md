# Narrative Constraint Satisfaction Problem (NCSP) Solver

This program takes in shot data and uses Gecode to find a narrative solution based on the solver's configuration settings in NCSPgui.exe. If the shot data has specified a media files, then the gui can play them in it's embedded VLC instance. Results are stored in file format in the program created "Output" folder. If building the project results in an error "NCSPLib.dll not found" then ensure that NCSPLib has the post-build script shown later in this document.

### Prerequisites

Gecode 5.1.0-x64-2017.msi (you can uninstall by running the installer again)
http://www.gecode.org/download.html

Latest .NET Framework Runtime+
https://www.microsoft.com/net/download/dotnet-framework-runtime
	
Latest C++ Redistributable
https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads	

To use VLC player you need to lib files (~128GB per configuration [x86/x64])
https://wiki.videolan.org/LibVLC/

The following packages should already be installed for NCSPGui.csproj:
* Vlc.DotNet.Core
* Vlc.DotNet.Core.Interops
* Vlc.DotNet.Wpf
* Vlc.DotNet.Forms
	
If not, then you can installed via Nuget (right click NCSPgui.csproj -> Manage NuGet packages) which will require a restart of Visual Studio.
	
### Architectural notes

NCSPgui is a standard C#/WPF MVVM pattern. It launches NCSPLib.dll in a different thread and is able to communicate with it using named pipes which allows the console output to be redirected to the NCSPgui listening thread. NCSPLib acts as a wrapper for NCSPCore.
	
NCSPCore has been designed to accept a constraint profile implementing IConstraintProfile which determines how the data is treated and what constraints are used. Derivatives of IConstraintProfile require an input struct ProfilePackage which contains information from the user.
	
Before NCSPCore starts looking for narrative solutions it will first run the Hierarch constraint profile which acts on a completely different model than the StoryProfile/StoryVariantProfile. The Hierarch is responsible for validating that the user has requested a solution that is valid. For example, if you request that the Character Appearance Rate to be 90% but no characters has that much screen time, then the Hierarch will fail. This is an attempt at overcomming the difficulty in determining why a Constraint Satisfaction Problem has failed, where you can't simply look into the search space to see what parameters the would-be solution is currently holding.
	
If you want to change the NCSPCore project settings into an executable, there is an entry point for command line arguments but it uses a different function in NCSPEntry than the function for the NCSPgui.	
	
### Post-build scripts for reference

  NCSPLib.dll (all platforms/configurations):
  xcopy "$(TargetPath)" "$(SolutionDir)\NCSPgui\bin\$(Platform)\$(Configuration)\" /Y

### Usage

GUI Modelling options:

* Viewer Type: Gore or Thrill viewer.

* Number of segments: A segment is a group of shots in the same location (NCSP is solved segment by segment).

* Duration: Desired time from the beginning of the first segment to the end of the last.

* Duration Bound: Actual output time will be within the range [duration - duration bound, duration + duration bound].

* Character Appearance Rate: NCSP will select a "main character", anyone can be the main character so long as they appear this percentage rate.

* Minimum Costs: The costs, or preference for a certain viewer type, of the shots selected for a segment will be at least this number.

* Variance: Variant solutions of the baseline will vary by this number in terms of unique elements.

* Number of Variants: Number of variant solutions that will try to be generated.

* Library Multiplier: Library size will be whatever is provided in the .xml data file + (Number of location * (32 * Library Multiplier)).

* Timeout: Time until search stops if a solution has not been found.

* Print Library Details: If selected, will print to file details of the input shot library (including synthetic shots from the Library Multiplier) in the \Data\ folder.

### Changelog

[1.1 Fixed: ensured Task exceptions quickly fail, updated all projects to latest libraries]

NCSPgui: crash shortly after pressing run (with a long wait time)
	
[1.1 Fixed: Pipe creation/destruction wasn't being handled correctly - added delay between setting up listening on the server and requesting connection from the client]

NCSPgui: The first time the run button is clicked, it may require another click (all other clicks are fine)
			 The first time a solution is being run, the debug message pane sometimes doesn't update (all future runs are fine)
