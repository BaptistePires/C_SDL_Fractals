#include "utils.h"
#include <stdio.h>


void updateMouseData(mouseData *md) {
    md->oldX = md->x;
    md->oldY = md->y;
    SDL_GetMouseState(&md->x, &md->y);
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
                        data->pixels[y * data->fd->width + x] = 0xFFFFFFFF;
                    } else{
                        data->pixels[y * data->fd->width + x] = (255 << 24) + ((int) (i * 255 / data->fd->iterations) << 16) + ((int) (i *  0 / data->fd->iterations) << 8) + ((int) (i * 255 / data->fd->iterations) );
                    }
                }

            }
    }
    printf("quit thread n°%d ! \n", data->n);
    return 0;
}