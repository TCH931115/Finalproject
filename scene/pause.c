#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "pause.h"
#include <stdbool.h>
#include "sceneManager.h"
#include <assert.h>
#include "../GAME_ASSERT.h"
#define CONTINUE_X 250
#define CONTINUE_Y 300
#define CONTINUE_W 1100
#define CONTINUE_H 150

/*
   [Pause function]
*/
Scene *New_Pause(int label)
{
    Pause *pDerivedObj = (Pause *)malloc(sizeof(Pause));
    Scene *pObj = New_Scene(label);
    pDerivedObj->background = al_load_bitmap("assets/image/pause.png");
    assert(pDerivedObj->background && "Failed to load pause background!");
    
    // 2) 音樂
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    GAME_ASSERT(pDerivedObj->song, "Failed to load menu.mp3");
    al_reserve_samples(4);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());

    // 3) 字體
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 48, 0);
    GAME_ASSERT(pDerivedObj->font, "Failed to load font");
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    pDerivedObj->volume = 0.1; // 初始音量
    al_set_sample_instance_gain(pDerivedObj->sample_instance, pDerivedObj->volume);

    // 初始化滑桿
    pDerivedObj->slider_x = WIDTH / 2 - 400; // 滑桿左端點 x 座標
    pDerivedObj->slider_y = HEIGHT / 2 +20 ; // 滑桿 y 座標（在 CONTINUE 按鈕下方）
    pDerivedObj->slider_width = 800; // 滑桿寬度
    pDerivedObj->slider_pos = pDerivedObj->volume; // 滑塊位置與音量同步
    pDerivedObj->slider_dragging = false; // 初始未拖動

    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = pause_update;
    pObj->Draw = pause_draw;
    pObj->Destroy = pause_destroy;
    return pObj;
}

void pause_update(Scene *self)
{
    Pause *Obj = ((Pause *)(self->pDerivedObj));
    ALLEGRO_MOUSE_STATE mouse_state;

    // 獲取滑鼠狀態
    al_get_mouse_state(&mouse_state);

    // 滑桿範圍
    float slider_left = Obj->slider_x;
    float slider_right = Obj->slider_x + Obj->slider_width;
    float slider_top = Obj->slider_y - 10; // 滑塊半徑範圍
    float slider_bottom = Obj->slider_y + 10;

    // 檢查滑鼠是否點擊滑桿
    if (mouse_state.buttons & 1) { // 左鍵按下
        if (mouse_state.x >= slider_left && mouse_state.x <= slider_right &&
            mouse_state.y >= slider_top && mouse_state.y <= slider_bottom) {
            Obj->slider_dragging = true;
        }
        // CONTINUE 按鈕區域
        if (mouse_state.x >= CONTINUE_X && mouse_state.x <= CONTINUE_X + CONTINUE_W &&
            mouse_state.y >= CONTINUE_Y && mouse_state.y <= CONTINUE_Y + CONTINUE_H) {
            self->scene_end = true;
            window = 1;
        }
    } else {
        Obj->slider_dragging = false; // 滑鼠釋放時停止拖動
    }

    // 如果正在拖動，更新滑塊位置和音量
    if (Obj->slider_dragging) {
        // 計算滑塊位置（0.0 到 1.0）
        Obj->slider_pos = (mouse_state.x - Obj->slider_x) / Obj->slider_width;
        if (Obj->slider_pos < 0.0) Obj->slider_pos = 0.0;
        if (Obj->slider_pos > 1.0) Obj->slider_pos = 1.0;

        // 更新音量
        Obj->volume = Obj->slider_pos;
        al_set_sample_instance_gain(Obj->sample_instance, Obj->volume);
    }
}

void pause_draw(Scene *self)
{
    Pause *Obj = ((Pause *)(self->pDerivedObj));

    // 播放音效
    if (!al_get_sample_instance_playing(Obj->sample_instance)) {
        al_play_sample_instance(Obj->sample_instance);
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    int bg_width = al_get_bitmap_width(Obj->background);
    int bg_height = al_get_bitmap_height(Obj->background);
    al_draw_scaled_bitmap(
        Obj->background, 0, 0, bg_width, bg_height,
        0, 0, WIDTH, HEIGHT, 0
    );

    // 繪製滑桿背景（藍色線）
    al_draw_line(Obj->slider_x, Obj->slider_y, 
                 Obj->slider_x + Obj->slider_width, Obj->slider_y, 
                 al_map_rgb(0, 120, 255), 8); // 藍色，線寬 4

    // 繪製滑塊（白色圓形）
    float slider_pos_x = Obj->slider_x + (Obj->slider_pos * Obj->slider_width);
    al_draw_filled_circle(slider_pos_x, Obj->slider_y, 20, al_map_rgb(255, 255, 255));
}

void pause_destroy(Scene *self)
{
    Pause *Obj = ((Pause *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    al_destroy_bitmap(Obj->background);
    free(Obj);
    free(self);
}