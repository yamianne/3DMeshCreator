#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(ID3D11Device* device, std::string name, int divisionN, int divisionM, float radius) :
	Mesh(name, D3D11_PRIMITIVE_TOPOLOGY_LINELIST),
    m_divisionN(divisionN),
    m_divisionM(divisionM),
	m_radius(radius)
{
	RecalculateGeometry(device);
}

void Sphere::RecalculateGeometry(ID3D11Device* device)
{
    m_vertices.clear();
    m_edges.clear();
    float R = m_radius;
    int n = m_divisionN, m = m_divisionM;
    m_vertices.push_back(Vector3(0, m_radius, 0));
    m_vertices.push_back(Vector3(0, -m_radius, 0));
    for (int j = 0; j < m; j++)
    {
        for (int i = 1; i < n; i++)
        {
            m_vertices.push_back(Vector3(   R * sin(i * DirectX::XM_PI / n) * cos(j * 2 * DirectX::XM_PI / m),
                                            R * cos(i * DirectX::XM_PI / n),
                                            R * sin(i * DirectX::XM_PI / n) * sin(j * 2 * DirectX::XM_PI / m)));
            auto startIdx = (n - 1) * j + i + 1;
            auto endIdx = ((n - 1) * (j + 1)) % ((n - 1) * m) + i + 1;
            m_edges.push_back(Edge(startIdx, endIdx));
            if(((n - 1) * j + i + 1) % (n - 1) != 2)
                m_edges.push_back(Edge((n - 1) * j + i, (n - 1) * j + i + 1));
        }

        m_edges.push_back(Edge(0, (n - 1) * j + 2));
        m_edges.push_back(Edge(1, (n - 1) * j + n));
    }
	Init(device);
}
