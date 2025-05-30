#include "GameWindow.h"
#include "GAME_ASSERT.h"
#include "global.h"
#include "shapes/Shape.h"
// include allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
// include scene and following component
#include "scene/sceneManager.h"
#include <stdbool.h>
#include "scene/gamescene.h"
#include "scene/setting.h"
#include "scene/pause.h"

bool g_paused = false;
ALLEGRO_BITMAP *g_pause_bitmap = NULL;
Game *New_Game()
{
    Game *game = (Game *)malloc(sizeof(Game));
    game->execute = execute;
    game->game_init = game_init;
    game->game_update = game_update;
    game->game_draw = game_draw;
    game->game_destroy = game_destroy;
    game->title = "Final Project 10xxxxxxx";
    game->display = NULL;
    game->game_init(game);
    return game;
}
void execute(Game *self)
{
    // main game loop
    bool run = true;
    while (run)
    {
        // process all events here
        al_wait_for_event(event_queue, &event);
        switch (event.type)
        {
            
        case ALLEGRO_EVENT_TIMER: //若事件是由 計時器（fps） 觸發
        {
            run &= self->game_update(self);
            self->game_draw(self);
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE: // 若玩家點擊關閉視窗 stop game
        {
            run = false;
            break;
        }
        case ALLEGRO_EVENT_KEY_DOWN: //鍵盤按下
        {
            key_state[event.keyboard.keycode] = true;
            break;
        }
        case ALLEGRO_EVENT_KEY_UP: //鍵盤放開
        {
            key_state[event.keyboard.keycode] = false;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_AXES: //滑鼠移動
        {
            mouse.x = event.mouse.x;
            mouse.y = event.mouse.y;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: //滑鼠按下
        {
            mouse_state[event.mouse.button] = true;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP: //滑鼠放開
        {
            mouse_state[event.mouse.button] = false;
            break;
        }
        default:
            break;
        }
    }
}
void game_init(Game *self)
{
    printf("Game Initializing...\n");
    GAME_ASSERT(al_init(), "failed to initialize allegro.");
    // initialize allegro addons
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();   // initialize the font addon
    addon_init &= al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    addon_init &= al_init_image_addon();  // initialize the image addon
    addon_init &= al_init_acodec_addon(); // initialize acodec addon
    addon_init &= al_install_keyboard();  // install keyboard event
    addon_init &= al_install_mouse();     // install mouse event
    addon_init &= al_install_audio();     // install audio event
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");
    // Create display
    self->display = al_create_display(WIDTH, HEIGHT); // 建立遊戲視窗
    GAME_ASSERT(self->display, "failed to create display.");
    // Create first scene
    create_scene(Menu_L); //建立 Menu
    // create event queue
    event_queue = al_create_event_queue();
    GAME_ASSERT(event_queue, "failed to create event queue.");
    // Initialize Allegro settings 設定視窗屬性
    al_set_window_position(self->display, 0, 0); //將視窗位置設在螢幕左上角
    al_set_window_title(self->display, self->title);
    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(self->display)); // register display event
    al_register_event_source(event_queue, al_get_keyboard_event_source());             // register keyboard event
    al_register_event_source(event_queue, al_get_mouse_event_source());                // register mouse event
    // register timer event  建立與啟動計時器
    fps = al_create_timer(1.0 / FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(fps));
    al_start_timer(fps);
    // initialize the icon on the display
    ALLEGRO_BITMAP *icon = al_load_bitmap("assets/image/icon.jpg"); //載入視窗圖示圖片並設為視窗左上角小圖示
    al_set_display_icon(self->display, icon);
    al_destroy_bitmap(icon);
    self->menu_background = al_load_bitmap("assets/image/Menu.png");
    GAME_ASSERT(self->menu_background,
                "failed to load assets/image/Menu.jpg");
    g_pause_bitmap = al_load_bitmap("assets/image/pause.png");
    GAME_ASSERT(g_pause_bitmap, "failed to load pause.png");
}
bool game_update(Game *self)
{
    scene->Update(scene);
    if (scene->scene_end)
    {
        scene->Destroy(scene);
        switch (window)
        {
        case 0:
            create_scene(Menu_L); //切回主選單
            break;
        case 1:
            create_scene(GameScene_L); //進入遊戲場景
            break;
        case 2:
            create_scene(Setting_L); //進入設定場景
            break;
        case 3:
            create_scene(Pause_L); //進入設定場景
            break;
        case -1:
            return false; //結束遊戲
        default:
            break;
        }
    }
    return true;
}
void game_draw(Game *self)
{
    // 1. 清屏
    al_clear_to_color(al_map_rgb(100, 100, 100));

    // 2. 如果是主选单，拉满菜单背景并画按钮
    if (window == Menu_L) {
        ALLEGRO_BITMAP *bg = self->menu_background;
        int bw = al_get_bitmap_width(bg);
        int bh = al_get_bitmap_height(bg);
        al_draw_scaled_bitmap(
            bg, 0, 0, bw, bh,
            0, 0, WIDTH, HEIGHT,
            0
        );
        // 接着调用 menu scene 自己的 Draw，画 START/SETTING
        scene->Draw(scene);
    }
    else if (window == GameScene_L) {
        GameScene *gs = (GameScene*)scene->pDerivedObj;
        ALLEGRO_BITMAP *bg = gs->background;
        int bw = al_get_bitmap_width(bg);
        int bh = al_get_bitmap_height(bg);
        al_draw_scaled_bitmap(
            bg, 0, 0, bw, bh,
            0, 0, WIDTH, HEIGHT,
            0
        );
        // 再画场景里的地面、树、角色、陷阱……所有元素
        scene->Draw(scene);
    }
    else if (window == Setting_L) {
        Setting *ss = (Setting*)scene->pDerivedObj;
        al_draw_bitmap(ss->background, 0, 0, 0);
        scene->Draw(scene); // 呼叫 setting_draw
    }
    else if (window == Pause_L) {
        Pause *ss = (Pause*)scene->pDerivedObj;
        al_draw_bitmap(ss->background, 0, 0, 0);
        scene->Draw(scene); // 呼叫 pause_draw
}

    // 4. 翻面
    al_flip_display();
}
   
void game_destroy(Game *self)
{
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(self->display);
    scene->Destroy(scene);
     if (self->menu_background) {
        al_destroy_bitmap(self->menu_background);
    }
     if (g_pause_bitmap) al_destroy_bitmap(g_pause_bitmap);
    free(self);
}
