#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include "utils.hpp"

void config();

void clear();
void present();

void draw_blank(int x, int y);
void draw_grid();
void draw_counter(int num, bool counter);
void draw_face(int face);
void draw_HUD();
void drawing_end();