#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

#include "element.h"

// Invisible platform for collision detection only
// Use New_Platform(label, x, y, w, h) to register

typedef struct {
    float x, y;   // position
    int   w, h;   // size
} Platform;

// Create a new platform element
Elements *New_Platform(int label, float x, float y, int w, int h);

// Lifecycle functions
void Platform_update(Elements *self);
void Platform_draw(Elements *self);
void Platform_interact(Elements *self);
void Platform_destroy(Elements *self);

#endif // PLATFORM_H_INCLUDED
