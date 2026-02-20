# Power Table Graphs

Program plots laser power table calibration graphs imported from the SemiShop program. Software workflow id following:
* User specifies the SemiShop root folder, e.g. `C:/Hamamatsu/SemiShop4.18.0`.
* Program locates and parses XML files from the child `Configuration` folder.
* Available lasers and lenses installed on the tool are detected.
* Corresponding laser power graphs are plotted for each installed lens.

<img src="https://raw.githubusercontent.com/petrstepanov/hc-power-table-graphs/refs/heads/main/screenshot.png" alt="Power Table Graphs for SemiShop" style="width: 100%;"/>

Application is written in C++ and utilized CERN ROOT libraries version 6.36.

It is possible to compile and run the application on either Windows, macOS or Linux.

## Installation on Windows

### Prerequisites

1. Download and run [Visual Studio Build Tools](https://aka.ms/vs/stable/vs_BuildTools.exe).
2. Select only "Desktop development with C++" checkbox and install it with default components.
3. Download and install ROOT release for Windows. Use `Windows Visual Studio 2022 64-bit x64` executable from the [latest release page](https://root.cern/install/all_releases/).
4. During the ROOT installation ensure ROOT environment is added to the system PATH.

### Compilation

1. Download and extract the program source code (green button on the top of this page).
2. Open `Developer Command Prompt` from Windows Start menu.
3. In the Command Prompt navigate to extracted program folder. Execute `cd C:\Users\<your-user>\Downloads\hc-power-table-graphs`.
4. Run `.\install.bat`.

### Execution

Find and run `PowerGraphs.exe` in your home folder under `Applications\PowerGraphs`.

## Installation on Linux or macOS

Install CERN ROOT on your system. Refer to [official install page](https://root.cern/install/#linux-package-managers) for more details.

Open Terminal and clone program source files in your home folder:

```
cd
git clone https://github.com/petrstepanov/hc-power-table-graphs
cd sw-calculator
```
Check program dependencies and create native makefile with CMake. Finally build and install. System will ask for your password to install the app.

```
cmake ./
make
sudo make install
```
On linux you can install application launcher:

```
make install-launcher
```
To install app launcher on macOS run package `resources/hc-power-table-graphs.pkg`.

Program source files are not needed any more and can be removed from your computer:

```
rm -rf ~/hc-power-table-graphs
```

## Running program

Find application in the system menu. Alternatively, open Termnal and type `hc-power-table-graphs` to launch the program.


---

Shoot me an email with feedback or questions: [stepanovps@gmail.com](mailto:stepanovps@gmail.com)
