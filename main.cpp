#include "include/raylib.h"
#include "include/raymath.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <deque>

using namespace std;

Color background = {255, 165, 158, 255};

int grid_count = 25;
int grid_size = 30;
vector<vector<bool>> grid(grid_count, vector<bool> (grid_count, 0));

double currTime = 0;
double tickSpeed = 0.2;
bool tickCoolDown = false;
bool gameover = false;

map<string,Vector2> moveDir = {{"up",Vector2{0,-1}},
  {"down",Vector2{0,1}},
  {"right",Vector2{1,0}},
  {"left",Vector2{-1,0}}
};

bool isMoveTick(double ts) {
  double gameTime = GetTime();
  if(gameTime - currTime >= ts) {
    currTime = gameTime;
    tickCoolDown = false;
    return true;
  }
  return false;
}

class Food {
  public:
    Texture2D texture;
    Vector2 pos = getRandomPosition();
    int radius = 10;
    Color color = WHITE;

    Food() {
      Image image = LoadImage("Assets/quartz.png"); //load the image into the memory
      texture = LoadTextureFromImage(image); //texture is optimized for rendering so in game dev we usually use texture rather than image
      UnloadImage(image); //therefore, we free the memory for image ;)
    }

    ~Food() {
      UnloadTexture(texture);
    }

    void draw() {
      DrawTexture(texture, pos.x * grid_size, pos.y * grid_size, WHITE);
    }

    Vector2 getRandomPosition() {
      Vector2 pos;
      do {
        pos.x = GetRandomValue(0, grid_count - 1);
        pos.y = GetRandomValue(0, grid_count - 1);
      }while (grid[pos.x][pos.y]);
      return pos;
    }
};

class Snake {
  public:
    deque<Vector2> body = {Vector2{12,10}, Vector2{12,11},Vector2{12,12}};
    Vector2 direction = Vector2{0,-1};
    Color color = {196, 64, 55, 255};

    Snake() {
      for (Vector2 part : body) {
        grid[part.x][part.y] = 1;
      }
    }

    void draw() {
      for(Vector2 pos : body) {
        Rectangle part = Rectangle{pos.x * grid_size, pos.y * grid_size, (float)grid_size, (float)grid_size};
        DrawRectangleRounded(part, 0.6, 1, color);
      }
    }

    void move(Vector2 foodPos,Food& food) {
      Vector2 head = Vector2Add(body.front(),direction);
      
      if (head.x<0 || head.x>=grid_count || head.y<0 || head.x >=grid_count || grid[head.x][head.y] == 1) gameover = 1;

      grid[head.x][head.y] = 1;
      body.push_front(head);

      if (head == foodPos) {
        food.pos = food.getRandomPosition();
        return;
      }
      
      grid[body.back().x][body.back().y] = 0;
      body.pop_back();
    }

};

void checkKeyInput(Snake& snake) {
  if(IsKeyPressed(KEY_DOWN) && snake.direction!=moveDir["up"]) {
    snake.direction = moveDir["down"];
    tickCoolDown = true;
  }
  if(IsKeyPressed(KEY_UP) && snake.direction!=moveDir["down"]) {
    snake.direction = moveDir["up"];
    tickCoolDown = true;
  }
  if(IsKeyPressed(KEY_LEFT) && snake.direction!=moveDir["right"]) {
    snake.direction = moveDir["left"];
    tickCoolDown = true;
  }
  if(IsKeyPressed(KEY_RIGHT) && snake.direction!=moveDir["left"]) {
    tickCoolDown = true;
    snake.direction = moveDir["right"];
  }
}

class Game {
  public:
    Snake snake;
    Food food = Food();

    void update() {
      if(!tickCoolDown) checkKeyInput(snake);
      if(isMoveTick(tickSpeed)) snake.move(food.pos, food);
    }

    void draw() {
      snake.draw();
      food.draw();

      // for (int i=0; i < grid_count; i++) {
      //   for (int j=0; j < grid_count; j++) {
      //     if(grid[i][j] == 1) cout << i << " " << j << " / ";
      //   }
      // }
      // cout << endl;
    }
};

int main() {
  InitWindow(grid_count*grid_size, grid_count*grid_size, "Snake"); //initialize the window's size by grid_size*grid_count
  SetTargetFPS(60);

  Game game;

  while(!WindowShouldClose()) {
    BeginDrawing();
    game.update();
    game.draw();
    ClearBackground(background);

    if(gameover) break;
    EndDrawing();
  }
  cout << "gamejoever\n";

  CloseWindow();
  return 0;
}
