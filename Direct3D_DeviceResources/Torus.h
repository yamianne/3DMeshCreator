#pragma once
#include "Mesh.h"
#include "IParametric.h"
class Torus : public Mesh, IParametric
{
public:
	Torus(ID3D11Device* device, GeometryType meshType) : Torus(device, meshType, "Torus" + std::to_string(m_creationIdx++)) {};
	Torus(ID3D11Device* device, GeometryType meshType, int divisionN, int divisionM, float minorRadius, float majorRadius) :
		Torus(device, meshType, "Torus" + std::to_string(m_creationIdx++), divisionN, divisionM, minorRadius, majorRadius) {};
	Torus(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN = 8, int divisionM = 8, float minorRadius = 0.5f, float majorRadius = 2.0f);
	
	int&			GetDivisionN() { return m_divisionN; }
	int&			GetDivisionM() { return m_divisionM; }
	float&			GetMinorRadius() { return m_minorRadius; }
	float&			GetMajorRadius() { return m_majorRadius; }

	// Inherited via Mesh
	virtual void    RecalculateTriMesh(ID3D11Device* device) override;
	virtual void    RecalculateQuadMesh(ID3D11Device* device) override;

	// Inherited via IParametric
	virtual Vector3 GetValue(float u, float v) override;
	virtual Vector3 GetDerivativeU(float u, float v) override;
	virtual Vector3 GetDerivativeV(float u, float v) override;
private:
	int m_divisionN, m_divisionM;
	float m_minorRadius, m_majorRadius;
};

