#include <ncurses.h>

#define BORDER 1
#define INCREASE_FOOD 2
#define DECREASE_FOOD 3


//Structure to hold properties of Obstacle
struct Obstacle {
  int x;
  int y;
  int width;
  int height;
  char type;
  struct Obstacle* next;
};

typedef struct Obstacle Obstacle;

//Function prototypes 
void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacle);
bool obstacle_exists(Obstacle* obstacles, int x, int y);
Obstacle* create_obstacle(int x, int y, int endX, int endY);
void draw_obstacle(Obstacle *obstacle);
