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

* Qt framework (Qt5 or Qt6).
* C++.

## TODO

* Improve enemy wave logic.
* Enhance battery feed selection logic.
* Add smarter decision-making (repair walls, stop production, etc.).
* Introduce additional Clanker types.
* Improve UI and visual feedback.
* Implement a save/load feature.
