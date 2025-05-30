#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "menu.h"
#include <stdbool.h>
#include "sceneManager.h"
#define START_X 650
#define START_Y 700
#define START_W 550
#define START_H 150
#define SETTING_X 650
#define SETTING_Y 900
#define SETTING_W 650
#define SETTING_H 150
/*
   [Menu function]
*/
Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene *self)
{
    // 檢查滑鼠點擊
    if (mouse_state[1]) { // 1 代表左鍵
        // 檢查是否點擊在 START 按鈕上
        if (mouse.x >= START_X && mouse.x <= START_X + START_W &&
            mouse.y >= START_Y && mouse.y <= START_Y + START_H) {
            self->scene_end = true;
            window = 1;
        }
        // 檢查是否點擊在 SETTING 按鈕上
        else if(mouse.x >= SETTING_X && mouse.x <= SETTING_X + SETTING_W &&
            mouse.y >= SETTING_Y && mouse.y <= SETTING_Y + SETTING_H) {
            self->scene_end = true;
            window = 2;
        }
        mouse_state[1] = false; 
    }
    return;
}
void menu_draw(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_play_sample_instance(Obj->sample_instance); // 播音樂
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
