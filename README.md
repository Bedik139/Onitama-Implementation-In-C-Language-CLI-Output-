# Onitama - Terminal-Based Strategy Game

A feature-rich implementation of the classic two-player abstract strategy board game **Onitama** in C, designed for CCPROG2 Machine Project. This terminal-based version combines elegant ASCII art with strategic gameplay, bringing the essence of chess and martial arts to your command line.

![Version](https://img.shields.io/badge/version-1.0-blue)
![Language](https://img.shields.io/badge/language-C-brightgreen)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

## 📋 Table of Contents
- [About the Game](#about-the-game)
- [Features](#features)
- [Requirements](#requirements)
- [Installation & Setup](#installation--setup)
- [How to Play](#how-to-play)
- [Game Controls](#game-controls)
- [File Structure](#file-structure)
- [Technical Details](#technical-details)
- [Screenshots](#screenshots)
- [Author](#author)
- [Acknowledgements](#acknowledgements)

## 🎮 About the Game

Onitama is a fast-paced, abstract strategy game for two players. Each player controls a **Sensei** (Master) and four **Students**, moving them on a 5×5 board according to unique movement cards. The game combines deep strategy with elegant simplicity.

### Win Conditions
1. **Way of the Stone**: Capture your opponent's Sensei
2. **Way of the Stream**: Move your Sensei to your opponent's temple (center of their back row)

## ✨ Features

- **Interactive Menu System**: Navigate through options using arrow keys
- **Turn-Based Gameplay**: Strategic two-player local gameplay
- **16 Unique Movement Cards**: Each card offers different movement patterns
- **Card Shuffling & Distribution**: Random card assignment for varied gameplay
- **Visual Card Display**: See your movement options in ASCII art format
- **Color-Coded Pieces**: 
  - 🔵 Player 1 (Blue)
  - 🔴 Player 2 (Red)
- **Hall of Fame**: Leaderboard tracking winners by number of turns
- **Game Replay System**: Watch a playback of the complete match
- **Move Validation**: Automatic checking of valid moves and pieces
- **ANSI Escape Codes**: Enhanced visual experience with colors

## 💻 Requirements

### System Requirements
- **Operating System**: Windows 11 (or Windows 10)
- **Compiler**: GCC (MinGW) or any C compiler supporting Windows.h
- **Terminal**: Windows Command Prompt or compatible terminal with ANSI support

### Dependencies
- `stdio.h` - Standard I/O operations
- `stdlib.h` - Memory allocation and system functions
- `conio.h` - Console I/O (Windows-specific)
- `string.h` - String manipulation
- `time.h` - Time functions for random seed
- `windows.h` - Windows-specific functions (Sleep)

## 🔧 Installation & Setup

### Step 1: Clone or Download
Download all project files to a local directory:
```
ccprog2mpdeliverables/
├── SANTOS_ONITAMA.c
├── movecards.txt
├── *.txt (card files: Tiger.txt, Dragon.txt, etc.)
└── HallOfFame.txt (created automatically)
```

### Step 2: Ensure All Files Are Present
The following files are **required**:
- `SANTOS_ONITAMA.c` - Main source code
- `movecards.txt` - Card configuration file
- **16 Card Files**: Boar.txt, Cobra.txt, Crab.txt, Crane.txt, Dragon.txt, Eel.txt, Elephant.txt, Frog.txt, Goose.txt, Horse.txt, Mantis.txt, Monkey.txt, Ox.txt, Rabbit.txt, Rooster.txt, Tiger.txt

### Step 3: Compile the Program
Using GCC (MinGW):
```bash
gcc SANTOS_ONITAMA.c -o onitama.exe
```

Or with Code::Blocks/Dev-C++:
- Open the .c file in your IDE
- Build and run the project

### Step 4: Run the Game
```bash
onitama.exe
```

## 🎯 How to Play

### Game Setup
1. The game shuffles 16 movement cards
2. Each player receives 2 cards
3. 1 neutral card is placed in the middle
4. The color of the neutral card determines who goes first

### Turn Structure
On your turn, you must:
1. **Select a movement card** (LEFT/RIGHT arrow keys)
2. **Choose a piece to move** (Arrow keys)
3. **Select destination** (Arrow keys, must match card pattern)
4. **Confirm with ENTER**

After moving, your used card becomes the neutral card, and you take the previous neutral card.

### Special Rules
- Each card has a unique movement pattern (shown in ASCII)
- You cannot move to a square occupied by your own piece
- Landing on an opponent's piece captures it
- If you have no valid moves, you must still swap a card

### Card Format
Cards are displayed in a 5×5 grid:
- `X` - Sensei's position (center)
- `x` - Possible movement destinations
- `.` - Cannot move here

Example (Tiger card):
```
..x..
.....
..X..
..x..
.....
```

## 🎮 Game Controls

### Main Menu
- **↑/↓ Arrow Keys**: Navigate menu options
- **ENTER**: Select option
- **Options**: Start Game, Hall of Fame, How to Play, Quit

### In-Game
- **↑↓←→ Arrow Keys**: Navigate board/select positions
- **LEFT/RIGHT Arrow Keys**: Choose movement card
- **ENTER**: Confirm selection
- **Visual Indicators**: `>piece<` shows selected position

## 📁 File Structure

```
ccprog2mpdeliverables/
│
├── SANTOS_ONITAMA.c              # Main game source code
├── README.md                      # This file
├── DECLARATION_OF_ORIGINAL_WORK.txt
│
├── movecards.txt                  # Card configuration (lists all 16 cards)
├── HallOfFame.txt                 # Leaderboard (auto-generated)
│
└── Card Files (16 total):
    ├── Boar.txt
    ├── Cobra.txt
    ├── Crab.txt
    ├── Crane.txt
    ├── Dragon.txt
    ├── Eel.txt
    ├── Elephant.txt
    ├── Frog.txt
    ├── Goose.txt
    ├── Horse.txt
    ├── Mantis.txt
    ├── Monkey.txt
    ├── Ox.txt
    ├── Rabbit.txt
    ├── Rooster.txt
    └── Tiger.txt
```

## 🔍 Technical Details

### Key Data Structures
- **`player`**: Stores player information (name, cards, pieces, turns)
- **`oniBrd`**: 5×5 board array with ANSI color codes
- **`dataCrdsIG`**: Manages game cards (left, right, neutral)
- **`MoveRecord`**: Records moves for replay functionality

### Key Functions
- **`shuffleCards()`**: Randomly shuffles the 16 movement cards
- **`assignMoveCards()`**: Distributes cards to players
- **`isValidMove()`**: Validates moves according to card patterns
- **`checkWinCondition()`**: Checks for game-ending conditions
- **`gameTurn()`**: Manages player turn logic
- **`replayMovesSequence()`**: Replays the entire game

### Board Representation
- **Empty Space**: `| _ |`
- **Player 1 Sensei**: `| M |` (Blue)
- **Player 1 Student**: `| s |` (Blue)
- **Player 2 Sensei**: `| M |` (Red)
- **Player 2 Student**: `| s |` (Red)

### Color Codes (ANSI Escape Sequences)
- **Blue** (`\x1b[34m`): Player 1
- **Red** (`\x1b[31m`): Player 2
- **Cyan** (`\x1b[36m`): Headings/Borders
- **Yellow** (`\x1b[33m`): Prompts
- **Magenta** (`\x1b[35m`): Selection indicators
- **Green** (`\x1b[32m`): Success messages

## 📸 Screenshots

### Main Menu
```
+===========================+
|       ONITAMA GAME        |
+===========================+

   Use ARROWKEYS to move, Enter to select.

 -> Start Game <-
    Hall of Fame
    How to Play
    Quit
```

### Game Board
```
                              O N I T A M A

 [Player 2] Alice's Cards:
                        |  ..x.. |        |  ..... |
                        |  ...x. |        |  ..x.. |
                        |  ..X.. |        |  ..X.x |
                        |  ...x. |        |  ..x.. |
                        |  ..x.. |        |  ..... |
                          Cobra                Horse
```

## 👤 Author

**John Benedict G. Santos**
- **Section**: S11
- **DLSU ID**: 12486566
- **Course**: CCPROG2 Machine Project
- **Date**: July 23, 2025
- **Version**: 1.0

## 🙏 Acknowledgements

This project was developed with reference to the following resources:

1. **ANSI Escape Codes**: [ANSI Escape Sequences Gist](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797)
   - Used for terminal color formatting and visual enhancements

2. **Console I/O (`conio.h`)**: [Stack Overflow - Why use conio.h](https://stackoverflow.com/questions/59812014/why-use-conio-h)
   - Understanding Windows-specific console input functions

3. **Random Number Generation**: [Stack Overflow - srand and rand](https://stackoverflow.com/questions/52801380/srandtimenull-function)
   - Implementing proper random number seeding with `srand(time(NULL))`

4. **Onitama Board Game**: Inspired by the original board game by Shimpei Sato

---

## 📝 Declaration

*This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned. I have constructed the functions and their respective algorithms and corresponding code individually. The program was run, tested, and debugged by my own efforts. I further certify that I have not copied in part or whole or otherwise plagiarized the work of other students and/or persons.*

---

## 📄 License

This project was created for educational purposes as part of CCPROG2 coursework at De La Salle University.

---

### 🎮 Enjoy Your Game of Onitama! 🎮

*May the best strategist win!*
