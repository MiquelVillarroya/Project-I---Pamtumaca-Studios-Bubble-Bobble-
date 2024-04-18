#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>


Sound sounds[10];

Game::Game()
{
    state = GameState::INTRO;
    scene = nullptr;
    img_menu = nullptr;
    img_intro1 = nullptr;
    img_intro2 = nullptr;
    startTime = 0;

    tracks[GAME_MUS] = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");
    tracks[GAME_MUS] = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");
    tracks[HURRY_MUS] = LoadMusicStream("audio/music/2_Hurry!_Main Theme__faster.ogg");
    tracks[BONUS_MUS] = LoadMusicStream("audio/music/3_Extend_Bonus_Music.ogg");
    tracks[SECRET_ROOM_MUS] = LoadMusicStream("audio/music/4_Secret_Room.ogg");
    tracks[FALSE_ENDING_MUS] = LoadMusicStream("audio/music/5_False_Ending.ogg");
    tracks[SUPER_DRUNK_MUS] = LoadMusicStream("audio/music/6_Super_Drunk.ogg");
    tracks[REAL_ENDING_MUS] = LoadMusicStream("audio/music/7_Real_Ending.ogg");
    tracks[NAME_REGISTER_MUS] = LoadMusicStream("audio/music/8_Name_Register.ogg");
    tracks[GAME_OVER] = LoadMusicStream("audio/music/9_Game_Over.ogg");
    currentTrack = LoadMusicStream("audio/music/1_Introduction_Main_Theme.ogg");

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }

}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vikings");

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
    startTime = GetTime();

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
  
    tracks[GAME_MUS].looping = true;

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{    
    scene = new Scene();
    currentTrack = tracks[GAME_MUS];
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
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::INTRO:
            {
            double time = GetTime() - startTime;
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) state = GameState::MAIN_MENU;
            if (time > 9) state = GameState::MAIN_MENU;
            break;
            }
        case GameState::MAIN_MENU: 
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
            }
            else
            {
                //Game logic
                scene->Update();
            }
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
         case GameState::INTRO:
            {
            double time = GetTime();
            if (time > 1 && time <= 2) {
                DrawTexture(*img_intro1, 0, 0, { 255, 255, 255, 255 });
            }
            else if (time > 2 && time < 5) {
                unsigned char trans = 410 - 82 * time;
                DrawTexture(*img_intro1, 0, 0, { 255, 255, 255, trans });
            }
            else if (time >= 5 && time < 6) {
                DrawTexture(*img_intro2, 0, 0, { 255, 255, 255, 255 });
            }
            else if (time > 6 && time <= 9) {
                unsigned char trans = 745 - 82 * time;
                DrawTexture(*img_intro2, 0, 0, {255, 255, 255, trans});
            }
            break;
            }
        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            if (GetMusicTimePlayed(currentTrack) >= 99) SeekMusicStream(currentTrack, 45);
            printf("%f \n",GetMusicTimePlayed(currentTrack));
            UpdateMusicStream(currentTrack);
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    for (int i = 0; i < GAME_OVER; ++i) {
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

    UnloadRenderTexture(target);
}
Music Game::GetCurrentTrack() const {
    return currentTrack;
}
void Game::ChangeTrack(MusicTrack track) {
    currentTrack = tracks[track];
}