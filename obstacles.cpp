#include "obstacles.hpp"

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>

// adds a new obstacle to the linked list of obstacles
void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacle) {
        Obstacle* temp = obstacles;
     while(temp->next) {
         temp = temp->next;
        }
          temp->next = new_obstacle;
}



// determines if there is an obstacle at points (x,y)
bool obstacle_exists(Obstacle* obstacles, int x, int y) {
     Obstacle* temp = obstacles;
    while(temp){
            if (((x >= temp->x) && x < (temp->x + temp->width)) && ((y >= temp->y) && y < (temp->y + temp->height))) {
                    return true;
                }
        temp = temp->next;
    }
    return false;
}


// makes an obstacle at (x,y)
Obstacle* create_obstacle(int x, int y, int endX, int endY) {
    Obstacle* new_Obstacle = (Obstacle*)malloc(sizeof(Obstacle));
    Obstacle* sublist;
    new_Obstacle->x = x;
    new_Obstacle->y = y;
    new_Obstacle->width = endX;
    new_Obstacle->height = endY;
    new_Obstacle->type = 'H';
    new_Obstacle->next = NULL;
    
    return new_Obstacle;
}


// renders tthe obstacle
void draw_obstacle(Obstacle *obstacle)
{   
    Obstacle* temp = obstacle;
    start_color();
    init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
    attron(COLOR_PAIR(6));
    while(temp) {       
        for (int h = temp->y; h < temp->y + temp->height; h++) {
                 for (int w = temp->x; w < temp->x + temp->width; w++) {
                 mvprintw(h, w, "%c", 'H');
                }
            }
        temp = temp->next;
    } 
      attroff(COLOR_PAIR(6));
    
}