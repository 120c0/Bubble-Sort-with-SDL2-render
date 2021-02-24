#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//  480   | 1
//------------
//  X     | 1340

#define MAX_NUMBERS 128
#define SIZE_BAR 10
#define SCALE_BAR 5.f

#define WINDOW_WIDTH 1340
#define WINDOW_HEIGHT 700

#define SORT_TIME_TICK_MS 1
#define CENTER_BAR WINDOW_WIDTH / MAX_NUMBERS / SIZE_BAR * WINDOW_WIDTH / MAX_NUMBERS * 3

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool window_running = false;

int *numbers = NULL;
SDL_Rect *sort_draw = NULL;

int last_time = 0;

void init_sort_draw()
{
    numbers = (int *)malloc(sizeof(int) * MAX_NUMBERS);
    sort_draw = (SDL_Rect *)malloc(sizeof(SDL_Rect) * MAX_NUMBERS);

    for (int i = 0; i < MAX_NUMBERS;)
    {
        int number = rand() % MAX_NUMBERS + 1;
        for (int j = 0; j < MAX_NUMBERS; j++)
        {
            if (number == numbers[i])
            {
                number = 0;
                break;
            }
        }
        if (number)
        {
            numbers[i] = number;
            sort_draw[i] = (SDL_Rect){SIZE_BAR * i + CENTER_BAR, WINDOW_HEIGHT - numbers[i] * SCALE_BAR, SIZE_BAR, numbers[i] * SCALE_BAR};
            i++;
        }
    }
}
void draw_bar()
{
    for (int i = 0; i < MAX_NUMBERS; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &sort_draw[i]);
    }
}


void clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void sort()
{
    int current_time = SDL_GetTicks();
    if (current_time >= last_time + SORT_TIME_TICK_MS)
    {
        last_time = current_time;
        for (int i = 0; i < MAX_NUMBERS; i++)
        {
            int temp = numbers[i];
            if (numbers[i] > numbers[i + 1])
            {
                numbers[i] = numbers[i + 1];
                numbers[i + 1] = temp;
                break;
            }
        }
        /*for(int i = MAX_NUMBERS; i > 0; i--)
        {
            int temp = numbers[i];
            if(numbers[i] < numbers[i - 1])
            {
                numbers[i] = numbers[i - 1];
                numbers[i - 1] = temp;
                break;
            }
        }

        for (int i = MAX_NUMBERS / 2 + 1; i < MAX_NUMBERS; i++)
        {
            int temp = numbers[i];
            if (numbers[i] > numbers[i + 1])
            {
                numbers[i] = numbers[i + 1];
                numbers[i + 1] = temp;
                break;
            }
        }
        for(int i = MAX_NUMBERS / 2 - 1; i > 0; i--)
        {
            int temp = numbers[i];
            if(numbers[i] < numbers[i - 1])
            {
                numbers[i] = numbers[i - 1];
                numbers[i - 1] = temp;
                break;
            }
        } */
    }
    for (int i = 0; i < MAX_NUMBERS; i++)
        sort_draw[i] = (SDL_Rect){SIZE_BAR * i + CENTER_BAR, WINDOW_HEIGHT - numbers[i] * SCALE_BAR, SIZE_BAR, numbers[i] * SCALE_BAR};
}


void init_display()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    window = SDL_CreateWindow(
        "Sort Algorithm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_PRESENTVSYNC
    );
    window_running = true;
}
void end()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
void update_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            window_running = false;
        if (event.key.keysym.sym == SDLK_r)
            init_sort_draw();
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    init_display();
    init_sort_draw();

    while (window_running)
    {
        update_events();
        clear();
                
        sort();
        draw_bar();

        SDL_RenderPresent(renderer);
    }

    end();
    return 0;
}
