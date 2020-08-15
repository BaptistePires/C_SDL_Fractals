#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "fractals.h"
#include <pthread.h>

#define FPS 144.0
#define FRAME_TIME 1000.0 / FPS
#define WIDTH 1000  
#define HEIGHT 800
#define COUNT_THREAD 4


struct MouseData_s {
    int oldX;
    int oldY;
    int x;
    int y;
    int pressed;
    long lastUpdated;
} MouseData_Default = {0, 0, 0, 0, 0, 0};

typedef struct MouseData_s mouseData;

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


    if(window == NULL) {
        printf("Error while creating the window : %s\n", SDL_GetError());
        return -1;
    }

    int running = 1;

    // TODO : Create struct to hold framerate data
    Uint32 frameStart; 
    int frameTime;
    int frameCounterTime = SDL_GetTicks();
    int frameCount = 0;
    int shouldDraw = 1;

    
    fractalData fractalData;
    initFractal(&fractalData);
    fractalData.zoom_x = WIDTH / (fractalData.x2 - fractalData.x1);
    fractalData.zoom_x = HEIGHT / (fractalData.y2 - fractalData.y1);
    
    // Texture data, it'll be using a 1D array representing all of the pixel that has to be rendered.
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
    Uint32 *pixels = (Uint32 *) calloc(WIDTH * HEIGHT,  sizeof(Uint32));
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
    
    mouseData mouseData = MouseData_Default;
    SDL_GetMouseState(&mouseData.x, &mouseData.y);

    // Threads that will be used to compute pixels
    pthread_t threads_id[COUNT_THREAD- 1];
    threadPixelArg argsIndexed[COUNT_THREAD - 1];
    int heightComputed = HEIGHT / COUNT_THREAD;
    for(int i = 0; i < COUNT_THREAD; i++) {
        argsIndexed[i].startHeight = i * heightComputed;
        argsIndexed[i].endHeight = argsIndexed[i].startHeight + heightComputed ;
        argsIndexed[i].fd = &fractalData;
        argsIndexed[i].pixels = pixels;
        argsIndexed[i].running = 1;
        argsIndexed[i].n = i + 1;
    }
    for(int i = 0; i < COUNT_THREAD; i++) {
        pthread_create(&threads_id[i], NULL, updatePixelArray, (void *)&argsIndexed[i]);
    }
    printf("Thread and args done;\n");
    
    // We need to draw it a first time outside the main loop because it is only re-rendered
    // when an user-input occurs.
    //drawMandelbrot(renderer, &fractalData, texture, pixels);
    while(running) {
        frameStart = SDL_GetTicks();
        shouldDraw = handleEvents(&running, &fractalData, &mouseData);
    
/*        if(shouldDraw == 1 || shouldDraw == 0){
            
            for(int i = 0; i < COUNT_THREAD; i++) {
                pthread_create(&threads_id[i], NULL, updatePixelArray, (void *)&argsIndexed[i]);
            }
            for(int i = 0; i < COUNT_THREAD; i++) {
                pthread_join(&threads_id[i], NULL);
            }
            
            //drawMandelbrot(renderer, &fractalData, texture, pixels);
            SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

            shouldDraw = 0;
        } */
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

    for(int i = 0; i < COUNT_THREAD; i++) {
        argsIndexed[i].running = 0;
        pthread_join(threads_id[i], NULL);
    }

    free(pixels);
    quit(window, renderer);
    return EXIT_SUCCESS;
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
        for(x = 0; x < WIDTH ; x++) {
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
                        data->pixels[y * WIDTH + x] = (0 << 24) + ((int) (i * 255 / data->fd->iterations) << 16) + ((int) (i * 0 / data->fd->iterations) << 8) + ((int) (i * 255 / data->fd->iterations) );
                    }
                }

            }
    }
    printf("quit thread n°%d ! \n", data->n);
    return EXIT_SUCCESS;
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
    float offset_x, offset_y;

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
             case SDL_QUIT:
                *running = 0;
                stateModified = 0;
                return stateModified;
            
            case SDL_KEYDOWN:

                if(e.key.keysym.sym == SDLK_KP_SPACE) {
                    initFractal(fractalData);
                    stateModified = 1;
                    return stateModified;
                }
                fractalData->zoom_x -= 50;
                fractalData->zoom_y += 50;
                stateModified =  1;
                break;

            case SDL_MOUSEWHEEL:
                if(e.wheel.y == 0) break;
                SDL_GetMouseState(&mouseX, &mouseY);
                float zoom = fractalData->zoom_x * 0.3;
                if(e.wheel.y < 0) zoom = -zoom;
                fAddZoom(fractalData, zoom);
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
                if(mouseData->pressed == 0 ) break;
                
                mouseData->oldX = mouseData->x;
                mouseData->oldY = mouseData->y;
                SDL_GetMouseState(&mouseData->x, &mouseData->y);
                // /printf("mousex: %d, mousey: %d\n", mouseData->x, mouseData->y);

                float dx = (float) mouseData->oldX - (float)  mouseData->x;
                float dy = (float) mouseData->oldY - (float) mouseData->y;
                if(fractalData->zoom_x > 100000) {
                    fractalData->x1 += dx / 100000.0 ;
                    fractalData->y1 += dy / 100000.0;
                    
                }else if(fractalData->zoom_x > 30000){
                    fractalData->x1 += dx / 10000.0 ;
                    fractalData->y1 += dy / 10000.0;
                }else{
                    fractalData->x1 += dx / 1000.0 ;
                    fractalData->y1 += dy / 1000.0;
                }

                printf("dx: %d, dy: %d\n", dx, dy);
                /*fractalData->x1 += 1;
                fractalData->y1 += 1;*/
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
