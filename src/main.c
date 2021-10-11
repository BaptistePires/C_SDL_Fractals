#include <SDL2/SDL.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "fractals.h"
#include "utils.h"
#include "guiMenu.h"

#define FPS 60.0
#define FRAME_TIME 1000.0 / FPS
#define WIDTH 1000
#define HEIGHT 800
#define COUNT_THREAD 8
#define TITLE "Fractals"

void init();
int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData, menuSlider *ms);
void quit(SDL_Window *window, SDL_Renderer *renderer);
void createWindowRenderer(SDL_Window *window, SDL_Renderer *renderer);
void drawMandelbrot(SDL_Renderer *renderer, fractalData *fractalData, SDL_Texture *texture, Uint32 *pixels);
void *updatePixelArray(void *args);


int main(int argc, char *argv[])
{

    init();
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);

    if (window == NULL || renderer == NULL)
    {
        printf("Error while creating the window or renderer : %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowTitle(window, TITLE);
    SDL_SetWindowResizable(window, SDL_FALSE);

    int running = 1;
    int nbCores = 0;
    nbCores = sysconf(_SC_NPROCESSORS_ONLN); 

    // TODO : Create struct to hold framerate data
    Uint32 frameStart;
    int frameTime;
    int frameCounterTime = SDL_GetTicks();
    int frameCount = 0;
    int shouldDraw = 1;

    fractalData fractalData;
    initFractal(&fractalData);
    fractalData.width = WIDTH;
    fractalData.height = HEIGHT;
    fractalData.zoom_x = fractalData.width / (fractalData.x2 - fractalData.x1);
    fractalData.zoom_x = fractalData.height / (fractalData.y2 - fractalData.y1);

    // Texture data, it'll be using a 1D array representing all of the pixel that has to be rendered.
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
    Uint32 *pixels = (Uint32 *)calloc(WIDTH * HEIGHT, sizeof(Uint32));
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

    menuSlider ms;
    setUpMenuSliders(&ms, WIDTH, HEIGHT);



    mouseData mouseData = {0, 0, 0, 0, 0, 0};
    SDL_GetMouseState(&mouseData.x, &mouseData.y);

    // Threads that will be used to compute pixels
    pthread_t threads_id[nbCores - 1];
    threadPixelArg argsIndexed[nbCores - 1];
    float heightComputed = (float)HEIGHT / (float)nbCores;

    for (int i = 0; i < nbCores; i++)
    {
        argsIndexed[i].startHeight = i * heightComputed;
        argsIndexed[i].endHeight = argsIndexed[i].startHeight + heightComputed;
        if (i > 0)
        {
            if (argsIndexed[i].startHeight != argsIndexed[i - 1].endHeight)
            {
                argsIndexed[i - 1].endHeight++;
            }
        }
        printf("[%d] %d ; %d\n", i, argsIndexed[i].startHeight, argsIndexed[i].endHeight);

        argsIndexed[i].fd = &fractalData;
        argsIndexed[i].pixels = pixels;
        argsIndexed[i].running = 1;
        argsIndexed[i].n = i + 1;
        argsIndexed[i].mss = &ms;
        pthread_create(&threads_id[i], NULL, updatePixelArray, (void *)&argsIndexed[i]);
    }

    // We need to draw it a first time outside the main loop because it is only re-rendered
    // when an user-input occurs.
    while (running)
    {
        frameStart = SDL_GetTicks();
        shouldDraw = handleEvents(&running, &fractalData, &mouseData, &ms);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TIME)
        {
            SDL_Delay(FRAME_TIME - frameTime);
        }

        if (SDL_GetTicks() - frameCounterTime >= 1000)
        {
            //printf("%d FPS\n", frameCount);
            frameCount = 0;
            frameCounterTime = SDL_GetTicks();
        }
        fractalData.iterations = (float) (MAX_ITER - MIN_ITER) * getSliderValueByFlag(&ms, 'f');
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, ms.bgColor.r, ms.bgColor.g, ms.bgColor.b, ms.bgColor.a);
        SDL_RenderFillRect(renderer, &ms.rect);
        // Rendering sliders
        for (int i = 0; i < ms.sliderCount; i++)
        {
            SDL_SetRenderDrawColor(renderer, ms.sliders[i].slideColor.r, ms.sliders[i].slideColor.g, ms.sliders[1].slideColor.b, ms.sliders[i].slideColor.a);
            SDL_RenderFillRect(renderer, &ms.sliders[i].slide);
            SDL_SetRenderDrawColor(renderer, ms.sliders[i].btnColor.r, ms.sliders[i].btnColor.g, ms.sliders[i].btnColor.b, ms.sliders[i].btnColor.a);
            SDL_RenderFillRect(renderer, &ms.sliders[i].btn);
        }

        SDL_RenderPresent(renderer);
        frameCount++;
       // printf("Fractal Iter : %d\n", fractalData.iterations);
    }

    for (int i = 0; i < nbCores; i++)
    {
        argsIndexed[i].running = 0;
        pthread_join(threads_id[i], NULL);
    }

    free(ms.sliders);
    free(pixels);

    quit(window, renderer);
    return 0;
}

void init()
{
    printf("Initialize SDL . . .\n");
    SDL_Init(SDL_INIT_VIDEO);
    printf("Initialized !\n");
}

int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData, menuSlider *ms)
{
    /**
     * This function will handle events and return 1 if an user-input applied changes
     * to the Mandelbrot Set so we can render it again, otherwise it returns 0.
     * 
     *  - *running = Set it to 0 if we want app to stop.
     **/

    SDL_Event e;
    int stateModified = 0;
    int tmpX = 0, tmpY = 0, tmpBuffer = 0, mouseX, mouseY;
    ;
    float offset_x, offset_y, dx, dy;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            *running = 0;
            stateModified = 0;
            return stateModified;

        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                *running = 0;
                stateModified = 0;
                return stateModified;
            }
            break;

        case SDL_MOUSEWHEEL:
            if (e.wheel.y == 0)
                break;
            updateMouseData(mouseData);
            fHandleWheelMotion(fractalData, mouseData->x, mouseData->y, e.wheel.y);
            stateModified = 1;
            break;

        case SDL_MOUSEBUTTONDOWN:
            updateMouseData(mouseData);
            setSelectedSlider(ms, mouseData->x, mouseData->y);
            mouseData->pressed = 1;
            break;

        case SDL_MOUSEBUTTONUP:
            unselectSlider(ms);
            mouseData->pressed = 0;
            break;

        case SDL_MOUSEMOTION:
            if (mouseData->pressed == 0)
                break;
            updateMouseData(mouseData);
            if (handleClickedSlider(ms, mouseData->x, mouseData->y))
                break;
            dx = (float)mouseData->oldX - (float)mouseData->x;
            dy = (float)mouseData->oldY - (float)mouseData->y;
            fMove(fractalData, dx, dy);
            mouseData->lastUpdated = SDL_GetTicks();
            stateModified = 1;
            break;
        default:
            break;
        }
    }
    return stateModified;
}

void quit(SDL_Window *window, SDL_Renderer *renderer)
{
    printf("Quiting . . .\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("GoodBye, it's over now. . .\n");
}
// gcc src/main.c src/fractals.c src/fractals.h -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2; bin/main.exe
