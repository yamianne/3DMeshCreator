#pragma once
#include "Mesh.h"
class Grid : public Mesh
{
public:
    Grid(ID3D11Device* device) : Grid(device, "Grid") {};
    Grid(ID3D11Device* device, std::string name, int divisionN = 10, int divisionM = 10, float unitSize = 1.0f);

    void RecalculateGeometry();
private:
    int m_divisionN, m_divisionM;
    float m_unitSize;
};

