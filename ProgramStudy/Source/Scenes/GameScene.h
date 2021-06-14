#pragma once
#include "IScene.h"

#include <memory>

class TileMap;
class Entity;

class GameScene :
    public IScene
{
public:
    GameScene();
    ~GameScene();

private:

    bool Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;
    void RenderToOwnScreen() override;
    std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene>) override;
    SCENE_ID GetSceneID() override;

private:
    std::unique_ptr<Entity> m_entity;
    std::unique_ptr<TileMap> m_map;
};

