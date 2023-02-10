#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <SDL2/SDL.h>
#undef main

#undef M_PI
#include <math.h>

#define WINDOW_WIDTH 400


void line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) 
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int error = dx - dy;
    int error2;

    SDL_RenderDrawPoint(renderer, x1, y1);

    while (x1 != x2 || y1 != y2) 
    {
        SDL_RenderDrawPoint(renderer, (WINDOW_WIDTH/2)+x1, (WINDOW_WIDTH/2)+y1);
        error2 = 2 * error;
        
        if (error2 > -dy) 
        {
            error -= dy;
            x1 += sx;
        }

        if (error2 < dx) 
        {
            error += dx;
            y1 += sy;
        }
    }
    SDL_RenderPresent(renderer);
}

void rotation(int points[8][3], double rotationx, double rotationy, int pointssize)
{
    for (int i = 0; i < pointssize; ++i)
    {
        int x = points[i][0];
        int y = points[i][1];
        int z = points[i][2];
		
		//Rotation axe X
        //points[i][1] = round(y * cos(rotationx) + z * sin(rotationx));
		
		//Rotation axe Y
		points[i][0] = round(x * -sin(rotationy) + z * cos(rotationy));
		
		//Rotation axe Z
        //points[i][0] = round(x * cos(rotation) + y * sin(rotation));
        //points[i][1] = round(x * -sin(rotation) + y * cos(rotation));
    }
}

void shape(SDL_Renderer *renderer, int points[8][3], int relations[12][2], int relationssize)
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
    int time = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer1);
    SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
    SDL_RenderClear(renderer1);

    if(SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
    {
        SDL_GetError();
        SDL_DestroyRenderer(renderer1);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
	

    int mousepositionx;
    int mousepositiony;

    double rx = 0;
    double ry = 0;
    double rz = 0;

    while (1) 
    {
		int points1[8][3] = {{-100,-100,100},{100,-100,100},{-100,100,100},{100,100,100},{-100,-100,-100},{100,-100,-100},{-100,100,-100},{100,100,-100}};
		int relations1[12][2] = {{0,1},{0,2},{1,3},{2,3},{0,4},{2,6},{3,7},{1,5},{7,5},{4,5},{6,7},{4,6}};
		
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
        else if(event.type == SDL_KEYDOWN) 
        {
            const char *key = SDL_GetKeyName(event.key.keysym.sym);
            if(strcmp(key, "Z") == 0) 
            {
                break;
            }
            else if(strcmp(key, "A") == 0) 
            {
                SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
                SDL_RenderClear(renderer1);
                SDL_SetRenderDrawColor(renderer1, 255, 0, 0, 255);

				rx += 0.15;
                rotation(points1, rx, ry, sizeof(points1)/sizeof(points1[0]));
                shape(renderer1, points1, relations1, sizeof(relations1)/sizeof(relations1[0]));
            }             
        }
        else if(event.type == SDL_MOUSEMOTION && time == 1) 
        {
            SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
            SDL_RenderClear(renderer1);
            SDL_SetRenderDrawColor(renderer1, 255, 0, 0, 255);

            SDL_GetRelativeMouseState(&mousepositionx, &mousepositiony);
            if (mousepositionx > 20)
            {
                ry += 0.15;
            }
            else if (mousepositionx < -20)
            {
                ry -= 0.15;
            }
            if (mousepositiony > 20)
            {
                rx += 0.15;
            }
            else if (mousepositiony < -20)
            {
                rx -= 0.15;
            }
            rotation(points1, rx, ry, sizeof(points1)/sizeof(points1[0]));
            shape(renderer1, points1, relations1, sizeof(relations1)/sizeof(relations1[0]));
        }
        if (time == 1)
        {
            time = 0;
        }
        else
        {
            time += 1;
        }
    }
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}