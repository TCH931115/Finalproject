#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include "gamescene.h"
#include "sceneManager.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/Ball.h"
#include "../element/platform.h"
#include <allegro5/allegro_image.h>
#define CLOUD1_X  200
#define CLOUD1_Y  300
#define CLOUD1_W  128
#define CLOUD1_H  64
#define CLOUD2_X  100  
#define CLOUD2_Y  600  
#define CLOUD2_W  128   
#define CLOUD2_H   64 
#define CLOUD3_X  100  
#define CLOUD3_Y  320 
#define CLOUD3_W  128   
#define CLOUD3_H   64   
#define TREEHOUSE_X  100  
#define TREEHOUSE_Y  600 
#define TREEHOUSE_W  128   
#define TREEHOUSE_H   64 
#define TREEHOLE_X  100  
#define TREEHOLE_Y  380
#define TREEHOLE_W  128   
#define TREEHOLE_H   64 
#define STAIRS_X  100  
#define STAIRS_Y  380
#define STAIRS_W  128  
#define STAIRS_H   64 

/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);

    // 載入背景圖
    pDerivedObj->background_count = 6; // 假設有 2 張背景圖
    pDerivedObj->backgrounds[0] = al_load_bitmap("assets/image/stage1.png");
    assert(pDerivedObj->backgrounds[0] && "Failed to load stage1.png!");
    pDerivedObj->backgrounds[1] = al_load_bitmap("assets/image/stage2.png");
    assert(pDerivedObj->backgrounds[1] && "Failed to load stage2.png!");
    pDerivedObj->backgrounds[2] = al_load_bitmap("assets/image/stage3.png");
    assert(pDerivedObj->backgrounds[2] && "Failed to load stage3.png!");
    pDerivedObj->backgrounds[3] = al_load_bitmap("assets/image/stage4.png");
    assert(pDerivedObj->backgrounds[3] && "Failed to load stage4.png!");
    pDerivedObj->backgrounds[4] = al_load_bitmap("assets/image/stage5.png");
    assert(pDerivedObj->backgrounds[4] && "Failed to load stage5.png!");
    pDerivedObj->backgrounds[5] = al_load_bitmap("assets/image/stage6.png");
    assert(pDerivedObj->backgrounds[5] && "Failed to load stage6.png!");

    // 读取
    if (g_has_saved_player) {
        pDerivedObj->current_background = g_saved_background;
    } else {
        pDerivedObj->current_background = 0;
    }
    pDerivedObj->background = pDerivedObj->backgrounds[pDerivedObj->current_background];

    //計時
    pDerivedObj->timer_font = al_load_ttf_font("assets/font/pirulen.ttf", 36, 0);

    // 註冊角色和其他元素
    //_Register_elements(pObj, New_Floor(Floor_L));
    //_Register_elements(pObj, New_Teleport(Teleport_L));
    //_Register_elements(pObj, New_Tree(Tree_L));
    //_Register_elements(pObj, New_Ball(Ball_L));
    _Register_elements(pObj,New_Platform(Platform_L, CLOUD1_X, CLOUD1_Y, CLOUD1_W, CLOUD1_H));
    _Register_elements(pObj,New_Platform(Platform_L, CLOUD2_X, CLOUD2_Y, CLOUD2_W, CLOUD2_H));
    _Register_elements(pObj,New_Platform(Platform_L, CLOUD3_X, CLOUD3_Y, CLOUD3_W, CLOUD3_H)); 
    _Register_elements(pObj,New_Platform(Platform_L, TREEHOUSE_X, TREEHOUSE_Y, TREEHOUSE_W, TREEHOUSE_H));
    _Register_elements(pObj,New_Platform(Platform_L, TREEHOLE_X, TREEHOLE_Y, TREEHOLE_W, TREEHOLE_H));
    _Register_elements(pObj,New_Platform(Platform_L, STAIRS_X, STAIRS_Y, STAIRS_W, STAIRS_H));
    // 2) 生成角色对象，但暂时不要立即注册
    Elements *charEle = New_Character(Character_L);
    Character *ch = (Character *)charEle->pDerivedObj;

    // 如果之前保存过，就用保存值覆盖
    if (g_has_saved_player) {
        ch->x               = g_saved_x;
        ch->y               = g_saved_y;
        ch->jump_count      = g_saved_jump_count;
        ch->jump_velocity   = g_saved_jump_velocity;
        ch->prev_space      = g_saved_prev_space;
        // 同步地面：以当前位置为平台顶
        ch->ground_y        = ch->y;
        ch->default_ground_y= ch->y;
        g_has_saved_player  = false; // 清标记，下一次新开游戏就不会继续恢复
    } else {
    // 第一次进入：放到起点云顶
        ch->x = CLOUD1_X + (CLOUD1_W - ch->width) / 2;
        ch->y = CLOUD1_Y - ch->height;
        ch->ground_y        = ch->y;
        ch->default_ground_y= ch->y;
    }
    _Register_elements(pObj, charEle);

    // 設置函數
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}
void game_scene_update(Scene *self)
{
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    ElementVec allEle = _Get_all_elements(self);

    // 檢查是否按下 ESC 鍵
    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        self->scene_end = true;
        window = Pause_L;
        return;
    }

    // 更新所有元素
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele);
    }
    
