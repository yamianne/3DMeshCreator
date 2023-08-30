#pragma once
#include "Mesh.h"
class Grid : public Mesh
{
public:
    Grid(ID3D11Device* device, GeometryType meshType) : Grid(device, meshType, "Grid") {};
    Grid(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN = 10, int divisionM = 10, float unitSize = 1.0f);

    // Inherited via Mesh
    virtual void    RecalculateTriMesh(ID3D11Device* device) override;
    virtual void    RecalculateQuadMesh(ID3D11Device* device) override;
private:
    int m_divisionN, m_divisionM;
    float m_unitSize;
};

