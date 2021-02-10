Welcome to the official distribution of the Osprey simulation engine.

**Osprey-DPD - Open Source Polymer Research Engine-Dissipative Particle Dynamics** - is free software distributed under the BSD 3-clause license. It was developed by the author at the Max Planck Institute of Colloids and Interfaces, Germany, 1999-2007. You may use the code in any way you wish, modify and redistribute it or embed it in other software, subject to the conditions of the license. See the license file for details.

The code is written in standard  C++03 and uses the STL. It has run successfully on Linux, Mac OS X and Windows 10 platforms.

On Linux platforms, the code is compiled and linked using the GNU C++ compiler with the following two commands executed in the source code directory:

```shell
$ g++ -c -O3 *.cpp
$ g++ -o dpd *.o
```

One can also build it using cmake:
```shell
$ mkdir build
$ cd build

# for Release
$ cmake ..
$ make

# for Debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make

# or using Ninja
$ cmake .. -GNinja
$ ninja

```

The main() routine is in the file  "dmpc.cpp".

The code is executed by entering its name at the command line and providing a single argument that is the extension of the required text input file that must be named "dmpci.nnn" where "nnn" is a user-defined alphanumeric string. This command
```shell
$ ./dpd  001
```

will invoke the code and cause it to read its input from the file "dmpci.001" that must be located in the current directory. If the specified file is not found, the code creates a default input file that can then be modified as desired. Some simple input files for various cases can be found in the /examples directory.

The User Guide describes how to compile and run the code, and explains many of the code features.

For further information, please contact the author:  jsnano "at" me "." com
