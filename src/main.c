#ifdef unix
#include <SDL2/SDL.h>
#include <sys/sysinfo.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#include<SDL.h>
#include<Windows.h>
#define HAVE_STRUCT_TIMESPEC
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractals.h"
#include <pthread.h>

#define FPS 144.0
#define FRAME_TIME 1000.0 / FPS
#define WIDTH 1000  
#define HEIGHT 800
#define COUNT_THREAD 8
#define TITLE "Fractals"



struct ThreadPixelArg {
        int startHeight;
        int endHeight;
        fractalData *fd;
        Uint32 *pixels;
        int running;
        int n;
};

typedef struct ThreadPixelArg threadPixelArg;

void init();
int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData);
void quit(SDL_Window *window, SDL_Renderer *renderer);
void createWindowRenderer(SDL_Window *window, SDL_Renderer *renderer);
void drawMandelbrot(SDL_Renderer *renderer, fractalData *fractalData, SDL_Texture *texture, Uint32 *pixels);
void *updatePixelArray(void *args);


int main(int argc, char* argv[]) {

    init();
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);


    if(window == NULL || renderer == NULL) {
        printf("Error while creating the window or renderer : %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowTitle(window, TITLE);
    SDL_SetWindowResizable(window, SDL_TRUE);

    int running = 1;
    int nbCores = 0;
    #ifdef unix
        nbCores = nbProc();
    #endif

    #if defined(_WIN32) || defined(WIN32)
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        nbCores = sysinfo.dwNumberOfProcessors;
    #endif
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
    Uint32 *pixels = (Uint32 *) calloc(WIDTH * HEIGHT,  sizeof(Uint32));
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
    
    mouseData mouseData = {0, 0, 0, 0, 0, 0};
    SDL_GetMouseState(&mouseData.x, &mouseData.y);

    // Threads that will be used to compute pixels
    pthread_t threads_id[nbCores - 1];
    threadPixelArg argsIndexed[nbCores - 1];
    float heightComputed = (float) HEIGHT / (float)nbCores;


    for(int i = 0; i < nbCores; i++) {
        argsIndexed[i].startHeight = i * heightComputed;
        argsIndexed[i].endHeight = argsIndexed[i].startHeight + heightComputed ;
        if(i > 0) {
            if(argsIndexed[i].startHeight != argsIndexed[i - 1].endHeight) {
                argsIndexed[i - 1].endHeight++;
            }
        }
        printf("[%d] %d ; %d\n", i, argsIndexed[i].startHeight, argsIndexed[i].endHeight);

        argsIndexed[i].fd = &fractalData;
        argsIndexed[i].pixels = pixels;
        argsIndexed[i].running = 1;
        argsIndexed[i].n = i + 1;
        pthread_create(&threads_id[i], NULL, updatePixelArray, (void *)&argsIndexed[i]);
    }
    /*for(int i = 0; i < nbCores; i++) {
        pthread_create(&threads_id[i], NULL, updatePixelArray, (void *)&argsIndexed[i]);
    }*/
    printf("Thread and args done;\n");
    
    // We need to draw it a first time outside the main loop because it is only re-rendered
    // when an user-input occurs.
    while(running) {
        frameStart = SDL_GetTicks();
        shouldDraw = handleEvents(&running, &fractalData, &mouseData);
        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }

        if(SDL_GetTicks() - frameCounterTime >= 1000) {
            printf("%d FPS\n", frameCount);
            frameCount = 0;
            frameCounterTime = SDL_GetTicks();
        }

        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        frameCount++;
    }

    for(int i = 0; i < nbCores; i++) {
        argsIndexed[i].running = 0;
        pthread_join(threads_id[i], NULL);
    }

    free(pixels);
    quit(window, renderer);
    return 0;
}

void* updatePixelArray(void* args) {
    /**
     * This function is called in threads, each thread will handle a slice of the window to render, boundaries are in arg.
     */
    threadPixelArg *data = (threadPixelArg *)args;
    printf("Thread n°%d created ! \n", data->n);
    float c_r, c_i, z_r, tmp_zr, z_i, i;
    int x, y;
    while(data->running == 1) {
        for(x = 0; x < data->fd->width ; x++) {
                for(y = data->startHeight; y < data->endHeight ;y++) {
                    c_r = x / data->fd->zoom_x + data->fd->x1;
                    c_i = y / data->fd->zoom_x + data->fd->y1;
                    z_r = 0;
                    z_i = 0;
                    i = 0;

                    while((z_r*z_r + z_i*z_i) < 4 && i < data->fd->iterations) {
                        tmp_zr = z_r;
                        z_r = z_r*z_r - z_i*z_i + c_r;
                        z_i = 2 * z_i * tmp_zr + c_i;
                        i++;
                    }

                    if(i == data->fd->iterations) {
                        data->pixels[y * WIDTH + x] = 0xFFFFFFFF;
                    } else{
                        data->pixels[y * WIDTH + x] = (255 << 24) + ((int) (i * 255 / data->fd->iterations) << 16) + ((int) (i *  0 / data->fd->iterations) << 8) + ((int) (i * 255 / data->fd->iterations) );
                    }
                }

            }
    }
    printf("quit thread n°%d ! \n", data->n);
    return 0;
}

void init() {
    printf("Initialize SDL . . .\n");
    SDL_Init(SDL_INIT_VIDEO);
    printf("Initialized !\n");
}


int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData) {
    /**
     * This function will handle events and return 1 if an user-input applied changes
     * to the Mandelbrot Set so we can render it again, otherwise it returns 0.
     * 
     *  - *running = Set it to 0 if we want app to stop.
     **/

    SDL_Event e;
    int stateModified = 0;
    int tmpX = 0, tmpY = 0, tmpBuffer = 0, mouseX, mouseY;;
    float offset_x, offset_y, dx, dy;

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
             case SDL_QUIT:
                *running = 0;
                stateModified = 0;
                return stateModified;
            
            case SDL_KEYDOWN:

                if(e.key.keysym.sym == SDLK_ESCAPE) {
                    *running = 0;
                    stateModified =  0;
                    return stateModified;
                }
                break;

            case SDL_MOUSEWHEEL:
                if(e.wheel.y == 0) break;
                mouseData->oldX = mouseData->x;
                mouseData->oldY = mouseData->y;
                SDL_GetMouseState(&mouseX, &mouseY);
                fHandleWheelMotion(fractalData, mouseData, e.wheel.y);
                stateModified =  1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouseData->x, &mouseData->y);
                mouseData->pressed = 1;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseData->pressed = 0;
                break;

            case SDL_MOUSEMOTION:
                if(mouseData->pressed == 0) break;
                mouseData->oldX = mouseData->x;
                mouseData->oldY = mouseData->y;
                SDL_GetMouseState(&mouseData->x, &mouseData->y);
                dx = (float) mouseData->oldX - (float)  mouseData->x;
                dy = (float) mouseData->oldY - (float) mouseData->y;
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

void quit(SDL_Window *window, SDL_Renderer *renderer) {
    printf("Quiting . . .\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
    printf("GoodBye, it's over now. . .\n");
}
// gcc src/main.c src/fractals.c src/fractals.h -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2; bin/main.exe
