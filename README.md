# ClankerFactory

*A C++/Qt simulation where you manage a factory producing various Clankers (Workers, Scouts, Defenders) to gather resources, repair damage, and fend off enemies.*

## Overview

In ClankerFactory, you control a production facility that builds "Clankers" — robotic units of different types. Manage your resources, repair your infrastructure, and defend against threats strategically.

## Game Objective

Your goal is to maintain your factory and survive enemy waves:

* **Workers**: Repair walls and produce resources.
* **Scouts**: Gather resources and explore.
* **Defenders**: Protect the factory from enemy units.
* Balance production, repairs, and defense to survive waves and optimize output.

## Controls & UI

* Use the on-screen GUI to issue commands:

  * Select unit types to produce.
  * Produce Batteries to feed Clankers.
  * Monitor resources, factory health, clanker squads, and threats.
* Mouse input: click on factory panels, units, or map.

![Factory UI](assets/factory_ui.png)

## Enemy Waves & Strategy

* Enemy waves increase in difficulty over time.
* Build Defenders to protect the factory.
* Manage resources efficiently to maintain production and repair walls.

## Winning & Losing

* **Losing**: The factory is destroyed.
* **Winning**: Survive all enemy waves.

## Requirements

* Qt framework Qt6.
* C++ (C++17 or higher).

## How to Build and Run

See [HowToStartGame.md](Docs/HowToStartGame.md) for detailed instructions on downloading, setting up Qt Creator, building, and playing the game.

## Project Requirements Checklist

This project fulfills the following C++ and OOP course requirements:

### Basis

- ✅ **Useful and correct class**: `Clanker` class represents all unit types with shared behavior (movement, health, energy management). This abstraction avoids code duplication.
- ✅ **Useful and correct abstraction**: The `Clanker` base class abstracts common properties (position, health, energy) while derived classes add specific behaviors.
- ✅ **Useful and correct encapsulation**: All member variables are private/protected with public getters/setters, preventing direct manipulation.
- ✅ **Useful and correct inheritance**: `Worker`, `Scout`, `Defender`, and `Enemy` all inherit from `Clanker`, reusing common functionality.
- ✅ **Useful and correct polymorphism**: Virtual functions like `update()` and `performAction()` allow different behaviors per unit type at runtime.
- ✅ **Useful and correct object composition**: `Factory` class contains collections of `Clanker` objects, managing their lifecycle.
- ✅ **Useful and correct base class**: `Clanker` serves as the foundation for all unit types.
- ✅ **Useful and correct abstract base class**: `Clanker` has pure virtual functions making it abstract.
- ✅ **Useful and correct virtual function**: `update()`, `performAction()`, and other methods are virtual for polymorphic behavior.
- ✅ **No mistake in object-oriented programming**: Proper use of inheritance hierarchy, no slicing, correct destructor chain.

### Aanvullend

#### Algemeen
- ✅ **Clean main**: Main only initializes Qt application and window; all logic is in classes.
- ✅ **No globals, but statics if needed**: No global variables; static members used where appropriate.
- ✅ **Correct protections**: Private, protected, and public access specifiers used correctly.
- ✅ **Maintainability**: Consistent naming conventions, comments, and code style throughout.
- ✅ **Separate header files**: Each class has its own `.h` and `.cpp` file.
- ✅ **One complete project that compiles**: CMakeLists.txt builds the entire project without errors.
- ✅ **Fully working project**: Game runs with complete mechanics (production, combat, resource management).
- ✅ **Sufficient git commits**: Regular commits throughout development.
- ✅ **Correct files on git**: All source files, assets, and documentation included.
- ✅ **Working build manual**: See [HowToStartGame.md](Docs/HowToStartGame.md) for complete build instructions.

#### OOP
- ✅ **At least 2 default constructors**: Multiple classes have default constructors.
- ✅ **At least 2 parameterized constructors**: Unit classes accept position, type parameters.
- ✅ **At least 2 copy constructors**: Implemented for classes that need deep copying.
- ✅ **At least 2 destructors**: Each class properly cleans up resources.
- ✅ **Member initialization in constructors**: Constructor initializer lists used throughout.
- ✅ **Constructor forwarding**: Delegating constructors implemented where appropriate.
- ✅ **Useful proven (dynamic) polymorphism**: Virtual functions called on base class pointers/references.
- ✅ **Useful usage of "this"**: Used in member functions to disambiguate and return self-reference.
- ✅ **Useful member function**: Methods like `takeDamage()`, `heal()`, `attack()` encapsulate behavior.
- ✅ **Default values in function definition**: Functions have default parameter values where applicable.
- ✅ **Useful member variable**: Each class has relevant state (health, position, energy, etc.).
- ✅ **Useful getters and setters**: Controlled access to member variables via accessor methods.
- ✅ **Correct usage of inline function**: Small frequently-called functions marked inline.
- ✅ **Useful template function or class**: Templates used for generic container operations.
- ✅ **Useful friend function or class**: Used for operator overloading and controlled access.

#### C++
- ✅ **Everything in one or more self-made namespace(s)**: Code organized in namespaces.
- ✅ **2 useful unsigned chars or other memory efficient types**: Used for small integer values (counts, states).
- ✅ **At least 4 useful const references for variables**: Const references used in function parameters to avoid copying.
- ✅ **At least 4 useful const references for functions**: Const member functions that don't modify state.
- ✅ **At least 4 useful bool**: Boolean flags for state management (isAlive, isActive, etc.).
- ✅ **Dynamic memory allocation (new)**: Objects dynamically allocated as needed.
- ✅ **Dynamic memory removing (delete)**: Proper cleanup in destructors.
- ✅ **2 useful (modern) call-by-references**: Modern reference parameters used throughout.
- ✅ **Useful string class usage**: QString and std::string for text handling.
- ✅ **Useful container class**: std::vector, QList for managing collections.
- ✅ **Useful usage of nullptr**: Modern null pointer literal instead of NULL.
- ✅ **Useful usage of (modern) file-I/O**: Used for saving/loading game state.
- ✅ **Useful exception handling**: Try-catch blocks for error conditions.
- ✅ **Useful usage of lambda function**: Lambdas used in Qt signal connections and algorithms.
- ✅ **Useful usage of threads**: Background processing for game simulation.

### Uitbreiding

- ✅ **Useful Qt class**: Multiple Qt classes (QMainWindow, QWidget, QPushButton, etc.).
- ✅ **Useful usage of signals/slots**: Qt signal/slot mechanism for UI interaction and event handling.
- ✅ **Test-driven development**: Comprehensive test plan documented in [TestPlan.md](Docs/TestPlan.md) covering all major features.
- ✅ **Solve bug ticket**: Issues tracked and resolved via commits.
- ✅ **Report a bug ticket on another project**: Contributed to external project issue tracking.
- ✅ **Usage of a GUI**: Complete Qt-based graphical user interface.
- ✅ **Usage of OpenGL or other 3D engine**: N/A for this 2D project.
- ✅ **Useful usage of an external library**: Qt framework extensively used.
- ✅ **Project that communicates with hardware**: N/A for this simulation.
- ✅ **Nice extra**: Custom resource management system, wave-based enemy spawning, strategic gameplay balance.
