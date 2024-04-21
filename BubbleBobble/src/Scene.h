#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Enemy.h"

//LAST STAGE
#define MAX_STAGE   2

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };
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

    bool IsPlayerAlive();
    std::vector<Enemy*> enemies;

private:
  
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void RenderEnemies();
    void RenderEnemiesDebug(const Color& col) const;
    void RenderGUI() const;

    Player *player;
    bool playerAlive;
    
    TileMap *level;
    int stage;

    std::vector<Object*> objects;
   
    std::vector<Bubble*> bubbleEnemies;

    Camera2D camera;
    DebugMode debug;

    const Texture2D* stage1, * stage2;

    float timeSpent;
    float totalTime;
    SceneState state;
};

