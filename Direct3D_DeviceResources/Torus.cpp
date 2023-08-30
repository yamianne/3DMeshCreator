#include "pch.h"
#include "Torus.h"

Torus::Torus(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN, int divisionM, float minorRadius, float majorRadius):
	Mesh(name, meshType),
	m_divisionN(divisionN),
	m_divisionM(divisionM),
	m_minorRadius(minorRadius),
	m_majorRadius(majorRadius)
{
	RecalculateGeometry(device);
}

void Torus::RecalculateTriMesh(ID3D11Device* device)
{
    float R = m_majorRadius, r = m_minorRadius;
    int n = m_divisionN, m = m_divisionM;
    for (int j = 0; j < m; j++)
    {
        float theta = 1.0f * j / m;
        for (int i = 0; i < n; i++)
        {
            float phi = 1.0f * i / n;
            m_vertices.push_back(GetValue(phi, theta));
            m_normals.push_back(GetNormal(phi, theta));
        }
    }
    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            m_edges.push_back(((j + 1) % m) * n + i);
            m_edges.push_back(j * n + i);
            m_edges.push_back(((j + 1) % m) * n + (i + 1) % n);
            m_edges.push_back(j * n + i);
            m_edges.push_back(j * n + (i + 1) % n);
            m_edges.push_back(((j + 1) % m) * n + (i + 1) % n);
        }
    }
}

void Torus::RecalculateQuadMesh(ID3D11Device* device)
{
    float R = m_majorRadius, r = m_minorRadius;
    int n = m_divisionN, m = m_divisionM;
    for (int i = 0; i < n; i++)
    {
        //m_vertices.push_back(Vector3(R + r * cos(i * 2 * DirectX::XM_PI / n), r * sin(i * 2 * DirectX::XM_PI / n), 0));
        m_vertices.push_back(GetValue(1.0f * i / n, 0));
        //m_edges.push_back(Edge(i, (i + 1) % n));
        m_edges.push_back(i);
        m_edges.push_back((i + 1) % n);

    }
    for (int j = 1; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            //m_edges.push_back(Edge(n * j + i - n, n * j + i));
            m_edges.push_back(n * j + i - n);
            m_edges.push_back(n * j + i);
            m_vertices.push_back(GetValue(1.0f * i / n, 1.0f * j / m));
            //m_vertices.push_back(Vector3((R + r * cos(i * 2 * DirectX::XM_PI / n)) * cos(j * 2 * DirectX::XM_PI / m),
            //    r * sin(i * 2 * DirectX::XM_PI / n),
            //    (R + r * cos(i * 2 * DirectX::XM_PI / n)) * sin(j * 2 * DirectX::XM_PI / m)));
            //m_edges.push_back(Edge(n * j + i, n * j + (i + 1) % n));
            m_edges.push_back(n * j + i);
            m_edges.push_back(n * j + (i + 1) % n);
        }
    }
    for (int i = 0; i < n; i++)
    {
        //m_edges.push_back(Edge(n * m - n + i, i));
        m_edges.push_back(n * m - n + i);
        m_edges.push_back(i);
    }
}

Vector3 Torus::GetValue(float u, float v)
{
    return Vector3((m_majorRadius + m_minorRadius * cos(v * XM_2PI)) * cos(u * XM_2PI),
                    m_minorRadius * sin(v * XM_2PI),
                    (m_majorRadius + m_minorRadius * cos(v * XM_2PI)) * sin(u * XM_2PI));
}

Vector3 Torus::GetDerivativeU(float u, float v)
{
    return Vector3(-sin(u * XM_2PI),
                    0,
                    cos(u * XM_2PI));
}

Vector3 Torus::GetDerivativeV(float u, float v)
{
    return Vector3(-sin(v * XM_2PI) * cos(u * XM_2PI),
                    cos(v * XM_2PI),
                   -sin(v * XM_2PI) * sin(u * XM_2PI));
}