// 檢查角色位置並切換背景（有界，不循環）
for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    if (ele->label == Character_L) {
        Character *ch = (Character *)ele->pDerivedObj;

        // → 走到最右邊，且不是最後一關，才能前進
        if (ch->x >= WIDTH - 50 && gs->current_background < gs->background_count - 1) {
            gs->current_background++;
            gs->background = gs->backgrounds[gs->current_background];
            // 根據 new background 放到對應的雲頂
            if (gs->current_background == 1) {
                ch->x = CLOUD2_X + (CLOUD2_W - ch->width) / 2;
                ch->y = CLOUD2_Y - ch->height;
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 2) {
                ch->x = CLOUD3_X + (CLOUD3_W - ch->width) / 2;
                ch->y = CLOUD3_Y - ch->height;
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 3) {
                ch->x = TREEHOUSE_X + (TREEHOUSE_W - ch->width) / 2;
                ch->y = TREEHOUSE_Y - ch->height;
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 4) {
                ch->x = TREEHOLE_X + (TREEHOLE_W - ch->width) / 2;
                ch->y = TREEHOLE_Y - ch->height;
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 5) {
                ch->x = STAIRS_X + (STAIRS_W - ch->width) / 2;
                ch->y = STAIRS_Y - ch->height;
                Character_ResetOnPlatform(ch);
            }
        }
        // ← 走到最左邊，且不是第一關，才能後退
        else if (ch->x <= 50 && gs->current_background > 0) {
            gs->current_background--;
            gs->background = gs->backgrounds[gs->current_background];
            if (gs->current_background == 0) {
            // 回到第1关：放到右下角（距离右边50px，地面上）
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 1) {
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 2) {
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
                if (gs->current_background == 3) {
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
                if (gs->current_background == 4) {
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
            if (gs->current_background == 5) {
                ch->x = WIDTH - 50;
                ch->y = HEIGHT - ch->height - 60;  // 跟 New_Character 初始化地面一致
                Character_ResetOnPlatform(ch);
            }
        }
    }
}


    // 執行交互
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }

    // 移除標記為刪除的元素
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
}
void game_scene_draw(Scene *self)
{
    // 1. 清屏
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    
    // 2. 绘制缩放后的背景（填满整个窗口）
    if (gs->background) {
        int bg_width = al_get_bitmap_width(gs->background);
        int bg_height = al_get_bitmap_height(gs->background);
        
        al_draw_scaled_bitmap(
            gs->background,   // 源位图
            0, 0,             // 源位图起始坐标
            bg_width,         // 源位图宽度
            bg_height,        // 源位图高度
            0, 0,             // 目标起始坐标
            WIDTH,            // 目标宽度（使用全局定义的窗口宽度）
            HEIGHT,           // 目标高度（使用全局定义的窗口高度）
            0                 // 绘制标志（0表示无特殊效果）
        );
    }
    
    // 3. 绘制所有游戏元素
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++) {
        Elements *ele = allEle.arr[i];
        if (ele && ele->Draw) {  // 安全检查
            ele->Draw(ele);
        }
    }
    
    // 4. 更新显示（如果在主循环中已调用，这里可以省略）
    // al_flip_display();

    // 加在繪製完背景與元素之後
    double current_time = al_get_time();
    double elapsed_time = current_time - gs->start_time;

    // 顯示時間（確保你已經初始化字型系統並載入字型）
 
    if (gs->timer_font) {
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "Time: %.1f s", elapsed_time);

    int text_width = al_get_text_width(gs->timer_font, time_str);
    int text_height = al_get_font_line_height(gs->timer_font);

    int x = (WIDTH - text_width) / 2;  // 水平置中
    int y = text_height / 2;            // 靠近畫面上方，但不貼邊，留點空間

    al_draw_text(gs->timer_font, al_map_rgb(255, 0, 0), x, y, 0, time_str);
}
}

void game_scene_destroy(Scene *self)
{
    // 在确定要切出游戏场景前：
    Elements *ele;
    ElementVec all = _Get_label_elements(scene, Character_L);
    if (all.len > 0) {
        ele = all.arr[0];
        Character *ch = (Character *)ele->pDerivedObj;
        g_saved_x             = ch->x;
        g_saved_y             = ch->y;
        g_saved_jump_count    = ch->jump_count;
        g_saved_jump_velocity = ch->jump_velocity;
        g_saved_prev_space    = ch->prev_space;
        g_has_saved_player    = true;
    }
    GameScene *gs = (GameScene *)self->pDerivedObj;
    g_saved_background = gs->current_background;
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    
    // 銷毀所有背景圖
    for (int i = 0; i < Obj->background_count; i++)
    {
        if (Obj->backgrounds[i])
            al_destroy_bitmap(Obj->backgrounds[i]);
    }

    // 銷毀所有元素
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }

    free(Obj);
    free(self);

    if (Obj->timer_font)
    al_destroy_font(Obj->timer_font);
}

void Character_ResetOnPlatform(Character *ch) {
    ch->ground_y         = ch->y;
    ch->default_ground_y = ch->y;
    ch->jump_velocity    = 0;
    ch->prev_space       = false;
}
