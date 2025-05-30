#include "platform.h"
#include "charater.h"             // for Character_L and _Character_update_position
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h"// for global 'scene'
#include <stdlib.h>

/*
   [Platform function]
   Creates a static, invisible platform element for collision only.
*/
Elements *New_Platform(int label, float x, float y, int w, int h)
{
    // Allocate and initialize platform data
    Platform *pPlat = malloc(sizeof(Platform));
    pPlat->x = x;
    pPlat->y = y;
    pPlat->w = w;
    pPlat->h = h;

    // Create a new Elements object with the given label
    Elements *pObj = New_Elements(label);
    pObj->pDerivedObj = pPlat;

    // Register character as an interactable target
    pObj->inter_obj[pObj->inter_len++] = Character_L;

    // Assign lifecycle functions
    pObj->Update   = Platform_update;
    pObj->Draw     = Platform_draw;
    pObj->Interact = Platform_interact;
    pObj->Destroy  = Platform_destroy;

    return pObj;
}

void Platform_update(Elements *self)
{
    // Static platform: no per-frame update needed
}

void Platform_draw(Elements *self)
{
    // Invisible collision platform: no drawing
}

void Platform_interact(Elements *self)
{
    Platform *plat = (Platform *)self->pDerivedObj;
    // Retrieve all character elements
    ElementVec chars = _Get_label_elements(scene, Character_L);
    for (int i = 0; i < chars.len; i++) {
        Elements *tar = chars.arr[i];
        Character *chara = (Character *)tar->pDerivedObj;

        // Predict next Y position
        float nextY = chara->y + chara->jump_velocity;

        // AABB collision: character landing on platform
        if (chara->x + chara->width  > plat->x &&
            chara->x                < plat->x + plat->w &&
            chara->y + chara->height <= plat->y &&
            nextY + chara->height   >= plat->y) {

            // Snap character onto platform
            _Character_update_position(tar,
                0,
                plat->y - (chara->y + chara->height)
            );
            // Reset vertical velocity and state
            chara->ground_y         = plat->y - chara->height;
            chara->default_ground_y = plat->y - chara->height;
            chara->jump_velocity    = 0;
        }
    }
}

void Platform_destroy(Elements *self)
{
    free(self->pDerivedObj);
    free(self);
}
