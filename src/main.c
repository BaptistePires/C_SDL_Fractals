#include <stdio.h>
#include <Windows.h>
#include <SDL.h>
#include <math.h>
#include "fractals.h"

#define FPS 144.0
#define FRAME_TIME 1000.0 / FPS
#define WIDTH 1000  
#define HEIGHT 800

struct MouseData_s {
    int oldX;
    int oldY;
    int x;
    int y;
    int pressed;
    long lastUpdated;
} MouseData_Default = {0, 0, 0, 0, 0, 0};


typedef struct MouseData_s mouseData;
void init();
int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData);
void quit(SDL_Window *window, SDL_Renderer *renderer);
void createWindowRenderer(SDL_Window *window, SDL_Renderer *renderer);
void drawMander(SDL_Renderer *renderer, fractalData *fractalData, SDL_Texture *texture, Uint32 *pixels);


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
    Uint32 frameStart; 
    int frameTime;
    int frameCounterTime = SDL_GetTicks();
    int frameCount = 0;
    int shouldDraw = 1;
    fractalData fractalData;
    initFractal(&fractalData);
    
    fractalData.zoom_x = WIDTH / (fractalData.x2 - fractalData.x1);
    fractalData.zoom_x = HEIGHT / (fractalData.y2 - fractalData.y1);
    printf("%f\n", fractalData.zoom_x );

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
    Uint8 red = 255, green = 0, blue = 0, alpha = 255;
    Uint32 *pixels = (Uint32 *) calloc(WIDTH * HEIGHT,  sizeof(Uint32));
    for(int i = 0; i < WIDTH * HEIGHT; i++) {
        // pixels[i] = (255 << 24) + (0 << 16) + (0 << 8) + 0;
        pixels[i] = 0xFFFFFF;
    }
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
    mouseData mouseData = MouseData_Default;
    SDL_GetMouseState(&mouseData.x, &mouseData.y);
    

    drawMander(renderer, &fractalData, texture, pixels);
    while(running) {
        frameStart = SDL_GetTicks();
        shouldDraw = handleEvents(&running, &fractalData, &mouseData);
    
        if(shouldDraw == 1){
            drawMander(renderer, &fractalData, texture, pixels);
            shouldDraw = 0;
        } 
        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }

        if(SDL_GetTicks() - frameCounterTime >= 1000) {
            printf("%d FPS\n", frameCount);
            frameCount = 0;
            frameCounterTime = SDL_GetTicks();
        }
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        frameCount++;

        // printf("fd x1 : %d ; y1 %d \n", fractalData.x1, fractalData.y1);
    }

    free(pixels);
    quit(window, renderer);
    return 0;
}

void drawMander(SDL_Renderer *renderer, fractalData *fractalData, SDL_Texture *texture, Uint32 *pixels) {
     int iters = 50;

    float c_r, c_i, z_r, tmp_zr, z_i, i;
    
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderClear(renderer);

    for(int x = 0; x < WIDTH ; x++) {
        for(int y = 0; y < HEIGHT ;y++) {
            c_r = x / fractalData->zoom_x + fractalData->x1;
            c_i = y / fractalData->zoom_x + fractalData->y1;
            z_r = 0;
            z_i = 0;
            i = 0;

            while((z_r*z_r + z_i*z_i) < 4 && i < fractalData->iterations) {
                tmp_zr = z_r;
                z_r = z_r*z_r - z_i*z_i + c_r;
                z_i = 2 * z_i * tmp_zr + c_i;
                i++;
            }

            // if(i == fractalData->iterations) {
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);          
            // }else{
            //     SDL_SetRenderDrawColor(renderer, i * 255 / iters, i * 255 / iters, 0, 255);
            // }

            // SDL_RenderDrawPoint(renderer,x, y);
            // printf("%d\n", y * HEIGHT + x);
            if(i == fractalData->iterations) {
                pixels[y * WIDTH + x] = 0xFFFFFFFF;
            } else{
                pixels[y * WIDTH + x] = (0 << 24) + ((int) (i * 0 / iters) << 16) + ((int) (i * 255 / iters) << 8) + ((int) (i * 255 / iters) );
            }
            
        }

    }
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
}

void init() {
    printf("Initialize SDL . . .\n");
    SDL_Init(SDL_INIT_VIDEO);
    printf("Initializes !\n");
}


int handleEvents(int *running, fractalData *fractalData, mouseData *mouseData) {
    SDL_Event e;
    int stateModified = 0;
    int tmpX = 0, tmpY = 0, tmpBuffer = 0;
    float offset_x, offset_y;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
             case SDL_QUIT:
                *running = 0;
                break;
            
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_KP_SPACE) {
                    // initFractal(*fractalData);
                    stateModified = 1;
                    return stateModified;
                }
                fractalData->zoom_x -= 50;
                fractalData->zoom_y += 50;
                stateModified =  1;
                break;

            case SDL_MOUSEWHEEL:
                if(e.wheel.y == 0) break;
                int mouseX, mouseY;
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
                if(mouseData->pressed == 0 || (SDL_GetTicks() - mouseData->lastUpdated) < 10) break;
                
                mouseData->oldX = mouseData->x;
                mouseData->oldY = mouseData->y;
                SDL_GetMouseState(&mouseData->x, &mouseData->y);

                float dx = mouseData->oldX - mouseData->x;
                float dy = mouseData->oldY - mouseData->y;

                fractalData->x1 += dx / 1000 + 1 / fractalData->zoom_x;
                fractalData->y1 += dy / 1000 + 1 / fractalData->zoom_y;
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
    printf("GoodBye, all trerminated. . .\n");
}
// gcc src/main.c src/fractals.c src/fractals.h -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2; bin/main.exe