#pragma once
#include "IScene.h"

class TitleScene :
    public IScene
{
public:
    TitleScene();
    ~TitleScene();
private:

    bool Init() override;
    void Update(float deltaTime_ms) override;
    void Render() override;
    std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene> scene) override;

private:
    int m_hBgImage;
};

