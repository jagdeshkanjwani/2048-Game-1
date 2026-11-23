# 2048: Customizable Number Puzzle Game

A console-based 2048 game written in C for the Programming Fundamentals (Lab) course at FAST-NUCES, Karachi.

## Team
- Jagdesh Kanjwani (26K-0567)
- Saqib Raza (26K-0568)
- Sumair Ahmed (26K-0800)

Submitted to: Miss Hijaab Sikander

## Features
- Board sizes from 4x4 to 8x8
- Default target of 2048 or a custom target (power of two, at least 4)
- Easy (no time limit), Medium (1000 moves), Hard (900 moves)
- Arrow-key controls
- Score, highest score, and move counter 
- Post-game menu: New Game, Restart, Exit

## Requirements
- Windows (uses `conio.h` and `windows.h`)
- GCC (MinGW) or any Windows-compatible C compiler

## How to Compile and Run
    gcc 2048.c -o 2048
    2048.exe

## How to Play
Choose a board size, target and difficulty, then use the arrow keys to slide tiles. Equal tiles merge into their sum. Reach the target before the time runs out or the board fills up.

## Limitations
- Highest score is not saved between sessions
- Windows-only
- Limited input validation

## Future Enhancements
- Save high scores to a file
- Graphical interface, colors and sound
- Better input validation
## Cases
Easy level
<img width="960" height="504" alt="image" src="https://github.com/user-attachments/assets/05fcb666-dcdb-4dcf-8f82-2faaef613490" />
Medium level
<img width="960" height="504" alt="image" src="https://github.com/user-attachments/assets/f94cadd8-af00-4b30-a746-a30c4d5088d8" />
Hard level 
<img width="960" height="504" alt="image" src="https://github.com/user-attachments/assets/91e5332f-a532-4e12-99f5-b81d96aa6053" />
