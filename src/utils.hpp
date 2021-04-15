#pragma once

#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include "drawing.hpp"
#include "game.hpp"

#define WIDTH 16
#define HEIGHT 16
#define MINES 40

#define GRID_W (16 * WIDTH)
#define GRID_H (16 * HEIGHT)

#define WHITE 0xFF, 0xFF, 0xFF
#define LIGHT 0xCB, 0xCB, 0xCB
#define DARK 0x92, 0x92, 0x92

#define WIN_W (20 + GRID_W)
#define WIN_H (63 + GRID_H)

#define COUNTER_L 0
#define COUNTER_R 1

#define PLAY 0
#define END 1

#define STATE_MASK 0b001111
#define HIDDEN_MASK 0b010000
#define MINE_MASK 0b100000

#define FACE_OFFSET_L (GRID_W / 2 + 1)

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

enum {
    BLANK_SQUARE = 0,
    MINE_SQUARE = 9,
    MINE_RED_SQUARE,
    MINE_X_SQUARE,
    FLAG_SQUARE,
    MARK_SQUARE
};

enum {
    COOL,
    DEAD,
    SCARED,
    HAPPY,
    HAPPY_PRESSED
};

extern unsigned char grid[WIDTH][HEIGHT];
extern int face;
extern float timer;
extern int remaining_mines;
extern SDL_Point clicked;
extern bool mouse_held;
extern int num_hidden;
extern bool timer_active;
extern bool won;
extern void (*update)(bool *);

void finish_game();
void reset();
void end();
void update_play(bool *running);
void update_end(bool *running);