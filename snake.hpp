/* snake.hpp -------
 *
 * Filename: snake.hpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer: Anushka Kulkarni
 * Created: Sun Sep 13 21:01:02 2022
 * Last-Updated: September 13 22:43 2022
 *
 */


/* Change log:
 * - added methods get_tail, get_length, and grow_tail
 *
 */

/* Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */


#include <cstdbool>
#include "key.hpp"
#define TAIL  4
#define HEAD   5


struct Snake {
  int x;
  int y;
  int speed;
  char color[3];
  char symbol;
  struct Snake* next;
};

typedef struct Snake Snake;

Snake* init_snake(int x, int y);
Snake* create_tail(int x, int y);
Snake* move_snake(Snake* snake, int direction);
void draw_snake(Snake* snake);
bool eat_itself(Snake* snake);
Snake* remove_tail(Snake* snake);
Snake *grow_tail(Snake *snake, enum KEY);
int get_length(Snake* snake);