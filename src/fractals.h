#ifndef HEADER_FILE
#define HEADER_FILE

#define MAX_ZOOM 100000
#define MIN_ZOOM 50

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
} ;

typedef struct FractalData_s fractalData;

void initFractal(fractalData *fd);
void fAddZoom(fractalData *fd, float zoomOffset);


#endif