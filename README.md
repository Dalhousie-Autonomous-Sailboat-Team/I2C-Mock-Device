# I2C-Mock-Device

Mock I2C Client with programmable address and register contents

This revision is based around the STM32F411RE Nucleo board microcontroller.  This project is intended to be developed outside of the STM32CubeIDE environment.  Instead, STM32CubeMX is used for pin configuration and peripheral initialization, and binaries are generated using the CMake build system.  VSCode with the STM32 and Cortex-Debug extensions is recommended for development.

## Project Structure

- `cmake/`: CubeMX generated CMake files
- `Core/`: CubeMX generated source files - **main.c is in this directory**
- `Drivers/`: External drivers and libraries
- **`User/`**: User created source files - **This is where you should put your code**
- `build/`: Temporary Build directory
- CMakelists.txt: Main CMake file - **Add any new source files here**

## Getting Started

**This guide assumes you are using VSCode.**

1. Ensure **Cortex-Debug** and **STM32 VS Code Extension** are installed in VSCode.
2. Ensure **STM32CubeCLT** is installed on your system and added to your PATH.
3. Ensure **STM32CubeMX** is installed on your system.
4. Open the project folder in VSCode.
5. Run the `CMake: Configure` command from the command palette.
6. Run the `CMake: Build` command from the command palette.
7. Ensure the board is powered on.
8. Run the `CMake: Debug` command from the command palette to start a debug session.