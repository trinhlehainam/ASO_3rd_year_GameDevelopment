#pragma once
#include "IScene.h"

class TitleScene :
    public IScene
{
public:
    TitleScene();
    ~TitleScene();

    void Update(float deltaTime_ms) override;
    void Render() override;
};

