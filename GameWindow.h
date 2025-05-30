#ifndef GAMEWINDOW_H_INCLUDED
#define GAMEWINDOW_H_INCLUDED
#include <allegro5/allegro.h>
#include <stdbool.h>

typedef struct _GAME Game;
typedef void (*fptrGameExecute)(Game *); //遊戲的主執行流程
typedef void (*fptrGameInit)(Game *); //初始化遊戲
typedef bool (*fptrGameUpdate)(Game *); //更新遊戲狀態（回傳布林值代表遊戲是否繼續）
typedef void (*fptrGameDraw)(Game *); //畫面繪製
typedef void (*fptrGameDestroy)(Game *); //資源釋放
void execute(Game *);
void game_init(Game *);
bool game_update(Game *);
void game_draw(Game *);
void game_destroy(Game *);
extern bool g_paused;
extern ALLEGRO_BITMAP *g_pause_bitmap;
struct _GAME
{
    const char *title;                      // 遊戲標題
    //ALLEGRO Variables
    ALLEGRO_DISPLAY *display;              // 遊戲視窗
    ALLEGRO_BITMAP   *menu_background;     // 遊戲初始畫面
    fptrGameExecute execute;               // 執行函式
    fptrGameInit game_init;                // 初始化函式
    fptrGameUpdate game_update;            // 更新函式
    fptrGameDraw game_draw;                // 繪圖函式
    fptrGameDestroy game_destroy;          // 清理函式
};
Game *New_Game();

#endif
