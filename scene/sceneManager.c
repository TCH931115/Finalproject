#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "setting.h"
#include "pause.h"
Scene *scene = NULL;
bool   g_has_saved_player      = false;
float  g_saved_x               = 0;
float  g_saved_y               = 0;
int    g_saved_jump_count      = 0;
float  g_saved_jump_velocity   = 0;
bool   g_saved_prev_space      = false;
void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
        break;
    case Setting_L:
        scene = New_Setting(Setting_L);
        break;
    case Pause_L:
        scene = New_Pause(Pause_L);
        break;
    default:
        break;
    }
}
