#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [Pause object]
*/
typedef struct _Pause
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_BITMAP *background;
    int title_x, title_y;
    float volume; // 音量（0.0 到 1.0）
    // 滑桿屬性
    float slider_x; // 滑桿起始 x 座標
    float slider_y; // 滑桿 y 座標
    float slider_width; // 滑桿寬度
    float slider_pos; // 滑塊當前位置（0.0 到 1.0，與音量同步）
    bool slider_dragging; // 是否正在拖動滑塊
} Pause;
Scene *New_Pause(int label);
void pause_update(Scene *self);
void pause_draw(Scene *self);
void pause_destroy(Scene *self);

#endif