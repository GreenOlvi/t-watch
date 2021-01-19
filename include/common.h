#ifndef common_h
#define common_h

#include "config.h"

struct point_t {
    int x;
    int y;
};

struct rect_t {
    int x;
    int y;
    int w;
    int h;
};

bool isInside(rect_t r, point_t p);
rect_t rectFromAbsolute(int x1, int y1, int x2, int y2);
point_t rectGetCentre(rect_t r);

#endif