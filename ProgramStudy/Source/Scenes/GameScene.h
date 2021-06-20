#pragma once
#include "IScene.h"

#include <memory>

class TileMap;
class Player;

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
    std::unique_ptr<Player> m_player;
    std::unique_ptr<TileMap> m_map;
};

