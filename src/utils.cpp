#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include "drawing.hpp"
#include "game.hpp"
#include "utils.hpp"

unsigned char grid[WIDTH][HEIGHT] = {};
int face = HAPPY;
float timer = 0;
int remaining_mines = MINES;
SDL_Point clicked = {-1, -1}; //{-1, -1} means none
bool mouse_held = false;
bool timer_active = false;
bool won = false;
int num_hidden = WIDTH * HEIGHT;
void (*update)(bool *) = &update_play;

void finish_game()
{
    update = &update_end;
    if (won) {
        set_all_flags();
        face = COOL;
    }
    else {
        show_all_mines();
        face = DEAD;
    }
}


void reset()
{
    update = &update_play;
    for (auto &col : grid) {
        for (auto &square : col)
            square = HIDDEN_MASK;
    }

    srand(time(NULL));

    for (int i = 0; i < MINES; i++) {
        unsigned char *p;
        do {
            p = &grid[rand() % WIDTH][rand() % HEIGHT];
        } while(*p & MINE_MASK);
        *p |= MINE_MASK;
    }

    face = HAPPY;
    timer_active = false;
    timer = 0;
    remaining_mines = MINES;
    num_hidden = WIDTH * HEIGHT;
}

void end()
{
    drawing_end();
    SDL_Quit();
}


void update_play(bool *running)
{
    SDL_Event event;
    SDL_Point mouse;
    int start_time = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
            break;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT)
                left_click_down();

            else if (event.button.button == SDL_BUTTON_RIGHT)
                right_click_down(event.button.x, event.button.y);
        } 
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            left_click_up(event.button.x, event.button.y);
        }

        SDL_GetMouseState(&mouse.x, &mouse.y);

        if (mouse_held && mouse.x != -1 && mouse.y != -1) {
            left_click_hold(mouse.x, mouse.y);
        }
    }

    clear();

    draw_HUD();
    draw_grid();
    draw_face(face);
    draw_counter(remaining_mines < 999 ? (remaining_mines > 0 ? remaining_mines : 0) : 999 , COUNTER_L);
    draw_counter(timer < 999 ? timer : 999, COUNTER_R);
    
    if (clicked.x != -1 && clicked.y != -1)
        draw_blank(clicked.x, clicked.y);

    present();

    int dt = SDL_GetTicks() - start_time;
    if (dt < 16)
        SDL_Delay(16 - dt);
    
    if (timer_active)
        timer += 0.016;
}


void update_end(bool *running)
{
    SDL_Event event;
    SDL_Point mouse;
    int start_time = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
            break;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                mouse_held = true;
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            left_click_up_end(event.button.x, event.button.y);

        SDL_GetMouseState(&mouse.x, &mouse.y);

        if (mouse_held && mouse.x != -1 && mouse.y != -1) {
            left_click_hold_end(mouse.x, mouse.y);
        }
    }

    clear();

    draw_HUD();
    draw_grid();
    draw_face(face);
    draw_counter(0, COUNTER_L);
    draw_counter(timer, COUNTER_R);
    present();

    int dt = SDL_GetTicks() - start_time;
    if (dt < 16)
        SDL_Delay(16 - dt);
}