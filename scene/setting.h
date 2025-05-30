#ifndef SETTING_H
#define SETTING_H

#include "sceneManager.h" // 確保包含 Scene 的定義

typedef struct _Setting {
    ALLEGRO_BITMAP *background;
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
    float volume; // 音量（0.0 到 1.0）
    // 滑桿屬性
    float slider_x; // 滑桿起始 x 座標
    float slider_y; // 滑桿 y 座標
    float slider_width; // 滑桿寬度
    float slider_pos; // 滑塊當前位置（0.0 到 1.0，與音量同步）
    bool slider_dragging; // 是否正在拖動滑塊
} Setting;

Scene *New_Setting(int label);
void setting_update(Scene *self);
void setting_draw(Scene *self);
void setting_destroy(Scene *self);

#endif