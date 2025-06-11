# Dino RL Bot

A simple reinforcement learning bot for a Dino game clone built using SFML in C++. The bot learns to avoid cacti by jumping, using a Q-learning algorithm.

## Prerequisites
- SFML library installed
- C++ compiler (e.g., g++)
- Assets:
- `Dino.png`: Image of the dinosaur
- `cactus.png`: Image of the cactus
- `Arial.ttf`: Font file (available in Windows Fonts directory or provide your own)

## How to Run
1. Install SFML (https://www.sfml-dev.org/).
2. Place the required assets in the `assets/` folder.
3. Compile and run `src/main.cpp` using a C++ compiler.
Example:
```bash
g++ -c src/main.cpp -I/path/to/sfml/include
g++ main.o -o DinoRLBot -L/path/to/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
./DinoRLBot