#include "guiMenu.h"



void setUpMenuSliders(menuSlider *ms, float width, float height) {
    ms->sliderCount = 5;
    ms->rect.x = 0;
    ms->rect.y = 0;
    ms->rect.w = width / 3;
    ms->rect.h = height / 3;
    ms->bgColor.r =255;
    ms->bgColor.g =255;
    ms->bgColor.b =255;
    ms->bgColor.a =255;
    ms->sliders = (slider *) malloc(sizeof(slider) * ms->sliderCount);

    ms->sliders[0].slide.x = 10;
    ms->sliders[0].slide.y = 30;
    ms->sliders[0].slide.w = ms->rect.w - ms->sliders[0].slide.x * 2;
    ms->sliders[0].slide.h = 5;
    ms->sliders[0].slideColor.r = 0;
    ms->sliders[0].slideColor.g = 0;
    ms->sliders[0].slideColor.b = 0;
    ms->sliders[0].slideColor.a = 0;

    ms->sliders[0].btn.w =15;
    ms->sliders[0].btn.h = 25;
    ms->sliders[0].btn.x = 10;
    ms->sliders[0].btn.y = ms->sliders[0].slide.y + ms->sliders[0].slide.h /2  - ms->sliders[0].btn.h / 2;
    
    ms->sliders[0].btnColor.r = 255;
    ms->sliders[0].btnColor.g = 0;
    ms->sliders[0].btnColor.b = 0;
    ms->sliders[0].btnColor.a = 255;
    ms->sliders[0].flag = 'r';
    ms->sliders[0].txtRect.x = 1;
    ms->sliders[0].txtRect.y = 25;


    // slider 2 
    ms->sliders[1].slide.x = 10;
    ms->sliders[1].slide.y = 70;
    ms->sliders[1].slide.w = ms->rect.w - ms->sliders[1].slide.x * 2;
    ms->sliders[1].slide.h = 5;
    ms->sliders[1].slideColor.r = 0;
    ms->sliders[1].slideColor.g = 0;
    ms->sliders[1].slideColor.b = 0;
    ms->sliders[1].slideColor.a = 0;

    ms->sliders[1].btn.w =  15;
    ms->sliders[1].btn.h = 25;
    ms->sliders[1].btn.x = 10;
    ms->sliders[1].btn.y = ms->sliders[1].slide.y + ms->sliders[1].slide.h /2  - ms->sliders[1].btn.h / 2;
    
    ms->sliders[1].btnColor.r = 0;
    ms->sliders[1].btnColor.g = 255;
    ms->sliders[1].btnColor.b = 0;
    ms->sliders[1].btnColor.a = 0;
    ms->sliders[1].flag = 'g';
    ms->sliders[0].txtRect.x = 1;
    ms->sliders[0].txtRect.y = 65;

    // slider 3
    ms->sliders[2].slide.x = 10;
    ms->sliders[2].slide.y = 110;
    ms->sliders[2].slide.w = ms->rect.w - ms->sliders[2].slide.x * 2;
    ms->sliders[2].slide.h = 5;
    ms->sliders[2].slideColor.r = 0;
    ms->sliders[2].slideColor.g = 0;
    ms->sliders[2].slideColor.b = 0;
    ms->sliders[2].slideColor.a = 0;

    ms->sliders[2].btn.w =  15;
    ms->sliders[2].btn.h = 25;
    ms->sliders[2].btn.x = 10;
    ms->sliders[2].btn.y = ms->sliders[2].slide.y + ms->sliders[2].slide.h /2  - ms->sliders[2].btn.h / 2;
    
    ms->sliders[2].btnColor.r = 0;
    ms->sliders[2].btnColor.g = 0;
    ms->sliders[2].btnColor.b = 255;
    ms->sliders[2].btnColor.a = 0;
    ms->sliders[2].flag = 'b';
    ms->sliders[2].txtRect.x = 1;
    ms->sliders[2].txtRect.y = 105;

    // slider 3
    ms->sliders[3].slide.x = 10;
    ms->sliders[3].slide.y = 150;
    ms->sliders[3].slide.w = ms->rect.w - ms->sliders[3].slide.x * 2;
    ms->sliders[3].slide.h = 5;
    ms->sliders[3].slideColor.r = 0;
    ms->sliders[3].slideColor.g = 0;
    ms->sliders[3].slideColor.b = 0;
    ms->sliders[3].slideColor.a = 0;

    ms->sliders[3].btn.w =  15;
    ms->sliders[3].btn.h = 25;
    ms->sliders[3].btn.x = 10;
    ms->sliders[3].btn.y = ms->sliders[3].slide.y + ms->sliders[3].slide.h /2  - ms->sliders[3].btn.h / 2;
    
    ms->sliders[3].btnColor.r = 0;
    ms->sliders[3].btnColor.g = 0;
    ms->sliders[3].btnColor.b = 0;
    ms->sliders[3].btnColor.a = 0;
    ms->sliders[3].flag = 'a';
    ms->sliders[0].txtRect.x = 1;
    ms->sliders[0].txtRect.y = 145;

    // slider 4
    ms->sliders[4].slide.x = 10;
    ms->sliders[4].slide.y = 190;
    ms->sliders[4].slide.w = ms->rect.w - ms->sliders[4].slide.x * 2;
    ms->sliders[4].slide.h = 5;
    ms->sliders[4].slideColor.r = 0;
    ms->sliders[4].slideColor.g = 0;
    ms->sliders[4].slideColor.b = 0;
    ms->sliders[4].slideColor.a = 0;

    ms->sliders[4].btn.w =  15;
    ms->sliders[4].btn.h = 25;
    ms->sliders[4].btn.x = 85;
    ms->sliders[4].btn.y = ms->sliders[4].slide.y + ms->sliders[4].slide.h /2  - ms->sliders[4].btn.h / 2;
    ms->sliders[4].value = 0.5f;

    ms->sliders[4].btnColor.r = 0;
    ms->sliders[4].btnColor.g = 0;
    ms->sliders[4].btnColor.b = 0;
    ms->sliders[4].btnColor.a = 0;
    ms->sliders[4].flag = 'f';
    ms->sliders[4].txtRect.x = 1;
    ms->sliders[4].txtRect.y = 145;

}

