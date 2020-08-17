#ifndef HEADER_FRACTALS
#define HEADER_FRACTALS

#define MAX_ZOOM 100000
#define MIN_ZOOM 50
#define MIN_ITER 20
#define MAX_ITER 1000

struct FractalData_s {
    int iterations;
    float x1;
    float y1;
    float x2;
    float y2;
    float zoom_x;
    float zoom_y;
    float offset_x;
    float offset_y;
    int width;
    int height;
};

typedef struct FractalData_s fractalData;

void initFractal(fractalData *fd);
void fAddZoom(fractalData *fd, float zoomOffset);
void fMove(fractalData *fd, float dx, float dy);
void fHandleWheelMotion(fractalData *fd, int mx, int my, int wheelY);

#endif