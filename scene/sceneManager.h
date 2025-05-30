#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "scene.h"
extern Scene *scene;
extern bool   g_has_saved_player;
extern float  g_saved_x, g_saved_y;
extern int    g_saved_jump_count;
extern float  g_saved_jump_velocity;
extern bool   g_saved_prev_space;
typedef enum SceneType
{
    Menu_L = 0,
    GameScene_L,
    Setting_L,
    Pause_L
} SceneType;
void create_scene(SceneType);

#endif