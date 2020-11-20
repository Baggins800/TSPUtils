# TSPUtils
Travelling salesman utility functions for c++ using json graphs. To generate datasets register on https://gisgraph.com, create and download the dataset(s).
## Requirements
* Download cmake from https://cmake.org/download/ and install
* *Windows:* Download MinGW from https://sourceforge.net/p/mingw-w64/wiki2/Make/ and install
* *Unix:* make sure you install g++ from your distribution's repository
* Download git from https://git-scm.com/downloads and install

Open a terminal and `git clone https://github.com/baggins800/TSPUtils` or download and unzip https://github.com/Baggins800/TSPUtils/archive/main.zip.
## How to build and compile
### Windows 10
*Without cmake:* open PowerShell, navigate to `TSPUtils/code/` and run `g++ main.cpp -o TSP && ./TSP.exe`\
*With cmake:* open PowerShell, navigate to `TSPUtils/`,
```
mkdir build && cd build
cmake ../code && make -j
```
*Using an IDE:* `File > Open Project` and select `TSPUtils/code/CMakeLists.txt`
### Linux/macOS
```
git clone https://github.com/baggins800/TSPUtils && cd TSPUtils
mkdir build && cd build
cmake ../code && make -j
```

## Run the executable
### Windows 10
./TSP.exe --json=pathto.json --greedy
### Linux
./TSP --json=pathto.json --greedy
