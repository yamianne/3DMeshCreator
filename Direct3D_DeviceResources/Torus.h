#pragma once
#include "Mesh.h"
class Torus : public Mesh
{
public:
	Torus(ID3D11Device* device) : Torus(device, "Torus" + std::to_string(m_creationIdx++)) {};
	Torus(ID3D11Device* device, int divisionN, int divisionM, float minorRadius, float majorRadius) : 
		Torus(device, "Torus" + std::to_string(m_creationIdx++), divisionN, divisionM, minorRadius, majorRadius) {};
	Torus(ID3D11Device* device, std::string name, int divisionN = 8, int divisionM = 8, float minorRadius = 0.5f, float majorRadius = 2.0f);
	
	int& GetDivisionN() { return m_divisionN; }
	int& GetDivisionM() { return m_divisionM; }
	float& GetMinorRadius() { return m_minorRadius; }
	float& GetMajorRadius() { return m_majorRadius; }

	void RecalculateGeometry(ID3D11Device* device);
private:
	int m_divisionN, m_divisionM;
	float m_minorRadius, m_majorRadius;
};

