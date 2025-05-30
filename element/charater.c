#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "charater.h"
#include "projectile.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "../element/platform.h"
#include <stdio.h>
#include <stdbool.h>
#include "../GameWindow.h"

/*
   [Character function]
*/
Elements *New_Character(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[3][10] = {"stop", "move", "jump"};
    for (int i = 0; i < 3; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/prince_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    pDerivedObj->width = pDerivedObj->gif_status[0]->width*0.5;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height*0.5;
    pDerivedObj->x = 300;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 60;
    pDerivedObj->ground_y = pDerivedObj->y;        // current ground
    pDerivedObj->default_ground_y = pDerivedObj->y; // default ground
    pDerivedObj->jump_velocity = 0;  // ← 一定要把速度先归零
    pDerivedObj->jump_count    = 0;  // ← 还可以初始化二段跳计数
    pDerivedObj->prev_space    = false; // ← 上一帧空格键状态
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}
void Character_update(Elements *self)
{
    if (g_paused) return;
    Character *chara = (Character *)(self->pDerivedObj);
    if (!chara) return;

    // 0) 平台检测：动态恢复 default_ground_y
    bool on_any_platform = false;
    ElementVec plats = _Get_label_elements(scene, Platform_L);
    for (int i = 0; i < plats.len; i++) {
        Platform *p = (Platform *)plats.arr[i]->pDerivedObj;
        if (chara->x + chara->width  > p->x &&
            chara->x                < p->x + p->w &&
            fabs((chara->y + chara->height) - p->y) < 1.0f) {
            on_any_platform = true;
            break;
        }
    }
    if (!on_any_platform) {
        chara->default_ground_y = HEIGHT - chara->height - 60;
        chara->ground_y         = chara->default_ground_y;   // ← 同步还原 ground_y
    }

    // 1) 检测跳跃按键 —— 就是这一段！
    bool space_pressed = key_state[ALLEGRO_KEY_SPACE];
    bool space_just_pressed = space_pressed && !chara->prev_space;
    if (space_just_pressed && chara->jump_count < 2) {
        chara->jump_velocity = -15;
        chara->jump_count++;
    }

    // 2) 垂直物理
    chara->y += chara->jump_velocity;
    chara->jump_velocity += 1;

    // 3) 落地检测
    if (chara->y >= chara->default_ground_y) {
        chara->y = chara->default_ground_y;
        chara->jump_velocity = 0;
        chara->jump_count    = 0;
        chara->state         = STOP;
    }
    // --------------------
    // ⬅️➡️ 水平方向（移動）
    // --------------------
    if (key_state[ALLEGRO_KEY_A])
    {
        chara->dir = false;
        _Character_update_position(self, -5, 0);
    }
    else if (key_state[ALLEGRO_KEY_D])
    {
        chara->dir = true;
        _Character_update_position(self, 5, 0);
    }

    // --------------------
    // 狀態顯示用途（動畫）
    // --------------------
    if (chara->y < chara->default_ground_y) {
        chara->state = JUMP;
    }
    // 然后才看左右键
    else if (key_state[ALLEGRO_KEY_A] || key_state[ALLEGRO_KEY_D]) {
        chara->state = MOVE;
    }
    else {
        chara->state = STOP;
    }
    chara->prev_space = space_pressed;
}

void Character_draw(Elements *self)
 {
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
       al_draw_scaled_bitmap(
    frame,
    0, 0,
    al_get_bitmap_width(frame),
    al_get_bitmap_height(frame),
    chara->x,
    chara->y,
    chara->width * 0.5,
    chara->height *0.5,
    (chara->dir ? ALLEGRO_FLIP_HORIZONTAL : 0)
);}
}
void Character_destory(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    for (int i = 0; i < 3; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Character_interact(Elements *self) {}