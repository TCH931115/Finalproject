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

        // 1. 水平碰撞检测
        bool horizontal_overlap = 
            chara->x + chara->width > plat->x &&
            chara->x < plat->x + plat->w;
        
        bool vertical_overlap = 
            chara->y < plat->y + plat->h &&
            chara->y + chara->height > plat->y;
        
        if (horizontal_overlap && vertical_overlap) {
            // 计算角色中心与平台中心的相对位置
            float char_center = chara->x + chara->width / 2;
            float plat_center = plat->x + plat->w / 2;
            
            // 计算水平穿透深度
            float penetration_left = (chara->x + chara->width) - plat->x;
            float penetration_right = (plat->x + plat->w) - chara->x;
            
            // 根据角色位置决定推开方向
            if (char_center < plat_center) {
                // 角色在平台左侧，向右推开
                _Character_update_position(tar, -penetration_left, 0);
            } else {
                // 角色在平台右侧，向左推开
                _Character_update_position(tar, penetration_right, 0);
            }
        }

        // 2. 垂直碰撞检测（角色落在平台上）
        float nextY = chara->y + chara->jump_velocity;
        
        // 优化检测条件：确保角色正在下落
        bool is_falling = chara->jump_velocity > 0;
        
        if (horizontal_overlap &&
            is_falling &&
            chara->y + chara->height <= plat->y &&
            nextY + chara->height >= plat->y)
        {
            // 将角色放在平台顶部
            _Character_update_position(tar, 0, plat->y - (chara->y + chara->height));
            // 重置垂直速度和状态
            chara->jump_velocity = 0;
            chara->jump_count = 0; // 重置跳跃计数
            chara->state = STOP;
        }
    }
}

void Platform_destroy(Elements *self)
{
    free(self->pDerivedObj);
    free(self);
}
