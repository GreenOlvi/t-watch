#include "common.h"

bool isInside(rect_t r, point_t p) {
    return p.x >= r.x
        && p.y >= r.y
        && p.x <= r.x + r.w
        && p.y <= r.y + r.h;
}