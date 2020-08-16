#include "fractals.h"
#include <stdio.h>

/**
 * This file will contains all of structs/functions related to the Mandelbrot set.
 */

// TODO : move zoom here and make it nice not trash

void initFractal(fractalData *fd) {
    fd->iterations = 100;
    fd->x1 = -2.1f;
    fd->y1 = -1.2f;
    fd->x2 = 0.6f;
    fd->y2 = 1.2f;
    fd->zoom_x = 0;
    fd->zoom_y = 0;
    fd->width = 0;
    fd->height = 0;
}



void fAddZoom(fractalData *fd, float zoomOffset) {
    float tmpX = fd->zoom_x + zoomOffset;
    float tmpY = fd->zoom_y + zoomOffset;

   if(tmpX < MIN_ZOOM || tmpY < MIN_ZOOM){
        fd->zoom_x = MIN_ZOOM;
        fd->zoom_y = MIN_ZOOM;
    } 
    else{
        fd->zoom_x = tmpX;
        fd->zoom_y = tmpY;
    }

    printf("zoom : %f\n", fd->zoom_x);

}

void fMove(fractalData *fd, float dx, float dy) {
    if(fd->zoom_x > 500000) {
        fd->x1 += dx / fd->zoom_x ;
        fd->y1 += dy / fd->zoom_x;
    }else if(fd->zoom_x > 100000) {
        fd->x1 += dx / fd->zoom_x ;
        fd->y1 += dy / fd->zoom_x;
        
    }else if(fd->zoom_x > 30000){
        fd->x1 += dx / fd->zoom_x ;
        fd->y1 += dy / fd->zoom_x;
    }else{
        fd->x1 += dx / fd->zoom_x;
        fd->y1 += dy / fd->zoom_x;
    }
}

void fHandleWheelMotion(fractalData *fd, int mx, int my, int wheelY) {
    // TODO / Update this trash zoom
    float zoom = fd->zoom_x * 0.3;
    if(mx > fd->width / 2) {
        fd->x1 -= 100 / fd->zoom_x;
    }else{
        fd->x1 += 100 / fd->zoom_x;
    }

    if(my > fd->height / 2) {
        fd->y1 -= 100 / fd->zoom_x;
    }else{
        fd->y1 += 100 / fd->zoom_x;
    }
    if(wheelY < 0) zoom = -zoom;
    fAddZoom(fd, zoom);
}
