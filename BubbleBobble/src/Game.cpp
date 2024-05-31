#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>
#include "Player.h"
#include "Scene.h"
#include "Entity.h"

Sound sounds[10];

Game::Game()
{
    state = GameState::INTRO;
    scene = nullptr;
    img_menu = nullptr;
    img_intro1 = nullptr;
    img_intro2 = nullptr;
    game_over = nullptr;

    introTimer = 0;

    //Temp?
    transition = nullptr;
    stage1 = nullptr;
    stage2 = nullptr;

    tracks[GAME_MUS] = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");
    tracks[GAME_MUS] = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");
    tracks[HURRY_MUS] = LoadMusicStream("audio/music/2_Hurry!_Main Theme__faster.ogg");
    tracks[BONUS_MUS] = LoadMusicStream("audio/music/3_Extend_Bonus_Music.ogg");
    tracks[SECRET_ROOM_MUS] = LoadMusicStream("audio/music/4_Secret_Room.ogg");
    tracks[FALSE_ENDING_MUS] = LoadMusicStream("audio/music/5_False_Ending.ogg");
    tracks[SUPER_DRUNK_MUS] = LoadMusicStream("audio/music/6_Super_Drunk.ogg");
    tracks[REAL_ENDING_MUS] = LoadMusicStream("audio/music/7_Real_Ending.ogg");
    tracks[NAME_REGISTER_MUS] = LoadMusicStream("audio/music/8_Name_Register.ogg");
    tracks[GAME_OVER_MUS] = LoadMusicStream("audio/music/9_Game_Over.ogg");
    
    currentTrack = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");

    target = {};
    src = {};
    dst = {};

    CinematicBub = new Entity();
    CinematicBob = new Entity();
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
    UnloadRenderTexture(target);
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow(w, h, "Bubble Bobble");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    if (data.LoadTexture(Resource::IMG_INTRO1, "images/intro_yapping1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro1 = data.GetTexture(Resource::IMG_INTRO1);

    if (data.LoadTexture(Resource::IMG_INTRO2, "images/intro_yapping2.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro2 = data.GetTexture(Resource::IMG_INTRO2);

    if (data.LoadTexture(Resource::GAME_OVER, "images/game_over.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    game_over = data.GetTexture(Resource::GAME_OVER);

    //Those files are temporary (for the transition)
    if (data.LoadTexture(Resource::STAGE1, "images/level1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    stage1 = data.GetTexture(Resource::STAGE1);
    if (data.LoadTexture(Resource::STAGE2, "images/level13.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    stage2 = data.GetTexture(Resource::STAGE2);

    if (data.LoadTexture(Resource::CINEMATIC_BACKGROUND, "images/cinematic_complete.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    cinematic_complete= data.GetTexture(Resource::CINEMATIC_BACKGROUND);

    if (data.LoadTexture(Resource::LIVES, "images/life.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    lives = data.GetTexture(Resource::LIVES);



    CinematicBubBob();



    tracks[GAME_MUS].looping = true;
    tracks[GAME_OVER_MUS].looping = false;

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{    
    scene = new Scene();
    currentTrack = tracks[GAME_MUS];
    SeekMusicStream(currentTrack, 0);
    PlayMusicStream(currentTrack);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}

void Game::CinematicBubBob() {


    Point p(80, 130);
    CinematicBub->Set(p, p, 32, 32, 32, 32);
    CinematicBub->Animations();
    CinematicBub->SetAnimationEntity((int)EntityAnim::BUBCINEMATIC);

  

}

void Game::renderLives() {

        if (scene->player->GetLives() == 3) {

            DrawTexture(*lives, 0, WINDOW_HEIGHT-8, WHITE);
            DrawTexture(*lives, 8, WINDOW_HEIGHT-8, WHITE);
            DrawTexture(*lives, 16, WINDOW_HEIGHT-8, WHITE);

        }
        else if (scene->player->GetLives() == 2) {

            DrawTexture(*lives, 0, WINDOW_HEIGHT - 8, WHITE);
            DrawTexture(*lives, 8, WINDOW_HEIGHT - 8, WHITE);


        }
        else if (scene->player->GetLives() == 1) {

            DrawTexture(*lives, 0, WINDOW_HEIGHT - 8, WHITE);


        }
    


}
AppStatus Game::Update(float scale)
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;


    switch (state)
    {
    case GameState::INTRO:
            introTimer += GetFrameTime();
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (introTimer > INTRO_TIME || IsKeyPressed(KEY_SPACE))
            {
                Sound menusound;
                menusound = LoadSound("audio/FX/Intro/TitleSFX.wav");
                PlaySound(menusound);
                state = GameState::MAIN_MENU;
            }
            break;

        case GameState::MAIN_MENU: 
           
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::CINEMATIC;
                
            }
            break;
        case GameState::CINEMATIC:
            if (IsKeyPressed(KEY_ESCAPE)) {

                state=GameState::MAIN_MENU;
                

            }
            if (IsKeyPressed(KEY_P)) {


                state = GameState::PLAYING;

            }
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE) || !scene->IsPlayerAlive() || !scene->IsSceneAlive())
            {
                FinishPlay();
                ChangeTrack(GAME_OVER_MUS);
                PlayMusicStream(currentTrack);
                state = GameState::GAME_OVER;
            }
            else
            {
                //Game logic
                scene->Update();
            }
            if (IsKeyPressed(KEY_N)) {
                state = GameState::TRANSITION;
            }
            break;

        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                state = GameState::MAIN_MENU;
            }
            break;
        
            
    }
    return AppStatus::OK;
}
void Game::Render(float scale)
{
    //Fullscreen
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;
    if (IsKeyPressed(KEY_F))
    {
        Fullscreen(w, h);
    }

    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
         case GameState::INTRO:
             if (introTimer <= INTRO_FIRST_IMAGE) {
                 DrawTexture(*img_intro1, 0, 0, WHITE);
             }
             else if (introTimer > INTRO_FIRST_IMAGE && introTimer < INTRO_SECOND_IMAGE) {
                 float trans = 1.25f - 0.28f * introTimer;
                 DrawTexture(*img_intro1, 0, 0, Fade (WHITE, trans));
             }
             else if (introTimer >= INTRO_SECOND_IMAGE && introTimer < INTRO_SECOND_FADE) {
                 DrawTexture(*img_intro2, 0, 0, WHITE);
             }
             else if (introTimer > INTRO_SECOND_FADE && introTimer <= INTRO_TIME) {
                 float trans = 2.75f - 0.28f * introTimer;
                 DrawTexture(*img_intro2, 0, 0, Fade (WHITE, trans));
             }
             break;

        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::CINEMATIC:
            DrawTexture(*cinematic_complete, 0, 0, WHITE);
            CinematicBub->DrawAnimation();
            break;

        case GameState::PLAYING:
            scene->Render();
            if (GetMusicTimePlayed(currentTrack) >= 100) SeekMusicStream(currentTrack, 45);
            UpdateMusicStream(currentTrack);
            renderLives();
            break;

        case GameState::GAME_OVER:
            DrawTexture(*game_over, 0, 0, WHITE);  
            UpdateMusicStream(currentTrack);
            break;

        //Temporary transition
        case GameState::TRANSITION:

            UpdateMusicStream(currentTrack);
            float transition = timeSpent / totalTime;
            float stage2_position = 224.0f * - transition;

            if (timeSpent <= totalTime) {


                DrawTexture(*stage1, 0, (int)stage2_position, WHITE);
                DrawTexture(*stage2, 0, (int)(stage2_position+224.0f), WHITE);
                timeSpent += GetFrameTime();
                
            }
            else {
                scene->LoadLevel(2);
                timeSpent = 0;
                state = GameState::PLAYING;
            }
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndScissorMode();
    EndDrawing();
}
void Game::Cleanup()
{
    for (int i = 0; i < GAME_OVER_MUS; ++i) {
        UnloadMusicStream(tracks[i]);
    }
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_INTRO1);
    data.ReleaseTexture(Resource::IMG_INTRO2);
    data.ReleaseTexture(Resource::GAME_OVER);
    UnloadRenderTexture(target);
   

}
Music Game::GetCurrentTrack() const {
    return currentTrack;
}
void Game::ChangeTrack(MusicTrack track) {
    currentTrack = tracks[track];
}
void Game::Fullscreen(int windowWidth, int windowHeight)
{
    if (!IsWindowFullscreen())
    {
        ToggleFullscreen();
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor) + 150, GetMonitorHeight(monitor) + 280);
    }
    else
    {
        ToggleFullscreen();
        SetWindowSize(windowWidth, windowHeight);
    }
}
int Game::GetDisplayWidth()
{
    if (IsWindowFullscreen())
    {
        int monitor = GetCurrentMonitor();
        return GetMonitorWidth(monitor);
    }
    else
    {
        return GetScreenWidth();
    }
}
int Game::GetDisplayHeight()
{
    if (IsWindowFullscreen())
    {
        int monitor = GetCurrentMonitor();
        return GetMonitorHeight(monitor);
    }
    else
    {
        return GetScreenHeight();
    }
}