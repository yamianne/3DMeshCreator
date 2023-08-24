#pragma once
#include "Mesh.h"

class Point : public Mesh
{
public:
	Point(ID3D11Device* device, std::string name, Vector3 position);
	Point(ID3D11Device* device, std::string name, Vector3 position, Vector4 color);

	Point(ID3D11Device* device, Vector3 position) : Point(device, "Point" + std::to_string(m_creationIdx++), position) {};
	Point(ID3D11Device* device, Vector3 position, Vector4 color) : Point(device, "Point" + std::to_string(m_creationIdx++), position, color) {};
};

