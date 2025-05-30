#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include <allegro5/allegro_font.h>
#include "scene.h"
#include "../element/charater.h"

/*
   [game scene object]
*/
extern int g_saved_background;
typedef enum EleType{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Projectile_L,
    Ball_L,
    Platform_L
} EleType;
typedef struct _GameScene {
    ALLEGRO_BITMAP *background; // 當前背景圖
    ALLEGRO_BITMAP *backgrounds[10]; // 背景圖陣列（假設有 2 張背景圖）
    ALLEGRO_FONT *timer_font;
    int current_background; // 當前背景圖索引
    int background_count; // 背景圖總數
    double start_time;  // 新增：記錄開始時間
} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);
void Character_ResetOnPlatform(Character *ch);

#endif