
void game(int level);
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset);
void generate_obstacles(int *obs_x, int *obs_y, int width, int height, int x_offset, int y_offset, int obstacle_x_size, int obstacle_y_size, int level);
void update_and_print_leaderboard(int new_score);
void draw_end_screen(int score, int length);
enum State
{
    INIT,
    ALIVE,
    DEAD,
    EXIT
};