int handleClickedSlider(menuSlider *ms, float mx, float my) {
    if(ms->sliders[0].selected == 0) return 0;
    int tmpX = mx;
    if(mx > ms->sliders[0].slide.x && mx <  ms->sliders[0].slide.x +  ms->sliders[0].slide.w){
            ms->sliders[0].btn.x = mx;
            ms->sliders[0].value = (ms->sliders[0].btn.x - ms->sliders[0].slide.x) / (float) ms->sliders[0].slide.w;
            printf("Value slider : %f  %c\n", ms->sliders[0].value, ms->sliders[0].flag);
    }

    return 1;
}

void setSelectedSlider(menuSlider *ms, int mx, int my) {
    /**
     * Always set the selected pointer first in the array
     */
    slider tmpSlider;
    for(int i = 0; i < ms->sliderCount; i++)  {
        if(isPointOnRect(&ms->sliders[i].btn, mx, my))  {

            if (i==0){
                ms->sliders[0].selected = 1;
                return;
            }
            else{
                tmpSlider = ms->sliders[0];
                ms->sliders[0] = ms->sliders[i];
                ms->sliders[0].selected = 1;
                ms->sliders[i] = tmpSlider;
                printf("0 : %f, i : %f\n", ms->sliders[0].btn.x, ms->sliders[i].btn.x);

                return;
            }
        }
    }
    return;
}

void unselectSlider(menuSlider *ms) {
    ms->sliders[0].selected = 0;
}

int isPointOnRect(SDL_Rect *rect, int x, int y) {
    SDL_Point pt;
    pt.x = x; 
    pt.y = y;
    if(SDL_PointInRect(&pt, rect) == SDL_TRUE) return 1;
    else return 0;
}

float getSliderValueByFlag(menuSlider *ms, char f) {
    for(int i = 0; i < ms->sliderCount;i++) {
        if(ms->sliders[i].flag == f) return ms->sliders[i].value;
    }
    return 1;
}
