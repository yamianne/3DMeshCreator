#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN, int divisionM, float radius) :
	Mesh(name, meshType),
    m_divisionN(divisionN),
    m_divisionM(divisionM),
	m_radius(radius)
{
	RecalculateGeometry(device);
}

void Sphere::RecalculateTriMesh(ID3D11Device* device)
{
    m_normals.clear();
    float R = m_radius;
    int n = m_divisionN, m = m_divisionM;
    m_vertices.push_back(Vector3(0, m_radius, 0));
    m_normals.push_back(Vector3(0, m_radius, 0).Normalized());
    for (int j = 0; j < m; j++)
    {
        float angle = 1 * DirectX::XM_PI / n;
        Vector3 vert = Vector3(R * sin(angle) * cos(j * 2 * DirectX::XM_PI / m),
            R * cos(angle),
            R * sin(angle) * sin(j * 2 * DirectX::XM_PI / m));
        m_vertices.push_back(vert);
        m_normals.push_back(vert.Normalized());

        m_edges.push_back(0);
        m_edges.push_back(j + 1);
        m_edges.push_back(((j + 1) % m + 1));
    }

    for (int i = 2; i < n - 1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Vector3 vert = Vector3(R * sin(i * DirectX::XM_PI / n) * cos(j * 2 * DirectX::XM_PI / m),
                R * cos(i * DirectX::XM_PI / n),
                R * sin(i * DirectX::XM_PI / n) * sin(j * 2 * DirectX::XM_PI / m));
            m_vertices.push_back(vert);
            m_normals.push_back(vert.Normalized());
            m_edges.push_back(((j + 1) % m + 1) + m * (i - 2));
            m_edges.push_back(j + 1 + m * (i - 2));
            m_edges.push_back(j + 1 + m * (i - 1));

            m_edges.push_back(((j + 1) % m + 1) + m * (i - 1));
            m_edges.push_back(((j + 1) % m + 1) + m * (i - 2));
            m_edges.push_back(j + 1 + m * (i - 1));
        }
    }
    m_vertices.push_back(Vector3(0, -m_radius, 0));
    m_normals.push_back(Vector3(0, -m_radius, 0).Normalized());
    for (int j = 0; j < m; j++)
    {
        m_edges.push_back((n - 2) * m + 1);
        m_edges.push_back(((j + 1) % m + 1) + m * (n - 3));
        m_edges.push_back(j + 1 + m *(n - 3));
    }
}

void Sphere::RecalculateQuadMesh(ID3D11Device* device)
{
    float R = m_radius;
    int n = m_divisionN, m = m_divisionM;
    m_vertices.push_back(Vector3(0, m_radius, 0));
    m_vertices.push_back(Vector3(0, -m_radius, 0));
    for (int j = 0; j < m; j++)
    {
        for (int i = 1; i < n; i++)
        {
            m_vertices.push_back(Vector3(R * sin(i * DirectX::XM_PI / n) * cos(j * 2 * DirectX::XM_PI / m),
                R * cos(i * DirectX::XM_PI / n),
                R * sin(i * DirectX::XM_PI / n) * sin(j * 2 * DirectX::XM_PI / m)));
            auto startIdx = (n - 1) * j + i + 1;
            auto endIdx = ((n - 1) * (j + 1)) % ((n - 1) * m) + i + 1;
            m_edges.push_back(startIdx);
            m_edges.push_back(endIdx);
            if (((n - 1) * j + i + 1) % (n - 1) != 2)
            {
                m_edges.push_back((n - 1) * j + i);
                m_edges.push_back((n - 1) * j + i + 1);
            }
        }

        m_edges.push_back(0);
        m_edges.push_back((n - 1) * j + 2);
        m_edges.push_back(1);
        m_edges.push_back((n - 1) * j + n);
    }
}
