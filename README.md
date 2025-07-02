# 🐍 Snake Game WebAssembly

A classic Snake game built with C++ and Raylib, compiled to WebAssembly for web browsers.

## 🎮 Play Now

**[Play the Game](https://nileshmahant.github.io/snakes/)**

## 🕹️ Controls

- **Arrow Keys**: Move the snake
- **Spacebar**: Start the game
- **Q**: Quit to game over
- **R**: Restart from game over screen
- **F** or **Fullscreen Button**: Toggle fullscreen mode

## ✨ Features

- Classic Snake gameplay
- Responsive fullscreen mode
- Smooth animations
- Score tracking
- Food collection with growth mechanics
- Collision detection (walls and self)
- Pulsing food animation
- Retro-style graphics

## 🛠️ Built With

- **C++** - Game logic and structure
- **Raylib** - Graphics and input handling
- **Emscripten** - WebAssembly compilation
- **WebAssembly** - High-performance web execution

## 🚀 Development

### Prerequisites
- Emscripten SDK
- Raylib (web-compiled version)
- Make

### Build Instructions

1. Clone this repository
2. Ensure you have the web-compiled Raylib in `../raylib/src/`
3. Compile:
   ```bash
   emcc main.cc \
  -I../raylib/src \
  -L../raylib/src \
  -lraylib.web \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s FORCE_FILESYSTEM=1 \
  -DPLATFORM_WEB \
  -o snake_game.html
   ```
4. Serve locally:
   ```bash
   python3 -m http.server 8000
   ```

## 📱 Browser Compatibility

- Chrome/Chromium (recommended)
- Firefox
- Safari
- Edge

## 🎯 Game Rules

1. Control the snake to eat the red food
2. Each food item increases your score by 10 points
3. The snake grows longer with each food eaten
4. Avoid hitting the walls or your own body
5. Try to achieve the highest score possible!

## 📄 License

This project is open source and available under the MIT License.