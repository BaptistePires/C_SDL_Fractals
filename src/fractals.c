#include "fractals.h"
#include <stdio.h>

void initFractal(fractalData *fd) {
    fd->iterations = 100;
    fd->x1 = -2.1f;
    fd->y1 = -1.2f;
    fd->x2 = 0.6f;
    fd->y2 = 1.2f;
    fd->zoom_x = 100;
    fd->zoom_y = 100;
}

void fAddZoom(fractalData *fd, float zoomOffset) {
    float tmpX = fd->zoom_x + zoomOffset;
    float tmpY = fd->zoom_y + zoomOffset;
    // if(tmpX > MAX_ZOOM || tmpY > MAX_ZOOM){
    //     fd->zoom_x = MAX_ZOOM;
    //     fd->zoom_y = MAX_ZOOM;
    //     return;
    // } 
   if(tmpX < MIN_ZOOM || tmpY < MIN_ZOOM){
        fd->zoom_x = MIN_ZOOM;
        fd->zoom_y = MIN_ZOOM;
    } 
    else{
        fd->zoom_x = tmpX;
        fd->zoom_y = tmpY;
    } 

        printf("%f\n", fd->zoom_x);
    // if(tmpy > MAX_ZOOM) fd->zoom_y = MAX_ZOOM;
    // else if(tmpy < MIN_ZOOM) fd->zoom_y = MIN_ZOOM;
    // else fd->zoom_y = zoomOffset;
}