#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "ShotManager.h"
#include "Text.h"
#include "ObjectManager.h"

//LAST STAGE
#define MAX_STAGE   4

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

//Temporary for transition
enum class SceneState {NORMAL, TRANSITION};

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    AppStatus LoadLevel(int stage);

    //Maybe rework?
    bool IsPlayerAlive();
    bool IsSceneAlive();
    Player* player;

private:
  
    void ClearLevel();

    void RenderGUI() const;

    
    bool playerAlive;
    
    //Level structure that contains all the static tile
    TileMap *level;
    int stage;

    //Enemies present in the level
    EnemyManager* enemies;

    //Shots thrown by enemies
    ShotManager* shots;
    ParticleManager* particles;

    //Objects
    ObjectManager* objects;
    ParticleScoreManager* scoreParticles;

    Camera2D camera;
    DebugMode debug;

    //Temporary for killing the scene
    SceneState state;
    bool alive;

    //Temporary for transition
    const Texture2D* stage1, * stage2;

    float timeSpent;
    float totalTime;

    float levelTimer;

    Text* font;

    bool win;
};

