# Power Table Graphs

Program plots laser power table calibration graphs imported from the SemiShop program. Software workflow id following:
* User specifies the SemiShop root folder, e.g. `C:/Hamamatsu/SemiShop4.18.0`.
* Program locates and parses XML files from the child `Configuration` folder.
* Available lasers and lenses installed on the tool are detected.
* Corresponding laser power graphs are plotted for each installed lens.

<img src="https://raw.githubusercontent.com/petrstepanov/hc-power-table-graphs/refs/heads/main/screenshot.png" alt="Power Table Graphs for SemiShop" style="width: 100%;"/>

Application is written in C++ and utilized CERN ROOT libraries version 6.36.

It is possible to compile and run the application on either Windows, macOS or Linux.

## 🪟 Installation on Windows

### Prerequisites

1. Download and run [Visual Studio 2022 Build Tools](https://aka.ms/vs/17/release/vs_buildtools.exe).
2. Select only `Desktop development with C++` checkbox and install it with default components.
3. Download and install [ROOT v6.36.06 x64 release for Windows](https://root.cern/download/root_v6.36.06.win64.python311.vc17.exe).
4. During the ROOT installation ensure ROOT environment is added to the system PATH.

### Compilation

1. Download and extract the [program source code](https://github.com/petrstepanov/hc-power-table-graphs/archive/refs/heads/main.zip).
2. Reveal the downloaded `.zip` archive in Windows Explorer. Right-click and select `Extract All...`.
3. Open `Developer Command Prompt` from Windows Start menu.
4. In the Command Prompt navigate to extracted program folder. Execute `cd %userprofile%\Downloads\hc-power-table-graphs-main\hc-power-table-graphs-main`.
5. Type and run `install.bat`.

### Run Executable

Locate and run `PowerGraphs.exe` in your user home folder under `Applications\PowerGraphs`.

## 🐧 Installation on Linux

Install CERN ROOT on your system. Refer to [official install page](https://root.cern/install/#linux-package-managers) for more details.

Open Terminal, clone program source files to your home folder, create a folder for out-of-source build. Then build and install.

```
cd && git clone https://github.com/petrstepanov/hc-power-table-graphs
mkdir hc-power-table-graphs-build && cd hc-power-table-graphs-build
cmake -DPORTABLE_INSTALL:BOOL=ON ../hc-power-table-graphs
make
make install
```

Program source files are not needed any more and can be removed from your computer:

```
rm -rf ~/hc-power-table-graphs
```

## Running program

Find application in the system application menu under the `Science` category. Alternatively, open Termnal and type `PowerGraphs` to launch the program.

## 🍏 Installation on macOS

### Prerequisites

Install Xcode command-line tools (includes `gcc` compiler, `git` version control, other utilities). In terminal, run:

```
xcode-select --install
```

Install MacPorts package manager ([not Homerbrew](https://root-forum.cern.ch/t/issue-with-root-on-mac-sequoia-still/63435/6)). [Download here](https://www.macports.org/install.php). Restart Terminal after installation.
Install CMake build system and a few program dependencies. In terminal, type:

```
sudo port install cmake
sudo port install giflib
sudo port install libjpeg-turbo
sudo port install tiff
sudo port install libpng
```

ROOT v6.36.08 is compiled with `libpng` library indifferent location. Therefore, we create symlink:

```
sudo mkdir /opt/X11/lib
sudo ln -s /opt/local/lib/libpng16.16.dylib /opt/X11/lib/libpng16.16.dylib
```

Download and install [latest stable ROOT](https://root.cern/install/all_releases/) release for macOS.

### Compilation

Check out the program source code and perform out-of-source build (recommended):

```
cd && git clone https://github.com/petrstepanov/hc-power-table-graphs
mkdir hc-power-table-graphs-build && cd hc-power-table-graphs-build
cmake -DPORTABLE_INSTALL:BOOL=ON ../hc-power-table-graphs
make
make install
```

Remove program source and build files from your computer:

```
rm -rf ~/hc-power-table-graphs*
```

### Run Executable

Locate and run `PowerGraphs` in Finder under the `Applications` folder. Alternatively, use `Spotlight` to find `PowerGraphs`.

---

Send me an email with feedback or questions: [stepanovps@gmail.com](mailto:stepanovps@gmail.com)
