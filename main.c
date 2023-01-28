#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <SDL2/SDL.h>
#undef main

#define WINDOW_WIDTH 600


void line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int error = dx - dy;
    int error2;

    SDL_RenderDrawPoint(renderer, x1, y1);

    while (x1 != x2 || y1 != y2) {
        SDL_RenderDrawPoint(renderer, x1, y1);
        error2 = 2 * error;
        
        if (error2 > -dy) {
            error -= dy;
            x1 += sx;
        }

        if (error2 < dx) {
            error += dx;
            y1 += sy;
        }
    }
    SDL_RenderPresent(renderer);
}

void shape(SDL_Renderer *renderer, int points[3][3], int relations[3][2], int relationssize)
{
    for (int i = 0; i < relationssize; ++i)
    {
        line(renderer, points[relations[i][0]][0], points[relations[i][0]][1], points[relations[i][1]][0], points[relations[i][1]][1]);
    }
}

int main(void) 
{
    SDL_Event event;
    SDL_Renderer *renderer1;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer1);
    SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
    SDL_RenderClear(renderer1);

    while (1) 
    {
        int points1[3][3] = {{20,20,20},{360,60,40},{540,410,10}};
        int relations1[3][2] = {{0,1},{1,2},{0,2}};

        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
        else if(event.type == SDL_KEYDOWN) 
        {
            const char *key = SDL_GetKeyName(event.key.keysym.sym);
            if(strcmp(key, "Q") == 0) 
            {
                SDL_SetRenderDrawColor(renderer1, 200, 200, 200, 255);
            } 
            else if(strcmp(key, "D") == 0) 
            {
                SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
                SDL_RenderClear(renderer1);

                SDL_SetRenderDrawColor(renderer1, 255, 0, 0, 255);
                for (i = 0; i < WINDOW_WIDTH; ++i)
                {
                    SDL_RenderDrawPoint(renderer1, i, i);
                }
                SDL_RenderPresent(renderer1);
            }
            else if(strcmp(key, "A") == 0) 
            {
                SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
                SDL_RenderClear(renderer1);

                SDL_SetRenderDrawColor(renderer1, 255, 0, 0, 255);
                //line(renderer1, 120, 161, 55, 32);
                shape(renderer1, points1, relations1, sizeof(relations1)/sizeof(relations1[0]));
            }             
        }
    }
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}