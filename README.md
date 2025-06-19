# Flow-Free

A C++ implementation of the popular Flow-Free puzzle game using the SFML library. The goal is to connect matching colored nodes on a grid without overlapping paths.

![FlowPic3](https://github.com/user-attachments/assets/dbe4e78d-7175-499c-8617-47c2128ffffb)
![FlowPic2](https://github.com/user-attachments/assets/3e7ff771-6b1a-4822-9ff8-010ed66c4a22)
![FlowPic1](https://github.com/user-attachments/assets/90974b96-6c32-4734-bca0-a90b159b98e1)




## Features
- Interactive grid-based gameplay
- Intuitive and satisfying path drawing
- 10 levels
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
   
