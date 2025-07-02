for web-

emcc main.cc \
  -I../raylib/src \
  -L../raylib/src \
  -lraylib.web \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s TOTAL_MEMORY=67108864 \
  -s FORCE_FILESYSTEM=1 \
  -DPLATFORM_WEB \
  -o index.html     


for desktop-

g++ main.cc -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -o snake_game