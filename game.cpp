/*game.cpp -------
 *
 * Filename: game.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:Anushka Kulkarni
 * Created: Sat Sep 12 13:16:12 2022
 * Last-Updated: September 12 16:51 2022
 *
 */

/* Change log:
 * 5/24 - Added movement and food eating functionality
 * 5/26 - added score and speed functionality
 * 5/29 - added obstacles
 * 5/30 - completed part 1
 * 5/30 - completed part 2
 */

/*Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "snake.hpp"
#include "food.hpp"
#include "obstacles.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define BORDER 1
#define INCREASE_FOOD 2
#define DECREASE_FOOD 3

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset)
{
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

void generate_obstacles(int *obs_x, int *obs_y, int width, int height,
                        int x_offset, int y_offset, int obstacle_x_size,
                        int obstacle_y_size, int level)
{
    *obs_x = rand() % (width - obstacle_x_size - 2) + x_offset + 1;
    *obs_y = rand() % (height - obstacle_y_size - 2) + y_offset + 1;
}

void update_and_print_leaderboard(int new_score);
void draw_end_screen(int score, int length);
bool game_over_displayed = false;

void game(int level)
{
    enum State state = INIT;            // Set the initial state
    static int x_max, y_max;            // Max screen size variables
    static int x_offset, y_offset;      // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window;               // Name of the board
    Snake *snake;                       // The snake
    Food *foods, *new_food;             // List of foods (Not an array)
    Obstacle *obstacles, *new_obstacle; // List of obstacles

    const int height = 30;
    const int width = 70;
    char ch;
    enum KEY dir = RIGHT; // direction thats acutally being moved int
    int num_food;
    int num_obstacles;
    int obstacle_x_size;
    int obstacle_y_size;
    int lives = 3;
    int score = 0;
    int speed_factor = 1;
    srand((int)time(0));
    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999 / (speed_factor * 4);

    while (state != EXIT)
    {
        switch (state)
        {
        case INIT:
            initscr();
            start_color();
            start_color();
            nodelay(stdscr, TRUE); // Dont wait for char
            noecho();              // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0);          // hide cursor
            timeout(100);

            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);

            // Init board
            window = init_GameWindow(x_offset, y_offset, width, height);

            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            snake->speed = 10 * level;
            // Init foods
            int food_x, food_y, i;
            enum Type type;
            // init obstacles
            int obs_x, obs_y, j;
            obstacle_x_size = (rand() % 7) + 1;
            obstacle_y_size = (rand() % 7) + 1;

            generate_obstacles(&obs_x, &obs_y, width, height, x_offset, y_offset,
                               obstacle_x_size, obstacle_y_size, level);

            obstacles = create_obstacle(obs_x, obs_y, obstacle_x_size, obstacle_y_size);

            // init random number of obstacles
            if (level == '3')
            {
                num_obstacles = (rand() % 7) + (3 * level);
            }
            else if (level == '2')
            {
                num_obstacles = (rand() % 7) + (2 * level);
            }
            else
            {
                num_obstacles = (rand() % 7) + 3;
            }

            for (j = 0; j < num_obstacles; j++)
            {
                obstacle_x_size = (rand() % 2) + 1;
                obstacle_y_size = (rand() % 2) + 1;
                generate_obstacles(&obs_x, &obs_y, width, height, x_offset,
                                   y_offset, obstacle_x_size, obstacle_y_size, level);
                while (obstacle_exists(obstacles, obs_x, obs_y))
                {
                    generate_obstacles(&obs_x, &obs_y, width, height, x_offset,
                                       y_offset, obstacle_x_size, obstacle_y_size, level);
                }
                new_obstacle = create_obstacle(obs_x, obs_y, obstacle_x_size, obstacle_y_size);
                add_new_obstacle(obstacles, new_obstacle);
            }

            // Generate foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX / 2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            if (level == '3')
            {
                num_food = (rand() % 7) + (4 * level);
            }
            else if (level == '2')
            {
                num_food = (rand() % 7) + (3 * level);
            }
            else
            {
                num_food = (rand() % 7) + 5;
            }

            for (i = 1; i < num_food; i++)
            {
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods, food_x, food_y))
                {
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                }
                type = (rand() > RAND_MAX / 2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
            state = ALIVE;
            break;

        case ALIVE:

            // MOVE DIRECTION
            ch = get_char();
            switch (ch)
            {
            case 'q':
            case 'Q':
                state = EXIT;
                break;

            case 'p':
            case 'P':
                timeout(-1);
                ch = get_char();
                while (ch != 'p' && ch != 'P')
                {
                    ;
                }
                timeout(100);
                break;
            case UP:
                if (dir != DOWN)
                {
                    dir = UP;
                }
                break;
            case DOWN:
                if (dir != UP)
                {
                    dir = DOWN;
                }
                break;

            case LEFT:
                if (dir != RIGHT)
                {
                    dir = LEFT;
                }
                break;
            case RIGHT:
                if (dir != LEFT)
                {
                    dir = RIGHT;
                }
                break;
            }

            snake->speed = snake->speed + (score / 100);
            if (score % 100)
            {
                if (level == '3')
                {
                    speed_factor = 1.5 * snake->speed * 4;
                }
                else if (level == '2')
                {
                    speed_factor = 1.5 * snake->speed * 3;
                }
                else
                {
                    speed_factor = 1.5 * snake->speed;
                }
            }

            snake = move_snake(snake, dir);

            enum Type new_type;
            enum Type eaten_type;
            if (food_exists(foods, snake->x, snake->y))
            {

                eaten_type = food_type(foods, snake->x, snake->y);
                if (eaten_type == Increase)
                {
                    snake = grow_tail(snake, dir);
                    score += 20;
                }
                else
                {
                    if (get_length(snake) > 1)
                    {
                        snake = remove_tail(snake);
                        score -= 10;
                    }
                    else
                    {
                        score -= 10;
                        lives--;
                        state = DEAD;
                    }
                }

                foods = remove_eaten_food(foods, snake->x, snake->y);
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods, food_x, food_y))
                {
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                }
                new_type = (rand() > RAND_MAX / 2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, new_type);
                add_new_food(foods, new_food);
            }

            if (lives > 0)
            {
                if (obstacle_exists(obstacles, snake->x, snake->y) || eat_itself(snake))
                {
                    
                    lives--;
                }
                if (snake->x < x_offset || snake->x >= x_offset + width || snake->y < y_offset || snake->y >= y_offset + height)
                {
                    lives--;
                }
            }
            else
            {
                state = DEAD;
            }

            // Draw everything on the screen
            clear();
            mvprintw(4, 20, "Key entered: %c", ch);
            mvprintw(5, 20, "Score: %i", score);
            mvprintw(6, 20, "Lives: %i", lives);
            mvprintw(7, 20, "Length: %i", get_length(snake));
            mvprintw(8, 20, "Level: %c", level);

            draw_Gamewindow(window);
            draw_snake(snake);
            draw_obstacle(obstacles);
            draw_food(foods);
            break;

        case DEAD:
            if (!game_over_displayed)
            {
                clear();
                refresh();
                undraw_Gamewindow(window);
                draw_end_screen(score, get_length(snake));
                endwin();
                game_over_displayed = true;
            }
            mvprintw(20, 20, "Press 'q' or 'Q' to exit to terminal");

            int user_exit = getch();
            if (user_exit == 'q' || user_exit == 'Q')
            {
                state = EXIT;
            }
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
    clear();
    refresh();
    endwin();
}

void draw_end_screen(int final_score, int final_length)
{
    int x_max, y_max;
    initscr();
    noecho();
    getmaxyx(stdscr, y_max, x_max);
    mvprintw(2, 20, "Game Over!");
    mvprintw(3, 20, "Final Length: %i", final_length);
    mvprintw(4, 20, "Points: %i", final_score);
    refresh();
    endwin();
    update_and_print_leaderboard(final_score);
}

void update_and_print_leaderboard(int new_score)
{
    const std::string filename = "./saves/save_best_10.game";
    const int leaderboardSize = 10;

    std::ifstream fileIn(filename);
    std::vector<int> scores;

    int score;
    while (fileIn >> score)
    {
        scores.push_back(score);
    }

    scores.push_back(new_score);

    std::sort(scores.begin(), scores.end(), std::greater<int>());

    if (scores.size() > leaderboardSize)
    {
        scores.pop_back();
    }

    std::ofstream fileOut(filename);
    for (int i = 0; i < scores.size(); i++)
    {
        fileOut << scores[i] << "\n";
    }
    fileOut.close();

    mvprintw(6, 20, "Leaderboard:\n");
    for (int i = 0; i < scores.size(); i++)
    {
        mvprintw(8 + i, 20, "%d. %d\n", i + 1, scores[i]);
    }
}