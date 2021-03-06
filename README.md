# Spring 2018 6.835 Final Project Mario IRL
## Table of Contents
Files that are not listed are provided leapmotion library and example files or files not changed in the mario game for the purpose of this project
* uMario_Jakowski
  * uNext
    * core.cpp (file altered to allow gesture control)
* leap_files
  * main.py (main file to run to allow for choice of gesture set used)
  * gesture1.py (code for gesture set 1)
  * gesture2.py (code for gesture set 2)
  * MarioTimer.py (custom functions for timers)

## System Requirements:
* OS: Ubuntu 16.04
* Python 2.7
* Leap Motion Linux SDK 2.2.3
* CMake

## Install Mario:
Cd into the uMario_Jakowski folder and sudo apt-get install the following dependencies:
* cmake
* libsdl2-dev version:2.0.4+dfsg1-2ubuntu2
* libsdl2-image-dev version:2.0.1+dfsg-2+deb9u1build0.16.04.1
* libsdl2-mixer-dev version:2.0.4+dfsg1

## Build the game:
if a build folder already exists delete the contents and perform the following commands
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./uMario
```

## Test the game
(executable should be in build folder)
```
$ ./uMario
```
Mario window should appear

## Configure Leap Motion Files:

Cd into leap_files
Open gesture1.py and on line 10, change the path path to your lib/x64 folder inside the SDK you downloaded:
```
sys.path.insert(0, "PATH TO LeapSDK/lib/x64")
```
Open gesture2.py and repeat the same steps

Pip install the following python packages (latest version):
* posix
* errno
* stat
* alsaaudio
* audioop

## Running the game (you will need to open three terminals A, B, C):
In terminal A start the leapmotion daemon (in ubuntu you have to manually start it)
```
$ leapd
```

In terminal B start the python main script
```
$ python main.py
```
Follow the prompts and chose which mode you’d like to play the game in

In terminal C start the mario game
```
$ ./uMario
```

Hit enter twice to start the game on the first level and you should be all set to play the game using the chosen modality!

**Credit to jakowskidev for the uMario platform!**
