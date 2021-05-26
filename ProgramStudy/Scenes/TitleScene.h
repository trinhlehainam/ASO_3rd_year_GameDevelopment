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

private:
    int m_hBgImage;
};

