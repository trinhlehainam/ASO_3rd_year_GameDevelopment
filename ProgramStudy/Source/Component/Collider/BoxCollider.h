#pragma once
#include "ICollider.h"
#include "../../Math/Geometry/AABB.h"

class BoxCollider :
    public ICollider
{
public:
    explicit BoxCollider(const std::shared_ptr<Entity>& owner);
    explicit BoxCollider(const std::shared_ptr<Entity>& owner, const AABBf& boxInfo);
    ~BoxCollider();

    void SetPos(const vec2f& pos);
    void SetSize(const vec2f& size);

    vec2f GetPos() const;
    vec2f GetSize() const;
    AABBf GetCollider() const;
public:
    void Init() override final;
    void Update(float deltaTime_s) override final;
    void Render() override final;
    COLLIDER_TYPE ColliderType() override final;
private:
    AABBf m_collider;
};

