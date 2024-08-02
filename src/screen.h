#pragma once 

#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <time.h>  
#include <vector>
#include <tuple>
#include <algorithm>
#include <exception>

class Screen {

private:

    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;
    SDL_Surface *tileMapSurface;
    SDL_Texture *tileTexture;

    SDL_Rect select_tile_1 = {0, 0, 9, 9};
    SDL_Rect select_tile_2 = {10, 0, 9, 9};
    SDL_Rect tile[64][48];


    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 480;

    const char * NAME_WINDOW = "SDL test";    
    const char * FILE_PATH = "res/rect.bmp";
    const std::string ERROR_SDL_Init = ("Error: SDL failed to initialize\nSDL Error: '%s'\n");
    const std::string ERROR_SDL_CreateWindow = ("Error: Failed to open window\nSDL Error: '%s'\n");
    const std::string ERROR_SDL_CreateRenderer = ("Error: Failed to create renderer\nSDL Error: '%s'\n");
    const std::string ERROR_SDL_LoadBMP = ("Error: Failed to load image\nSDL_Image Error: '%s'\n");
    const std::string ERROR_SDL_CreateTexture = ("Error: Failed to create texture\n");

public:

    Screen(int inSCREEN_WIDTH, int inSCREEN_HEIGHT) : SCREEN_WIDTH(inSCREEN_WIDTH), SCREEN_HEIGHT(inSCREEN_HEIGHT){};

    void screenInit() 
    {        
        if(SDL_Init(SDL_INIT_VIDEO) < 0) { throw std::invalid_argument(ERROR_SDL_Init); }

        window = SDL_CreateWindow(NAME_WINDOW, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!window) { throw std::invalid_argument(ERROR_SDL_CreateWindow); }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) { throw std::invalid_argument(ERROR_SDL_CreateRenderer); }

        tileMapSurface = SDL_LoadBMP(FILE_PATH);
        if(!tileMapSurface) { throw std::invalid_argument(ERROR_SDL_LoadBMP); }
        
        tileTexture = SDL_CreateTextureFromSurface(renderer, tileMapSurface);
        if(!tileTexture) { throw std::invalid_argument(ERROR_SDL_CreateTexture); }
        
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 48; y++) {
                tile[x][y].x = x * 10;
                tile[x][y].y = y * 10;
                tile[x][y].w = 10;
                tile[x][y].h = 10;
            }        
        }
    }
    ~Screen(){
        SDL_DestroyTexture(tileTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    template <int row, int cow>
    void show(int (&tilemap_display)[row][cow]) {
        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Delay(40);

        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 48; y++) {
                switch (tilemap_display[x][y]) {
                    case 1:
                        SDL_RenderCopy(renderer, tileTexture, &select_tile_1, &tile[x][y]);
                        break;
                    case 0:
                        SDL_RenderCopy(renderer, tileTexture, &select_tile_2, &tile[x][y]);
                        break;
                }
            }        
        }
        SDL_RenderPresent(renderer);       
    }

};