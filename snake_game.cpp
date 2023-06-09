
#include "game.hpp"
#include "key.hpp"
#include "game_window.hpp"
#include <cstdlib>
#include <cstdio>
#include <ncurses.h>

int start_menu();
int x_max, y_max;

int main(void)
{
    int levelchoice = start_menu();
    printf("thuoeou");
    if (levelchoice == '1')
    {
        mvprintw(2, 50, "Level Mode: Easy");
    }
    else if (levelchoice == '2')
    {
        mvprintw(2, 50, "Level Mode: Hard");
    }
    else if (levelchoice == '3')
    {
        mvprintw(2, 50, "Level Mode: Advanced");
    }
    mvprintw(5, 50, "Press 's' or 'S' to start the game");
    int input = getch();
    if (input == 's' || input == 'S')
    {
        game(levelchoice);
    }
    printf("tuenao");
    return (0);
}

//
int start_menu()
{
    initscr();
    noecho();
    getmaxyx(stdscr, y_max, x_max);
    mvprintw(2, 50, "SNAKE GAME");
    mvprintw(3, 50, "Rules:");
    mvprintw(4, 50, "Use the arrow keys to move the snake");
    mvprintw(5, 50, "Eating food, done by crossing over the 'O', 'X', '+', & '-' symbols, affects your score and size");
    mvprintw(6, 50, "The symbols 'O' and '+' make you grow, while 'X' and '-' make you shrink.");
    mvprintw(7, 50, "There are 4 ways to lose a life: hitting a 'H' (representing a hole),");
    mvprintw(8, 50, "hitting the wall, colliding with yourself, or if the length is < 1.");
    mvprintw(9, 50, "Remember, you only have 3 lives so play well!");
    mvprintw(11, 50, "Press 'q' or 'Q' to quit the game");
    mvprintw(12, 50, "Press 'p' or 'P' to pause the game");
    mvprintw(13, 50, "There are 3 level modes: easy, hard, and advances");
    mvprintw(14, 50, "Press [1] to begin easy mode, [2] for hard mode, and [3] for advanced mode");
    int inputLevel = getch();
    refresh();
    endwin();
    clear();
    return inputLevel;
}