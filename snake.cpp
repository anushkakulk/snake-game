

#include <cstring>
#include <cstdbool>
#include <cstdlib>
#include "snake.hpp"
#include "key.hpp"
#include <ncurses.h>
#define BORDER 1
#define INCREASE_FOOD 2
#define DECREASE_FOOD 3
#define TAIL 4
#define HEAD 5

// Initialize snake
Snake *init_snake(int x, int y)
{
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x - 1, y);
  Snake *tail2 = create_tail(x - 2, y);
  head->speed = 1;
  tail1->next = tail2;
  head->next = tail1;
  return head;
}

// Creates one tail
Snake *create_tail(int x, int y)
{
  Snake *snake = new Snake;
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 255;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

// Moves the snake in the input direction
Snake *move_snake(Snake *snake, int direction)
{
  // TODO
  Snake *new_head = new Snake;

  // Set the new head to have the x and y coordinates as the existing head of the snake
  new_head->x = snake->x;
  new_head->y = snake->y;
  switch (direction)
  {
  case UP:
    new_head->y--;
    break;
  case DOWN:
    new_head->y++;
    break;
  case RIGHT:
    new_head->x++;
    break;
  case LEFT:
    new_head->x--;
    break;
  }

  // Set new head as the new head of the entire snake
  new_head->next = snake;
  // Add all the features (color and symbol) to the new cell
  new_head->color[0] = snake->color[0];
  new_head->color[1] = snake->color[1];
  new_head->color[2] = snake->color[2];
  new_head->symbol = snake->symbol;

  // Delete the tail from the snake
  snake = remove_tail(snake);

  return new_head;
}

Snake *remove_tail(Snake *snake)
{
  Snake *end = snake;
  while (end->next->next)
    end = end->next;
  free(end->next);
  end->next = NULL;
  return snake;
}
Snake *get_tail(Snake *snake)
{
  Snake *end = snake;
  while (end->next->next)
    end = end->next;
  return end;
}
Snake *grow_tail(Snake *snake, KEY key)
{
  Snake *temp = snake;
  Snake *oldTail;
  switch (get_length(snake))
  {
  case 1:
    oldTail = temp;
    break;
  case 2:
    oldTail = temp->next;
    break;
  default:
    oldTail = get_tail(temp);
    break;
  }
  int x = oldTail->x;
  int y = oldTail->y;
  switch (key)
  {
  case LEFT:
    x++;
    break;
  case RIGHT:
    x--;
    break;
  case UP:
    y++;
    break;
  case DOWN:
    y--;
    break;
  }
  Snake *newTail = create_tail(x, y);

  switch (key)
  {
  case LEFT:
    x++;
    break;
  case RIGHT:
    x--;
    break;
  case UP:
    y++;
    break;
  case DOWN:
    y--;
    break;
  }
  Snake *finalNewTail = create_tail(x, y);
  oldTail->next = newTail;
  newTail->next = finalNewTail;
  newTail->color[0] = oldTail->color[0];
  newTail->color[1] = oldTail->color[1];
  newTail->color[2] = oldTail->color[2];
  newTail->speed = oldTail->speed;
  newTail->symbol = oldTail->symbol;
  finalNewTail->color[0] = oldTail->color[0];
  finalNewTail->color[1] = oldTail->color[1];
  finalNewTail->color[2] = oldTail->color[2];
  finalNewTail->speed = oldTail->speed;
  finalNewTail->symbol = oldTail->symbol;
  return snake;
}

// draws the snake on the board
void draw_snake(Snake *snake)
{
  start_color();
  int curr_pos = 0;
  init_pair(TAIL, COLOR_BLACK, COLOR_WHITE);
  init_pair(HEAD, COLOR_BLACK, COLOR_YELLOW);
  while (snake->next)
  {
    switch (curr_pos)
    {
    case 0:
      attron(COLOR_PAIR(HEAD));
      break;
    default:
      attron(COLOR_PAIR(TAIL));
      break;
    }
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
    curr_pos += 1;
  }
  attron(COLOR_PAIR(TAIL));
  mvprintw(snake->y, snake->x, "%c", snake->symbol);
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake *snake)
{
  Snake *head = snake;
  Snake *temp = snake->next;
  while (temp)
  {
    if (temp->x == head->x && temp->y == head->y)
    {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

int get_length(Snake *snake)
{
  int l = 1;
  while (snake->next)
  {
    l++;
    snake = snake->next;
  }
  return l;
}