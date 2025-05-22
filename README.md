# Self Playing Scrabble Game – INF3002L Project

This project is a Scrabble game simulator built upon the base code provided in the INF3002L course at Université Lyon 1. The original code included the Scrabble board implementation (board, spots, and bonus system). I expanded on this by implementing core gameplay logic and word validation using the **GADDAG** data structure for high-performance word search and validation.

## 📁 Project Structure

### 🔹 Instructor-Provided Base
- `board.*`: Implementation of the standard Scrabble board
- `spots.*`: Logic for handling individual spots on the board
- `bonus.*`: Setup for word and letter bonus positions
- `Makefile`: enabling the make command to compile the files.

### 🔹 Implemented Files

#### Word Dictionary & Search
- `GaddagNode.cpp / GaddagNode.hpp`:  
  Implements a GADDAG node, using an `unordered_map` to hold child nodes and a `bool` flag for terminal word detection.
  
- `gaddag.cpp / gaddag.hpp`:  
  Builds the GADDAG structure from a dictionary file. Each word is transformed into multiple prefix-inverted forms with an inserted `'+'` to allow bidirectional word building (as per standard GADDAG construction).

#### Game State & Letter Management
- `sac.cpp / sac.hpp`:  
  Models the Scrabble letter bag. Stores remaining letters and their frequencies. Used to generate random letter racks (`LettresJoueurs`) according to Scrabble letter distribution.

- `LettresJoueurs.cpp / LettresJoueurs.hpp`:  
  Represents the current letter rack of a player (7 letters max, refilled from `sac` as long as letters remain).

- `etat.cpp / etat.hpp`:  
  Encodes the current game state during word generation. Tracks:
  - Current board configuration
  - Accumulated score and word multipliers
  - Side word scores

#### Entry Point
- `test_board.cpp`:  
  Main driver for the simulation. It initializes the game from the center of the board and plays valid moves iteratively until no more moves can be made or no letters remain.

## 🔍 Gameplay Logic

### Word Placement
- Starts at the center of the board.
- At each step, the program searches for valid words by exploring **spots adjacent to already occupied ones**.

### Move Generation
- For each starting position and direction, possible paths are explored:
  - Add a letter from the current rack
  - Insert a `'+'` (indicating a change of direction in the GADDAG)
  - Terminate the word

Each action checks the validity of the move using the GADDAG structure. If the current state (`etat`) is valid, it is pushed onto a stack for further exploration.

### Side Word Scoring
- Each placed letter is checked for **side word creation** using the following functions in test_board.cpp:
  - `sideWordCheckerVertical`
  - `sideWordCheckerHorizontal`

These functions compute scores for any additional words formed in the perpendicular direction and return their values.

## ♟ Simulation Flow
1. Initialize board and dictionary (GADDAG).
2. Randomly generate 7-letter racks from `sac`.
3. Start game at the center of the board.
4. Loop:
   - Search for valid moves starting from eligible spots
   - Generate word paths using DFS and GADDAG transitions
   - Place highest scoring move
   - Update board and refill letter rack

5. End game when no valid moves remain or `sac` is empty.

## 📌 Notes
- All dictionary words are transformed into GADDAG format to allow efficient prefix+suffix bidirectional lookup.
- Word scoring includes bonuses and side word validation at each move.

## 🧠 Future Improvements
- Adding the bonus letter to the game
- Optimize state pruning for larger dictionaries
- Add support for multiplayer rounds
- Implement a GUI or visual board output
