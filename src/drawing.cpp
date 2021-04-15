#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include "utils.hpp"
#include "drawing.hpp"

static SDL_Window *window = SDL_CreateWindow(
    "Minesweeper",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIN_W,
    WIN_H,
    SDL_WINDOW_SHOWN
);
static SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
static SDL_Texture *assets = IMG_LoadTexture(renderer, "res/assets.png");


void config()
{
    if (window == NULL || renderer == NULL || assets == NULL || SDL_Init(SDL_INIT_EVERYTHING) > 0
    || !IMG_Init(IMG_INIT_PNG)) {
        std::cout << SDL_GetError() << std::endl;
        exit(-1);
    }
}


static void render(SDL_Rect *src, int x, int y)
{
    SDL_Rect dst = {x, y, src->w, src->h};
    SDL_RenderCopy(renderer, assets, src, &dst);
}


static void draw_line(int x1, int y1, int x2, int y2)
{ 
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}


static void draw_rect(const SDL_Rect *rect)
{
    SDL_RenderFillRect(renderer, rect);
}


static void set_colour(unsigned char R, unsigned char G, unsigned char B) 
{
    SDL_SetRenderDrawColor(renderer, R, G, B, 0xFF);
}

void clear()
{
    SDL_RenderClear(renderer);
}

void present()
{
    SDL_RenderPresent(renderer);
}

void draw_blank(int x, int y)
{
    SDL_Rect src = {0, 49, 16, 16};
    render(&src, 12 + 16 * x, 55 + 16 * y);
}

void draw_grid()
{
    SDL_Rect src;
    src.w = 16;
    src.h = 16;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            unsigned char i = grid[x][y];

            if (i & HIDDEN_MASK) {
                src.x = 16 * (i & STATE_MASK);
                src.y = 65;
            } else {
                src.x = 16 * (i & STATE_MASK);
                src.y = 49;
            }


            render(&src, 12 + 16 * x, 55 + 16 * y);
        }
    }
}


void draw_counter(int num, bool counter)
{
    int digits[] = {
        num / 100 % 10, 
        num / 10 % 10, 
        num % 10
    };

    for (int i = 0; i < 3; i++) {
        SDL_Rect src = {13 * digits[i], 0, 13, 23};
        render(&src, (counter ? WIN_W - 54 : 17) + 13 * i, 16);
    }
}


void draw_face(int face)
{
    SDL_Rect src = {26 * face, 23, 26, 26};
    render(&src, FACE_OFFSET_L, 15);
}


static void draw_HUD_rect(SDL_Rect *rect, int weight)
{
    int x1 = rect->x;
    int y1 = rect->y;
    int x2 = x1 + rect->w;
    int y2 = y1 + rect->h;

    //top and left
    set_colour(DARK);
    for (int i = 0; i < weight; i++) {
        draw_line(x1,  y1 + i,  x2 - i - 1,  y1 + i);
        draw_line(x1 + i,  y1,  x1 + i,  y2 - i - 1);
    }

    //bottom and right
    set_colour(WHITE);
    for (int i = 0; i < weight; i++) {
        draw_line(x2 - i,  y1 - i - 1,  x2 - i,  y2);
        draw_line(x1 + i + 1,  y2 - i,  x2,  y2 - i);
    }
}


void draw_HUD()
{
    SDL_Rect r;

    //background
    set_colour(LIGHT);
    r.x = 0;
    r.y = 0;
    r.w = WIN_W;
    r.h = WIN_H;
    draw_rect(&r);

    //white border
    set_colour(WHITE);
    r.h = 3;
    draw_rect(&r);
    r.w = 3;
    r.h = WIN_H;
    draw_rect(&r);

    //rect w/ face and counters
    r.x = 9;
    r.y = 9;
    r.w = GRID_W + 6;
    r.h = 37;
    draw_HUD_rect(&r, 2);

    //left counter
    r.x = 16;
    r.y = 15;
    r.w = 41;
    r.h = 25;
    draw_HUD_rect(&r, 1);

    //right counter
    r.x = WIN_W - 55;
    draw_HUD_rect(&r, 1);

    //grid
    r.x = 9;
    r.y = 52;
    r.w = GRID_W + 6;
    r.h = GRID_H + 6;
    draw_HUD_rect(&r, 3);
}

void drawing_end()
{
    SDL_DestroyWindow(window);
}