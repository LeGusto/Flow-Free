# Flow-Free

A C++ implementation of the popular Flow-Free puzzle game using the SFML library. The goal is to connect matching colored nodes on a grid without overlapping paths.

## Features
- Interactive grid-based gameplay
- Intuitive and satisfying path drawing
- 15+ levels loaded dynamically from text files
- Undo/Redo functionality for paths

## Requirements
- C++17 or later
- [SFML 2.5](https://www.sfml-dev.org/download/) or later
- CMake 3.15 or later

## Installation & Running the Game

1. **Clone the repository**
   ```bash
   git clone https://github.com/LeGusto/Flow-Free
   cd ./Flow-Free
   ```  

2. **Install Dependencies**  
- On Ubuntu/Debian:
   ```bash
   sudo apt update
   sudo apt install libsfml-dev cmake g++
   ```  
    
- On Windows:
  Download and install [SFML](https://www.sfml-dev.org/download/).  
  Ensure CMake and a compatible compiler (e.g., Visual Studio) are installed.  
- On macOS:
  ```bash
   brew install sfml cmake
3. **Build** the game:
   ```bash
   mkdir build 
   cd build
   cmake ..
   make
   cd ..
   
4. **Launch** the game:
   ```bash
   ./build/bin/main
   
