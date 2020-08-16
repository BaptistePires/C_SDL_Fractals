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
    int width;
    int height;
};

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

typedef struct FractalData_s fractalData;

void initFractal(fractalData *fd);
void fAddZoom(fractalData *fd, float zoomOffset);
void fMove(fractalData *fd, float dx, float dy);
void fHandleWheelMotion(fractalData *fd, mouseData *md, int wheelY);

#endif