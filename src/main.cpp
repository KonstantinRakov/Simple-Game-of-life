#include <SDL2/SDL.h>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>

#include "screen.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int zeroOrOne() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    return dis(gen);
}

template <int row, int cow>
bool isAlive(int (&game)[row][cow], int x, int y) {
    int alive = 0;

    // left test
    if (x > 0 && game[x - 1][y] == 1) alive++;
    // right test
    if (x < SCREEN_WIDTH && game[x + 1][y] == 1) alive++;
    // top test
    if (y > 0 && game[x][y - 1] == 1) alive++;
    // bottom test
    if (y < SCREEN_HEIGHT && game[x][y + 1] == 1) alive++;
    // top left test
    if (x > 0 && y > 0 && game[x - 1][y - 1] == 1) alive++;
    // top right test
    if (x < SCREEN_WIDTH && y > 0 && game[x + 1][y - 1] == 1) alive++;
    // bottom left test
    if (x > 0 && y < SCREEN_HEIGHT && game[x - 1][y + 1] == 1) alive++;
    // bottom right test
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && game[x + 1][y + 1] == 1) alive++;
    // if the cell is alive and has 2 or 3 neighbors, it stays alive or
    // if the cell is dead and has exactly 3 neighbors, it comes to life
    if ((game[x][y] == 1 && (alive == 2 || alive == 3)) || (game[x][y] == 0 && alive == 3)) return true;
        // otherwise, the cell dies
    else return false;
}


int main(int argc, char** argv)
{
    Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT);
    try {
        screen.screenInit();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    srand(time(nullptr));
    int tilemap_display[64][48] = {};
    int tilemap_swap[64][48] = {};

    //function for random placement
    /*
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 48; y++)  {
            
            tilemap_display[x][y] = zeroOrOne();
        }        
    }
    */     

    bool gamePause = false;
    bool running = true;
    while(running){
        SDL_Event event;
        // display to screen
        screen.show(tilemap_display);  

        // display to screen
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    int x, y;
                    SDL_GetMouseState(&x, &y); 
                    x /= 10;
                    y /= 10;
                    (tilemap_display[x][y] == 0) ? (tilemap_display[x][y] = 1) : (tilemap_display[x][y] = 0);
                    break;
                case SDL_KEYDOWN: 
                    switch(event.key.keysym.sym){ 
                    case SDLK_SPACE:
                        gamePause == false ? gamePause = true : gamePause = false;
                    }
                    break;

                default:
                    break;
            }
        }
        //Logic of the game
        if (gamePause) {

            for (int x = 0; x < 64; ++x) {
                for (int y = 0; y < 48; ++y) {
                    // check if the cell will be alive or dead in the next generation
                    tilemap_swap[x][y] = isAlive(tilemap_display, x, y) ? 1 : 0;
                }
            }
            // swap buffers
            std::swap(tilemap_swap, tilemap_display);
        }
    }
    return 0;
}