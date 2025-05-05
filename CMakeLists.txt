cmake_minimum_required(VERSION 3.28)
project(Flow-Free LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests 
    src/tests/pathTests.cpp
    src/tests/actionTests.cpp
    src/implementations/Button.cpp
    src/implementations/FlowGrid.cpp
    src/implementations/MainMenu.cpp
    src/implementations/LevelReader.cpp
    src/implementations/LevelSelection.cpp
    src/implementations/CompletedScreen.cpp
    src/implementations/Cells.cpp
    src/implementations/PathManager.cpp
    src/implementations/ActionManager.cpp
)

add_executable(main
    src/implementations/main.cpp
    src/implementations/Button.cpp
    src/implementations/FlowGrid.cpp
    src/implementations/MainMenu.cpp
    src/implementations/LevelReader.cpp
    src/implementations/LevelSelection.cpp
    src/implementations/CompletedScreen.cpp
    src/implementations/Cells.cpp
    src/implementations/PathManager.cpp
    src/implementations/ActionManager.cpp
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain SFML::Graphics)

