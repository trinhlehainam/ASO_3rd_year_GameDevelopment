#pragma once
#include <memory>
#include <vector>

class IShape;

class GeometryManager
{
public:
	GeometryManager(float screenWidth,float screenHeight);
	~GeometryManager();

	void AddShape(std::unique_ptr<IShape> shape);

	void Update(float deltaTime_ms);
	void Render();

private:
	// Don't allow to use copy or move
	GeometryManager(const GeometryManager&);
	GeometryManager(GeometryManager&&) noexcept;
	void operator = (const GeometryManager&);
	void operator = (GeometryManager&&) noexcept;
	//
private:
	float m_screenWidth;
	float m_screenHeight;

	std::vector<std::unique_ptr<IShape>> m_shapes;
	bool m_activeDelete;
	bool m_activeAdd;
};

