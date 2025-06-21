cmake_minimum_required(VERSION 3.15) # Specifies the minimum required CMake
                                     # version
# Defines the project name and the languages used
project(
  MyProject
  VERSION 1.0
  LANGUAGES CXX)

# Defines the configuration for this project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(series src/serie.cpp src/main.cpp) # Creates an executable
                                                   # target named
# 'my_program' from 'main.cpp'

# Include the directories for the header files
target_include_directories(series PUBLIC include)
