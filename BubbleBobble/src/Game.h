#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { INTRO, MAIN_MENU, PLAYING, SETTINGS, CREDITS, GAME_OVER };
enum  MusicTrack {
    GAME_MUS,
    HURRY_MUS,
    BONUS_MUS,
    SECRET_ROOM_MUS,
    FALSE_ENDING_MUS,
    SUPER_DRUNK_MUS,
    REAL_ENDING_MUS,
    NAME_REGISTER_MUS,
    GAME_OVER_MUS
};
class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    
    Music GetCurrentTrack() const;
    void ChangeTrack(MusicTrack track);

    Music tracks[10];
    Music currentTrack;
    Music trackTimer;

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu, *img_intro1, *img_intro2, *game_over;
    double startTime;



    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};