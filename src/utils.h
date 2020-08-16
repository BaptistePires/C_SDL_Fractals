#ifndef HEADER_UTILS
#define HEADER_UTILS

#ifdef unix
#include <SDL2/SDL.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#include <SDL.h>
#endif

#include "fractals.h"
#include "guiMenu.h"

// Create separated files for handling input -> will stay here atm
struct MouseData_s {
    int oldX;
    int oldY;
    int x;
    int y;
    int pressed;
    long lastUpdated;
} ;

typedef struct MouseData_s mouseData;

// This struct will be used as arg. given to the thread to render the
struct ThreadPixelArg {
        int startHeight;
        int endHeight;
        fractalData *fd;
        Uint32 *pixels;
        int running;
        int n;
        menuSlider *mss;
};

typedef struct ThreadPixelArg threadPixelArg;


void updateMouseData(mouseData *md);
void* updatePixelArray(void* args);

#endif