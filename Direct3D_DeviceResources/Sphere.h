#pragma once
#include "Mesh.h"
class Sphere : public Mesh
{
public:
	Sphere(ID3D11Device* device, GeometryType meshType) : Sphere(device, meshType, "Shpere" + std::to_string(m_creationIdx++)) {};
	Sphere(ID3D11Device* device, GeometryType meshType, int divisionN, int divisionM, float radius) :
		Sphere(device, meshType, "Sphere" + std::to_string(m_creationIdx++), divisionN, divisionM, radius) {};
	Sphere(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN = 8, int divisionM = 8, float radius = 2.0f);

	int& GetDivisionN() { return m_divisionN; }
	int& GetDivisionM() { return m_divisionM; }
	float& GetRadius() { return m_radius; }

	// Inherited via Mesh
	virtual void    RecalculateTriMesh(ID3D11Device* device) override;
	virtual void    RecalculateQuadMesh(ID3D11Device* device) override;
private:
	int m_divisionN, m_divisionM;
	float m_radius;
};

