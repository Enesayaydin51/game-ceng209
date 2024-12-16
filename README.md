# game-ceng209
# Dungeon Adventure Game

## Project Overview
This project is a dungeon exploration game developed in C. The player moves through various rooms, fights creatures, collects items, and tries to develop their character. The game uses file-based storage to manage data for rooms, creatures, items, and inventory.

## Features
- **Room Exploration**: Players can move through randomly generated rooms.
- **Combat with Creatures**: Players can fight creatures found in rooms. Items can be picked up only after defeating the creatures.
- **Inventory Management**: Players can add collected items to their inventory.
- **Save and Load**: Game state can be saved to a file and loaded later.
- **Command System**: Players can use various commands like move, look, manage inventory, and engage in combat.

## Installation

### Requirements
- C Compiler (e.g., GCC)
- Terminal or Command Line Access

### Installation Steps

1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/yourusername/dungeon-adventure-game.git

2.Navigate to the project directory:
cd dungeon-adventure-game

3.Compile the project:
gcc -o dungeon_game main.c -lm

4.Run the game:
./dungeon_game

## Usage
Game Commands
move <direction>: Moves the player in a specific direction (north, south, east, west).
look: Displays the description of the current room.
pickup: Takes an item from the room. (Items cannot be taken if creatures are alive in the room.)
attack: Engages in combat with a creature in the room.
inventory: Displays the player's inventory.
save: Saves the current game state.
load: Loads a previously saved game.
exit: Exits the game.

## Structures
Room Structure
Each room contains descriptions, creatures, traps, and items. When the player enters a room, the content of the room is checked.

Creature Structure
Creatures have attributes like health and attack power. Players can engage in combat with creatures.

Inventory Structure
Players store collected items in their inventory. These items can be used during combat or other interactions.
