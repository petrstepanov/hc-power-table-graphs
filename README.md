# Power Table Graphs

Program plots laser power table calibration graphs imported from the SemiShop program. Software imports XML configuration files, detects installed lasers, lenses and plots corresponding laser power graphs for each installed lens.

<img src="https://raw.githubusercontent.com/petrstepanov/hc-power-table-graphs/refs/heads/main/screenshot.png" alt="Power Table Graphs for SemiShop" style="width: 100%;"/>

Application is written in C++ and utilized CERN ROOT libraries version 6.36.

It is possible to compile and run the application on either Windows, macOS or Linux.

## Installation on Windows

### Prerequisites

1. Download and run [Visual Studio Build Tools](https://aka.ms/vs/stable/vs_BuildTools.exe).
2. Install the "Desktop development with C++" package and install it with default components.
3. Download and install ROOT release for Windows. Use `Windows Visual Studio 2022 64-bit x64` executable from the [latest release page](https://root.cern/install/all_releases/). During the installation ensure ROOT environment is added to the system PATH.
4. ROOT releases not necessarily includes the Fourier Transform library (FFTW). [Download it here](http://www.fftw.org/install/windows.html). Use x64 version. Unpack and copy the `libfftw3-3.dll` to ROOT's `bin` folder, e.g. `C:\root_v6.##.##\bin`.
5. Above FFTW library is linked to ROOT's `libFFTW.dll` and should be renamed correctly. Rename `libfftw3-3.dll` to `fftw3-3.dll`.
6. **Optional**. To ensure the correct name for the FFTW library, open ROOT's `libFFTW.dll` in [Dependencies](https://github.com/lucasg/Dependencies) (former [Dependency Walker](https://dependencywalker.com/)) program. The correct name for the FFTW library will be listed as a missing dependency in the top-right panel.

### Compilation

1. Download and extract the program source code (green button on the top of this page).
2. Open Developer Command Prompt from the Start menu.
3. In the Command Prompt navigate to extracted program folder, e.g. `cd C:\Users\<your-user>\Downloads\hc-power-table-graphs`.
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
