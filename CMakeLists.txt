cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once, and is not re-generated if converter is
# called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
set(CMAKE_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32_Car)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM CXX)

# C++ related settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Flag confiugration for C and C++
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g3")
  message("-Og -g3 flag(s) set")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  message("-O3 flag(s) set")
endif()

# -O3 → high-level optimization -flto → link-time optimization (lets the linker
# optimize across all files) -ffunction-sections / -fdata-sections → places each
# function/variable in its own section --gc-sections → removes all unused code
# -fno-exceptions / -fno-rtti → removes huge C++ runtime bloat (especially from
# newlib)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -fno-exceptions")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti"
)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Library folder include
add_subdirectory(Libs)

# Link directories setup
target_link_directories(
  ${CMAKE_PROJECT_NAME} PRIVATE # Add user defined library search paths
)

# Add sources to executable
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE # Add user sources here
          Core/Src/entry.cpp
          Core/Src/error_handler.cpp
          Core/Src/App/rtos.cpp
          Core/Src/App/hal_init.cpp
          Core/Src/App/tasks.cpp
          Core/Src/App/interrupts.cpp
          Core/Src/Drivers/irremote.cpp
          Core/Src/Drivers/drive_sys.cpp
          Core/Src/Drivers/hcsr04.cpp)

# Add include paths
target_include_directories(
  ${CMAKE_PROJECT_NAME} PRIVATE # Add user defined include paths
                                Core/Inc/Drivers Core/Inc/App)

# Add project symbols (macros)
target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PRIVATE # Add user defined symbols
)

target_compile_definitions(
  sca PUBLIC # Add user defined symbols for cpp abstraction library
             SCA_GPIO SCA_IWDG SCA_PTIMER)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(
  ${CMAKE_PROJECT_NAME} stm32cubemx # Add user defined libraries
  sca etl::etl)
