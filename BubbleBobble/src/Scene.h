#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "ShotManager.h"
#include "Text.h"

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

private:
  
    void CheckObjectCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    bool playerAlive;
    
    //Level structure that contains all the static tile
    TileMap *level;
    int stage;

    //Dynamic objects of the level: items and collectables
    std::vector<Object*> objects;

    //Enemies present in the level
    EnemyManager* enemies;

    //Shots thrown by enemies
    ShotManager* shots;
    ParticleManager* particles;

    Camera2D camera;
    DebugMode debug;

    //Temporary for killing the scene
    SceneState state;
    bool alive;

    //Temporary for transition
    const Texture2D* stage1, * stage2;

    float timeSpent;
    float totalTime;

    Text* font;
};

