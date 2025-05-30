#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"

/*
   [game scene object]
*/
typedef enum EleType{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Projectile_L,
    Platform_L
} EleType;
typedef struct {
    ALLEGRO_BITMAP *backgrounds[6]; // 匹配背景數量
    int background_count;
    ALLEGRO_BITMAP *background;
    int current_background;
} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif