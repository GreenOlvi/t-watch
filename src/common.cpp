#include "common.h"

bool isInside(rect_t r, point_t p) {
    return p.x >= r.x
        && p.y >= r.y
        && p.x <= r.x + r.w
        && p.y <= r.y + r.h;
}

rect_t rectFromAbsolute(int x1, int y1, int x2, int y2) {
    return rect_t{
        .x = x1,
        .y = y1,
        .w = x2 - x1,
        .h = y2 - y1,
    };
}

point_t rectGetCentre(rect_t r) {
    return point_t{
        .x = r.x + r.w / 2,
        .y = r.y + r.h / 2,
    };
}