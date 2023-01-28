#include <stdlib.h>

#include <windows.h>

#include <SDL2/SDL.h>
#undef main

#define WINDOW_WIDTH 600

void line (SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
    int x = x1;
    int y = y1;

    SDL_RenderDrawPoint(renderer, x, y);
	
	if(x <= x2 && y <= y2)
	{
		float error = 2*(y2 - y1) - (x2 - x1);
		float a = 2*(y2 - y1);
		float b = 2*(x2 - x1);
	
		for (int x = x1; x < x2; ++x)
		{
			error = error + a;
			if (error >= 0)
			{
				y = y + 1;
				error = error - b;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	if(x > x2 && y < y2)
	{
		float error = 2*(y2 - y1) - (x1 - x2);
		float a = 2*(y2 - y1);
		float b = 2*(x1 - x2);
	
		for (int x = x1; x > x2; --x)
		{
			error = error + a;
			if (error >= 0)
			{
				y = y + 1;
				error = error - b;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	if(x < x2 && y > y2)
	{
		float error = 2*(y1 - y2) - (x2 - x1);
		float a = 2*(y1 - y2);
		float b = 2*(x2 - x1);
	
		for (int x = x1; x < x2; ++x)
		{
			error = error + a;
			if (error >= 0)
			{
				y = y - 1;
				error = error - b;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	if(x >= x2 && y >= y2)
	{
		float error = 2*(y1 - y2) - (x1 - x2);
		float a = 2*(y1 - y2);
		float b = 2*(x1 - x2);
	
		for (int x = x1; x > x2; --x)
		{
			error = error + a;
			if (error >= 0)
			{
				y = y - 1;
				error = error - b;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
    SDL_RenderPresent(renderer);
}
void shape(SDL_Renderer *renderer, int **points, int **relations)
{
    for (int i = 0; i < sizeof(relations); ++i)
    {
        line(renderer, points[relations[i][0]][0], points[relations[i][0]][1], points[relations[i][1]][0], points[relations[i][1]][1]);
    }
}

int main(void) 
{
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    while (1) 
    {
        int points1[3][3] = {{20,20,20},{360,60,40},{540,410,10}};
        int relations1[3][2] = {{0,1},{1,2},{2,0}};

        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            perror("out");
            break;
        }
        else if(event.type == SDL_KEYDOWN) 
        {
            const char *key = SDL_GetKeyName(event.key.keysym.sym);
            if(strcmp(key, "Q") == 0)
            {

            } 
            else if(strcmp(key, "D") == 0) 
            {

            }
            else if(strcmp(key, "A") == 0) 
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                //line(renderer, 399, 520, 5, 256);
                shape(renderer, points1, relations1);
            }                
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}