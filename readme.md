# 🐍 Slither - SDL2 Snake Game

A classic Snake game built using **C++** and **SDL2**, featuring dynamic snake movement, apple generation, score tracking, and text rendering.

![Slither Gameplay](https://raw.githubusercontent.com/frizzflicks/Snake/refs/heads/master/play.png)

---

## 🎮 Features

- 🍎 Random apple generation
- 🧠 Self-collision detection
- 🟩 Smooth grid-based movement
- 🧱 Restart and game-over logic
- 🖋️ Score rendering with SDL_ttf

---

## 🛠️ Requirements

- C++17 compatible compiler  
- [SDL2](https://www.libsdl.org/)  
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)  
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)  

> You can install these using your system’s package manager. For example:
```bash
# Debian/Ubuntu
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```
🚀 Build & Run
```bash
g++ main.cpp -o slither -lSDL2 -lSDL2_ttf -lSDL2_image
./slither
```
⌨️ Controls
Key	Action
```
↑ ↓ ← →	Move Snake
Enter	Start / Restart
```

📂 File Structure
```bash
.
├── main.cpp        # Main game code
├── text.ttf        # Font used for rendering text
└── README.md       # This file
```
📸 Preview
<img src="https://raw.githubusercontent.com/frizzflicks/Snake/refs/heads/master/play.png" alt="Score Display" width="400" />
📜 License
MIT License. Feel free to use and modify.

💬 Credits
Made with 🧠, 🎮 and a lot of debugging by Taranveer Singh.
