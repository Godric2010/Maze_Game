# Readme Maze Game

[![CI](https://github.com/Godric2010/Maze_Game/actions/workflows/build_and_test.yml/badge.svg?branch=master)](https://github.com/Godric2010/Maze_Game/actions/workflows/build_and_test.yml?query=branch%3Amaster)

Maze Game is a training and showcase project. It features a simple maze puzzle game, written in a custom C++ engine.
The emphasis lies more on the [engine](/engine/Readme.md) development and architecture, than on the gameplay part.
The project is primarily designed to learn about engine development and to showcase my development over time.
So every curious C++/Engine Dev or Recruiter is welcome to take a look.

## Features

- Custom C++ Engine
    - OpenGL Render support
    - Custom Entity-Component-System
    - Custom Collision Physics calculation
    - UI Support
    - Scene Management
    - Automatic system detection and code-generated registration
    - Debug User Interface
- Procedurally generated mazes
- Various sized levels

## Platforms

While primarily developed on macOS, Windows compatibility is also supported.
Linux is not officially supported.

## Requirements

### Compiler

- macOS: clang
- Windows: MSVC

### Build Tool

- CMake

### Package Manager

- vcpkg manifest

## How to run

The simplest way to run this project is to open it in the latest CLion IDE and making sure, the vcpkg Package Manager is
installed.

## Controls

- WASD to control the camera
- Mouse Movement to rotate the camera

Game Controllers are not (yet) supported.

## Structure

- Engine: Contains the logic of the engine itself
- Gameplay: Contains the logic of the game, using the engine
- Resources: Holds the resources used for the game
- Generated: Contains auto-generated code (Do not temper with!)
- Scripts: Contains scripts for coverage reports and other utilities for GitHub Actions
- Tools: Contains scripts for at-build scripts, like code gen

## License

The project falls under the licensing of public domain. Use it, modify it and tinker with it however you like.