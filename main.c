#define ALLEGRO_NO_MAGIC_MAIN
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include "GameWindow.h"

int real_main(int argc, char **argv)
{
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro!\n");
        return -1;
    }

    al_init_ttf_addon();  // 初始化字型模組，沒有回傳值

    Game *game = New_Game();
    game->execute(game);
    game->game_destroy(game);

    // 字型模組不用額外銷毀

    return 0;
}

int main(int argc, char **argv)
{
    return al_run_main(argc, argv, real_main);
}
