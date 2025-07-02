#include "raylib.h"
#include <vector>
#include <deque>
#include <cmath>

// STRINGS
const char *TITLE = "Snake Game";
const char *START_INST = "Press spacebar to start the game";
const char *MOVE_INST = "Use ARROW KEYS to move";
const char *QUIT_INST = "Press Q for Game Over"; // temporary
const char *GAMEOVER_TEXT = "GAME OVER";
const char *RESTART_INST = "Press R to return to Menu";
// SCREEN
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
// GRID
const int GRID_SIZE = 20;
const int GRID_WIDTH = SCREEN_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE;
// Fonts
const int FS_TITLE = 40;
const int FS_BODY = 20;
const int FS_BIG_TITLE = 60;
enum GameState
{
    MENU,
    PLAYING,
    GAME_OVER
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position
{
    int x, y;

    Position(int x = 0, int y = 0) : x(x), y(y) {}

    // compare equality of two Positions
    bool operator==(const Position &other) const
    {
        return x == other.x && y == other.y;
    }
};

class Snake
{
private:
    std::deque<Position> body;
    Direction direction;
    Direction nextDirection;
    float moveTimer;
    float moveDelay;

public:
    Snake()
    {
        reset();
    }
    void draw()
    {
        for (size_t i = 0; i < body.size(); i++)
        {
            Color segmentColor;
            if (i == 0)
            {
                // give a brighter color to head
                segmentColor = LIME;
            }
            else
            {
                // give a darker color to body
                segmentColor = GREEN;
            }

            DrawRectangle(
                body[i].x * GRID_SIZE,
                body[i].y * GRID_SIZE,
                GRID_SIZE - 1, // for a small gap between segments
                GRID_SIZE - 1,
                segmentColor);
        }
    }
    void move()
    {
        direction = nextDirection;
        Position newHead = body.front();
        switch (direction)
        {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
        }
        body.push_front(newHead);
        body.pop_back(); // remove tail (TODO: modify when eating food)
    }
    void grow() {
        body.push_back(body.back()); // adds a new segment on the tail
    }
    void update(float deltaTime)
    {
        handleInput();
        moveTimer += deltaTime;
        if (moveTimer >= moveDelay)
        {
            move();
            moveTimer = 0.0f;
        }
    }
    void reset()
    {
        body.clear();
        // start with a three segment snake in the middle
        body.push_back(Position(GRID_WIDTH / 2, GRID_HEIGHT / 2));
        body.push_back(Position(GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2));
        body.push_back(Position(GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2));
        direction = RIGHT;
        nextDirection = RIGHT;
        moveTimer = 0.0f;
        moveDelay = 0.15f; // move every .15 seconds
    }
    void handleInput()
    {
        if (IsKeyPressed(KEY_UP) && direction != DOWN)
        {
            nextDirection = UP;
        }
        else if (IsKeyPressed(KEY_DOWN) && direction != UP)
        {
            nextDirection = DOWN;
        }
        else if (IsKeyPressed(KEY_LEFT) && direction != RIGHT)
        {
            nextDirection = LEFT;
        }
        else if (IsKeyPressed(KEY_RIGHT) && direction != LEFT)
        {
            nextDirection = RIGHT;
        }
    }
    bool checkWallCollision() {
        Position head = body.front();
        return  head.x < 0 || head.x >= GRID_WIDTH ||
                head.y < 0 || head.y >= GRID_HEIGHT;
    }
    bool checkSelfCollision() {
        Position head = body.front();
        for(size_t i = 1; i < body.size(); i++) {
            if(head == body[i]) {
                return true;
            }
        }
        return false;
    }
    Position getHeadPosition() {
        return body.front();
    }
    size_t getLength() {
        return body.size();
    }
    const std::deque<Position>& getBody() const {
        return body;
    }
};

class SnakeGame
{
private:
    GameState currentState;
    Snake snake;
    Position food;
    int score;

public:
    SnakeGame() : currentState(MENU), score(0)
    {
        // initialize the window
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
        // set frame rate
        SetTargetFPS(60);
        // generate food
        generateFood();
    }

