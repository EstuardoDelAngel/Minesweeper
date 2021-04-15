#include <ctime>
#include <cstdlib>
#include "utils.hpp"
#include "game.hpp"

unsigned char count_neighbours(int x, int y)
{
    int count = 0;
    for (int i = MAX(x - 1, 0); i <= MIN(x + 1, WIDTH - 1); i++) {
        for (int j = MAX(y - 1, 0); j <= MIN(y + 1, HEIGHT - 1); j++) {
            if (grid[i][j] & MINE_MASK)
                count++;
        }
    }
    return count;
}


void show_all_mines()
{
    for (auto &col : grid) {
        for (auto &squ : col) {
            if (!(squ & HIDDEN_MASK))
                continue;

            if ((squ & STATE_MASK) == FLAG_SQUARE) {
                if (!(squ & MINE_MASK))
                    squ = MINE_MASK | MINE_X_SQUARE;
            }
            else if (squ & MINE_MASK) {
                squ = MINE_MASK | MINE_SQUARE;
            }
        }
    }
}


void set_all_flags()
{
    for (auto &col : grid) {
        for (auto &squ : col) {
            if (squ & HIDDEN_MASK)
                squ = (squ & ~STATE_MASK) | FLAG_SQUARE;
        }
    }
}


int show_square(int x, int y)
{
    unsigned char *squ = &grid[x][y];

    if (*squ != (HIDDEN_MASK | BLANK_SQUARE) && *squ != (HIDDEN_MASK | MARK_SQUARE))
        return -1;
    
    *squ = count_neighbours(x, y);
    num_hidden--;
    return *squ;
}

void expand(int x, int y)
{
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT && show_square(i, j) == 0)
                expand(i, j);
        }
    }
}

bool pos_to_grid(int x, int y, SDL_Point *squ)
{
    int x1 = (x - 12) / 16;
    int y1 = (y - 55) / 16;

    if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT)
        return false;

    squ->x = x1;
    squ->y = y1;
    return true;
}


bool on_face(int x, int y)
{
    if (x >= FACE_OFFSET_L && x < FACE_OFFSET_L + 26 && y >= 15 && y < 41)
        return true;

    return false;
}


static inline unsigned char cycle_marker(unsigned char x)
{
    if (x == FLAG_SQUARE) {
        remaining_mines++;
        return MARK_SQUARE;
    }
    if (x == MARK_SQUARE) {
        return BLANK_SQUARE;
    }
    if (x == BLANK_SQUARE) {
        remaining_mines--;
        return FLAG_SQUARE;
    }
    return x;
}


void right_click_down(int x, int y)
{
    SDL_Point squ;

    if (pos_to_grid(x, y, &squ)) {
        unsigned char *ptr = &grid[squ.x][squ.y];

        if(*ptr & HIDDEN_MASK)
            *ptr = (*ptr & ~STATE_MASK) | cycle_marker(*ptr & STATE_MASK);
    }
}


void left_click_down()
{
    mouse_held = true;
    face = SCARED;
}


void left_click_hold(int x, int y)
{
    SDL_Point squ;
    face = SCARED;

    if (pos_to_grid(x, y, &squ) && (grid[squ.x][squ.y] & HIDDEN_MASK)
    && (grid[squ.x][squ.y] & STATE_MASK) != FLAG_SQUARE) {
        clicked = squ;
    }
    else {
        clicked.x = -1;
        clicked.y = -1;
        if (on_face(x, y))
            face = HAPPY_PRESSED;
    }
}


void left_click_up(int x, int y)
{
    clicked.x = -1;
    clicked.y = -1;
    mouse_held = false;
    face = HAPPY;
    
    SDL_Point squ;
    if (on_face(x, y)) {
        reset();
    }
    else if (pos_to_grid(x, y, &squ)) {
        if (show_square(squ.x, squ.y) != -1)
            expand(squ.x, squ.y);
            
        unsigned char *ptr = &grid[squ.x][squ.y];

        if (!timer_active) {
            timer_active = true;
            if (*ptr & MINE_MASK) {
                *ptr &= ~MINE_MASK;
                
                unsigned char *p;
                srand(time(NULL));
                
                do {
                    p = &grid[rand() % WIDTH][rand() % HEIGHT];
                } while ((*p & MINE_MASK) || (ptr == p));
                *p |= MINE_MASK;

                left_click_up(x, y);
            }
        }
            
        if (*ptr & MINE_MASK) {
            *ptr = MINE_RED_SQUARE;
            won = false;
            finish_game();
        } else if (num_hidden <= MINES) {
            won = true;
            finish_game();
        }
    }
}


void left_click_hold_end(int x, int y)
{
    if (on_face(x, y))
        face = HAPPY_PRESSED;
    else
        face = won ? COOL : DEAD;
}

void left_click_up_end(int x, int y)
{
    mouse_held = false;
    if (on_face(x, y))
        reset();
    else
        face = won ? COOL : DEAD;
}