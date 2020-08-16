#ifndef HEADER_GUIMENU
#define HEADER_GUIMENU

#include <stdio.h>
#include <stdlib.h>

#ifdef unix
#include <SDL2/SDL.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#include <SDL.h>
#endif


struct ColorRGBA {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

typedef struct ColorRGBA colorRGBA;

struct Slider {
    SDL_Rect slide;
    colorRGBA slideColor;
    SDL_Rect btn;
    colorRGBA btnColor;
    int selected;
    float value;
    char flag;
};

typedef struct Slider slider;

struct MenuSlider {
    SDL_Rect rect;
    colorRGBA bgColor;
    slider *sliders;
    int sliderCount;
};
typedef struct MenuSlider menuSlider;


void setUpMenuSliders(menuSlider *ms,  float width, float height);
int handleClickedSlider(menuSlider *ms, float mx, float my);
int isPointOnRect(SDL_Rect *rect, int x, int y);
void setSelectedSlider(menuSlider *ms, int mx, int my);
void unselectSlider(menuSlider *ms);
float getSliderValueByFlag(menuSlider *ms, char f);
#endif