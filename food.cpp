/*food.cpp -------
 *
 * Filename: food.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer: Anushka Kulkarni
 * Created: Sat Sep 12 13:16:12 2022
 * Last-Updated: September 12 16:51 2022
 *
 */

/* Change log:
 * added implementations for food_type and remove_eaten_food
 * added two new food symbols
 */

/*Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp"

// Create new food
Food *create_food(int x, int y, enum Type type)
{
    Food *new_food = (Food *)malloc(sizeof(Food));

    new_food->x = x;
    new_food->y = y;
    if (type == Increase)
    {

        if (rand() % 2 == 0)
        {
            new_food->type = 'O';
        }
        else
        {
            new_food->type = '+';
        }
    }
    else if (type == Decrease)
    {

        if (rand() % 2 == 0)
        {
            new_food->type = 'X';
        }
        else
        {
            new_food->type = '-';
        }
    }
    new_food->next = NULL;

    return new_food;
}

// Check if food exists at coordinates
bool food_exists(Food *foods, int x, int y)
{
    Food *temp = foods;
    while (temp)
    {
        if (temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

// Add new food to end of food list
void add_new_food(Food *foods, Food *new_food)
{
    Food *temp = foods;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = new_food;
}

enum Type food_type(Food *foods, int x, int y)
{
    Food *temp = foods;

    while (temp)
    {
        if (temp->x == x && temp->y == y)
        {
            if (temp->type == 'O' || temp->type == '+')
            {
                return Increase;
                break;
            }
            else if (temp->type == 'X' || temp->type == '-')
            {
                return Decrease;
                break;
            }
        }
        temp = temp->next;
    }
}

Food *remove_eaten_food(Food *foods, int x, int y)
{

    if (foods->x == x && foods->y == y)
    {
        Food *temp = foods;
        foods = foods->next;
        free(temp);
        return foods;
    }

    Food *new_food = foods;
    while (new_food->next)
    {
        if (new_food->next->x == x && new_food->next->y == y)
        {
            Food *temp = new_food->next;
            new_food->next = new_food->next->next;
            free(temp);
            break;
        }
        new_food = new_food->next;
    }

    return foods;
}
// Display all the food
void draw_food(Food *foods)
{
    Food *temp = foods;
    start_color();
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    while (temp)
    {
        if (food_type(temp, temp->x, temp->y) == Decrease)
        {
            attron(COLOR_PAIR(3));
            mvprintw(temp->y, temp->x, "%c", temp->type);
            attroff(COLOR_PAIR(3));
        }
        else
        {
            attron(COLOR_PAIR(2));

            mvprintw(temp->y, temp->x, "%c", temp->type);
            attroff(COLOR_PAIR(2));
        }

        temp = temp->next;
    }
}