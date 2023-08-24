#include "pch.h"
#include "Torus.h"

Torus::Torus(ID3D11Device* device, std::string name, int divisionN, int divisionM, float minorRadius, float majorRadius):
	Mesh(name, D3D11_PRIMITIVE_TOPOLOGY_LINELIST),
	m_divisionN(divisionN),
	m_divisionM(divisionM),
	m_minorRadius(minorRadius),
	m_majorRadius(majorRadius)
{
	RecalculateGeometry(device);
}

void Torus::RecalculateGeometry(ID3D11Device* device)
{
    m_vertices.clear();
    m_edges.clear();
    float R = m_majorRadius, r = m_minorRadius;
    int n = m_divisionN, m = m_divisionM;
    for (int i = 0; i < n; i++)
    {
        m_vertices.push_back(Vector3(R + r * cos(i * 2 * DirectX::XM_PI / n), r * sin(i * 2 * DirectX::XM_PI / n), 0));
        m_edges.push_back(Edge(i, (i + 1) % n));
    }
    for (int j = 1; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            m_edges.push_back(Edge(n * j + i - n, n * j + i));
            m_vertices.push_back(Vector3((R + r * cos(i * 2 * DirectX::XM_PI / n)) * cos(j * 2 * DirectX::XM_PI / m),
                    r * sin(i * 2 * DirectX::XM_PI / n),
                    (R + r * cos(i * 2 * DirectX::XM_PI / n)) * sin(j * 2 * DirectX::XM_PI / m)));
            m_edges.push_back(Edge(n * j + i, n * j + (i + 1) % n));
        }
    }
    for (int i = 0; i < n; i++)
        m_edges.push_back(Edge(n * m - n + i, i));

    Init(device);
}
