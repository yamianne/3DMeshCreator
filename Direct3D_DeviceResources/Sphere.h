#pragma once
#include "Mesh.h"
class Sphere : public Mesh
{
public:
	Sphere(ID3D11Device* device) : Sphere(device, "Shpere" + std::to_string(m_creationIdx++)) {};
	Sphere(ID3D11Device* device, int divisionN, int divisionM, float radius) : Sphere(device, "Torus" + std::to_string(m_creationIdx++), divisionN, divisionM, radius) {};
	Sphere(ID3D11Device* device, std::string name, int divisionN, int divisionM, float radius);

	float& GetRadius() { return m_radius; }

	void RecalculateGeometry(ID3D11Device* device);
private:
	int m_divisionN, m_divisionM;
	float m_radius;
};

