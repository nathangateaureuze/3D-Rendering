#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <SDL2/SDL.h>
#undef main

#undef M_PI
#include <math.h>

#define WINDOW_WIDTH 400

struct Shape
{
  int (*points)[3];
  int pointslength;

  int (*relations)[2];
  int relationslength;
};

struct Shape shapeconstruct(int *p, int pl, int *r, int rl)
{
    struct Shape *s = malloc(sizeof(struct Shape));

    (*s).pointslength = pl;
    (*s).relationslength = rl;

    (*s).points = malloc(sizeof(int[3]) * (*s).pointslength);
    (*s).relations = malloc(sizeof(int[2]) * (*s).relationslength);

    for(int i = 0; i < (*s).pointslength; i++)
    {   
        (*s).points[i][0] = *(p + 3 * i + 0);
        (*s).points[i][1] = *(p + 3 * i + 1);
        (*s).points[i][2] = *(p + 3 * i + 2);
    }
    for(int i = 0; i < (*s).relationslength; i++)
    {
        (*s).relations[i][0] = *(r + 2 * i + 0);
        (*s).relations[i][1] = *(r + 2 * i + 1);
    }
    
    return *s;
}

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

void rotation(struct Shape s, double rotationx, double rotationy, double rotationz)
{
    for (int i = 0; i < s.pointslength; ++i)
    {
        int x = s.points[i][0];
        int y = s.points[i][1];
        int z = s.points[i][2];
        
        //Rotation axe X
        s.points[i][0] = round(x);
        s.points[i][1] = round(y * cos(rotationx) + z * -sin(rotationx));
        s.points[i][2] = round(y * sin(rotationx) + z * cos(rotationx));
        
        x = s.points[i][0];
        y = s.points[i][1];
        z = s.points[i][2];
        
        //Rotation axe Y
        s.points[i][0] = round(x * cos(rotationy) + z * sin(rotationy));
        s.points[i][1] = round(y);
        s.points[i][2] = round(x * sin(rotationy) + z * cos(rotationy));
        
        x = s.points[i][0];
        y = s.points[i][1];
        z = s.points[i][2];
        
        //Rotation axe Z
        s.points[i][0] = round(x * cos(rotationz) + y * sin(rotationz));
        s.points[i][1] = round(x * -sin(rotationz) + y * cos(rotationz));
        s.points[i][2] = round(z);

        /*s.points[i][0] = round( (x * (cos(rotationy) * cos(rotationz))) + 
                                (y * (cos(rotationy) * sin(rotationz))) + 
                                (z * sin(rotationy)));
        s.points[i][1] = round( (x * (sin(rotationx) * sin(rotationy) * cos(rotationz) + cos(rotationx) * sin(rotationz))) + 
                                (y * (sin(rotationx) * sin(rotationy) * sin(rotationz) + cos(rotationx) * cos(rotationz))) +
                                (z * cos(rotationy)));
        s.points[i][2] = round( (x * (cos(rotationx) * sin(rotationy) * cos(rotationz) + sin(rotationx) * sin(rotationz))) +
                                (y * (cos(rotationx) * sin(rotationy) * sin(rotationz) + sin(rotationz) * cos(rotationz))) +
                                (z * cos(rotationy)));*/
    }
}

void shape(SDL_Renderer *renderer, int points[8][3], int relations[12][2], int relationssize)
{
    for (int i = 0; i < relationssize; ++i)
    {
        line(renderer, points[relations[i][0]][0], points[relations[i][0]][1], points[relations[i][1]][0], points[relations[i][1]][1]);
    }
}

int main(int argc, char *argv[]) 
{
    int i;

    SDL_Event event;
    SDL_Renderer *renderer1;
    SDL_Window *window;

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
    
    int time = 0;

    int mousepositionx;
    int mousepositiony;

    double rx = 0;
    double ry = 0;
    double rz = 0;

    int points1[8][3] = {{-100,-100,100},{100,-100,100},{-100,100,100},{100,100,100},{-100,-100,-100},{100,-100,-100},{-100,100,-100},{100,100,-100}};
    int relations1[12][2] = {{0,1},{0,2},{1,3},{2,3},{0,4},{2,6},{3,7},{1,5},{7,5},{4,5},{6,7},{4,6}};

    struct Shape cube = shapeconstruct((int *)points1, 8, (int *)relations1, 12);

    int points2[5][3] = {{-100,-100,100},{100,-100,100},{-100,-100,-100},{100,-100,-100},{0,100,0}};
    int relations2[8][2] = {{0,1},{0,2},{1,3},{2,3},{0,4},{1,4},{2,4},{3,4}};

    struct Shape pyramide = shapeconstruct((int *)points2, 5, (int *)relations2, 8);
        
    while (1)
    {
        int points1[8][3] = {{-100,-100,100},{100,-100,100},{-100,100,100},{100,100,100},{-100,-100,-100},{100,-100,-100},{-100,100,-100},{100,100,-100}};
        
        cube.points = points1;

        int points2[5][3] = {{-100,-100,100},{100,-100,100},{-100,-100,-100},{100,-100,-100},{0,100,0}};

        pyramide.points = points2;

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
        }
        else if(event.type == SDL_MOUSEMOTION && time == 16) 
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
            rotation(cube, rx, ry, 0);
            shape(renderer1, cube.points, cube.relations, cube.relationslength);
            rotation(pyramide, 0, ry/2, rx/2);
            shape(renderer1, pyramide.points, pyramide.relations, pyramide.relationslength);
        }
        if (time == 16)
        {
            time = 0;
        }
        else
        {
            time += 1;
        }
    }
    free(cube.points);
    free(cube.relations);
    free(&cube);

    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}