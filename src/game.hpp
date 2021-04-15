#pragma once

#include <ctime>
#include <cstdlib>
#include "utils.hpp"

unsigned char count_neighbours(int x, int y);
void show_all_mines();
void set_all_flags();
int show_square(int x, int y);
void expand(int x, int y);

bool pos_to_grid(int x, int y, SDL_Point *squ);
bool on_face(int x, int y);

void right_click_down(int x, int y);
void left_click_down();
void left_click_hold(int x, int y);
void left_click_up(int x, int y);
void left_click_hold_end(int x, int y);
void left_click_up_end(int x, int y);