    void update()
    {
        float deltaTime = GetFrameTime();
        switch (currentState)
        {
        case MENU:
            updateMenu();
            break;
        case PLAYING:
            updateGame(deltaTime);
            break;
        case GAME_OVER:
            updateGameOver();
            break;
        }
    }
    void updateMenu()
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            startNewGame();
        }
    }
    void updateGame(float deltaTime)
    {
        snake.update(deltaTime);
        // check collisions
        if(snake.checkWallCollision() || snake.checkSelfCollision()) {
            currentState = GAME_OVER;
            return;
        }
        //check food collision
        if(snake.getHeadPosition() == food) {
            snake.grow();
            score += 10;
            generateFood();
        }
        if (IsKeyPressed(KEY_Q))
        {
            currentState = GAME_OVER;
        }
    }
    void updateGameOver()
    {
        if (IsKeyPressed(KEY_R))
        {
            currentState = MENU;
        }
    }

    void draw()
    {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        switch (currentState)
        {
        case MENU:
            drawMenu();
            break;
        case PLAYING:
            drawGame();
            break;
        case GAME_OVER:
            drawGameOver();
            break;
        }
        EndDrawing();
    }
    void drawMenu()
    {
        // Draw Title text
        DrawText(
            TITLE,
            SCREEN_WIDTH / 2 - MeasureText(TITLE, FS_TITLE) / 2,
            SCREEN_HEIGHT / 2 - FS_TITLE / 2,
            FS_TITLE,
            WHITE);

        // Draw Starting instruction
        DrawText(
            START_INST,
            SCREEN_WIDTH / 2 - MeasureText(START_INST, FS_BODY) / 2,
            SCREEN_HEIGHT / 2 + FS_TITLE,
            FS_BODY,
            LIGHTGRAY);

        DrawText(
            MOVE_INST,
            SCREEN_WIDTH / 2 - MeasureText(MOVE_INST, FS_BODY) / 2,
            SCREEN_HEIGHT / 2 + 60,
            FS_BODY,
            GRAY);
    }
    void drawGame()
    {

        // Draw Gridline for debugging
        drawGrid();
        // Draw snake
        snake.draw();

        // Draw Food with pulsing effect
        float pulse = (sin(GetTime() * 0.8f) + 1.0f) * 0.5f;
        Color foodColor = ColorAlpha(RED, 0.7f + pulse * 0.3f);

        DrawRectangle(
            food.x * GRID_SIZE + 2,
            food.y * GRID_SIZE + 2,
            GRID_SIZE - 4,
            GRID_SIZE - 4,
            foodColor
        );

        // add highlight to food
        DrawRectangle(
            food.x * GRID_SIZE + 4,
            food.y * GRID_SIZE + 4,
            4,
            4,
            WHITE
        );

        // Draw UI
        DrawText(
            TextFormat("Score: %d", score),
            10, 10, FS_BODY, WHITE);
        
        DrawText(
            TextFormat("Length: %d", int(snake.getLength())),
            10, 35, FS_BODY, WHITE);

        DrawText(
            QUIT_INST,
            10,
            SCREEN_HEIGHT - 30,
            FS_BODY,
            GRAY);
    }
    void drawGameOver()
    {
        DrawText(
            GAMEOVER_TEXT,
            SCREEN_WIDTH / 2 - MeasureText(GAMEOVER_TEXT, FS_BIG_TITLE) / 2,
            SCREEN_HEIGHT / 2 - FS_BIG_TITLE / 2,
            FS_BIG_TITLE,
            RED);

        DrawText(
            RESTART_INST,
            SCREEN_WIDTH / 2 - MeasureText(RESTART_INST, FS_BODY) / 2,
            SCREEN_HEIGHT / 2 + FS_BODY * 2,
            FS_BODY,
            WHITE);
    }
    void drawGrid()
    {
        for (int i = 0; i <= GRID_WIDTH; i++)
        {
            DrawLine(
                i * GRID_SIZE,
                0,
                i * GRID_SIZE,
                SCREEN_HEIGHT,
                ColorAlpha(WHITE, 0.1f));
        }
        for (int i = 0; i <= GRID_HEIGHT; i++)
        {
            DrawLine(
                0,
                i * GRID_SIZE,
                SCREEN_WIDTH,
                i * GRID_SIZE,
                ColorAlpha(WHITE, 0.1f));
        }
    }

    void startNewGame()
    {
        snake.reset();
        score = 0;
        generateFood();
        currentState = PLAYING;
    }

    void generateFood()
    {
        std::vector<Position> availablePositions;
        // Find all positions which are not occupied by snake
        for(int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                Position pos(x, y);
                bool occupied = false;

                // Check if position is occupied by snake
                for(const Position& segment : snake.getBody()) {
                    if (segment == pos) {
                        occupied = true;
                        break;
                    }
                }
                if(!occupied) {
                    availablePositions.push_back(pos);
                }
            }
        }
        if(!availablePositions.empty()) {
            int randomIndex = GetRandomValue(0, availablePositions.size() - 1);
            food = availablePositions[randomIndex];
        }
    }
};

int main()
{

    SnakeGame game;

    while (!WindowShouldClose())
    {
        game.update();
        game.draw();
    }

    CloseWindow();
    return 0;
}