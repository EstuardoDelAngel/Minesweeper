#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "drawing.hpp"
#include "game.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    config();
    reset();
    bool running = true;

    while (running) {
        update(&running);
    }

    end();
    return 0;
}