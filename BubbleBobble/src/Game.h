#pragma once
#include "Globals.h"
#include "Scene.h"
#include "Enemy.h"

//Intro Timing
#define INTRO_TIME          10.0
#define INTRO_FIRST_IMAGE   INTRO_TIME * 0.1
#define INTRO_SECOND_IMAGE   INTRO_TIME * 0.5
#define INTRO_SECOND_FADE   INTRO_TIME * 0.6
#define CINEMATIC_TIME  3.0




enum class GameState { INTRO, MAIN_MENU, CINEMATIC, PLAYING, SETTINGS, CREDITS, GAME_OVER, TRANSITION };
enum  MusicTrack {
    GAME_MUS,
    HURRY_MUS,
    BONUS_MUS,
    SECRET_ROOM_MUS,
    FALSE_ENDING_MUS,
    SUPER_DRUNK_MUS,
    REAL_ENDING_MUS,
    NAME_REGISTER_MUS,
    GAME_OVER_MUS,  
};

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update(float scale);
    void Render(float scale);
    void Cleanup();
    void CinematicBubBob();
    void renderLives();
    
    Music GetCurrentTrack() const;
    void ChangeTrack(MusicTrack track);

    Music tracks[10];
    Music currentTrack;

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    //Fullscreen
    void Fullscreen(int windowWidth, int windowHeight);
    int GetDisplayWidth();
    int GetDisplayHeight();

    GameState state;
    Scene *scene;
    const Texture2D* img_menu, * img_intro1, * img_intro2, * game_over, * transition, * stage2, * stage1, *cinematic_complete,*lives;
    float introTimer;;
    float cinematicTimer;

    //Temporary (for transition)
    float totalTime = 1.5f;
    float timeSpent = 0.0;

    Entity* CinematicBub;
    Entity* CinematicBob;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};