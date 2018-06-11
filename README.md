# LudumDare41

https://ldjam.com/events/ludum-dare/41/$80635


Started with base project from here: (https://github.com/Stephen321/sfml_starter)

Controls 
- W A S D to move 
- E to start fishing

Version submitted for Ludum Dare 41: https://github.com/Stephen321/LudumDare41/releases/tag/1.0.0 
Continued progress on https://github.com/Stephen321/LudumDare41/tree/master

### Using CMake:  
Install latest: (https://cmake.org/download/)  
- git clone https://github.com/stephenn321/LudumDare41 ld41
- cd ld41
- mkdir build
- cd build
- choose generator:
    + cmake -G "Visual Studio 15 2017" ..
    + cmake -G "Unix Makefiles" ..
- open LudumDare41.sln  
*With cmake-gui set source to path to "ld41" folder and build to a subddirectory under that*

If using Visual Studio then right click the LudumDare41 project and "Set as startup project".


### Linux:  
If *sudo apt-get install libsfml-dev* doesnt install sfml 2.4.2 or later then install as follows:  

*Install sfml dependencies*
  - sudo apt-get install -y freeglut3-dev 
  - sudo apt-get install -y libjpeg-dev 
  - sudo apt-get install -y libfreetype6-dev 
  - sudo apt-get install -y libxrandr-dev 
  - sudo apt-get install -y libglew-dev 
  - sudo apt-get install -y libsndfile1-dev 
  - sudo apt-get install -y libopenal-dev
  
*Download and setup sfml*
  - SFML_URL="https://www.sfml-dev.org/files/SFML-2.4.2-linux-gcc-64-bit.tar.gz"
  - mkdir sfml && wget --no-check-certificate --quiet -O - ${SFML_URL} | tar --strip-components=1 -xz -C sfml
  - export SFML_ROOT=/path/to/sfml


