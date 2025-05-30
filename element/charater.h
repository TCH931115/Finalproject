#ifndef CHARATER_H_INCLUDED
#define CHARATER_H_INCLUDED
#include <allegro5/allegro_audio.h> //播放攻擊音效
#include "element.h"
#include "../shapes/Shape.h" //角色碰撞用的 hitbox
#include "../algif5/algif.h"
#include <stdbool.h>

/*
   [character object]
*/
typedef enum CharacterType
{
    STOP = 0, //停止不動
    MOVE, //移動
    JUMP, //跳躍
} CharacterType;
typedef struct _Character
{
    int x, y;                       // 位置座標
    int width, height;              // 角色圖片的寬高
    bool dir;                       // true 為右, false 為左
    int state;                      // the state of character
    ALGIF_ANIMATION *gif_status[3]; // [0]：停止動畫, [1]：移動動畫, [2]：攻擊動畫
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound; //攻擊時播放的音效
    int anime;      // 計數動畫時間
    int anime_time; // 動畫總時長
    bool new_proj; // 是否要發射投射物（如子彈、火球）
    int ground_y;         // 記住地面位置（避免無限掉落）
    int jump_velocity;
    int jump_count;    // 控制跳躍速度
    bool prev_space;
    float default_ground_y;
    Shape *hitbox; // the hitbox of object
} Character;
Elements *New_Character(int label);
void Character_update(Elements *self);
void Character_interact(Elements *self);
void Character_draw(Elements *self);
void Character_destory(Elements *self);
void _Character_update_position(Elements *self, int dx, int dy);

#endif