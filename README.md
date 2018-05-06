# LudumDare41

https://ldjam.com/events/ludum-dare/41/$80635


Started with base project from here: (https://github.com/Stephen321/sfml_starter)


Linux:
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

