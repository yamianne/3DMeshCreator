#include "pch.h"
#include "Grid.h"

Grid::Grid(ID3D11Device* device, GeometryType meshType, std::string name, int divisionN, int divisionM, float unitSize) :
	Mesh(name, meshType),
	m_divisionN(divisionN),
	m_divisionM(divisionM),
	m_unitSize(unitSize)
{
	Color = { 0.8f,  0.5f,  0.8f, 1.00f };
	RecalculateGeometry(device);
};

void Grid::RecalculateTriMesh(ID3D11Device* device)
{
	for (int j = 0; j <= m_divisionM; j++)
	{
		for (int i = 0; i <= m_divisionN; i++)
		{
			m_vertices.push_back(Vector3((i - 1.0f * m_divisionN / 2) * m_unitSize, 0, (j - 1.0f * m_divisionM / 2) * m_unitSize));
			m_normals.push_back(Vector3::UnitY);
		}
	}
	int numberOfindices = 6 * (m_divisionN + 1) * (m_divisionM + 1);
	int currentidx = 0;
	while (m_edges.size() < numberOfindices)
	{
		m_edges.push_back(currentidx);
		m_edges.push_back(currentidx + 1);
		m_edges.push_back(currentidx + m_divisionN + 1);
		m_edges.push_back(currentidx + 1);
		m_edges.push_back(currentidx + m_divisionN + 2);
		m_edges.push_back(currentidx + m_divisionN + 1);
		currentidx++;
	}
}

void Grid::RecalculateQuadMesh(ID3D11Device* device)
{
	m_vertices.push_back(Vector3((-m_divisionN / 2) * m_unitSize, 0, (-m_divisionM / 2) * m_unitSize));
	for (int j = 1; j <= m_divisionM; j++)
	{
		m_vertices.push_back(Vector3((-m_divisionN / 2) * m_unitSize, 0, (j - m_divisionM / 2) * m_unitSize));
		m_edges.push_back(j - 1);
		m_edges.push_back(j);
	}

	for (int i = 1; i <= m_divisionN; i++)
	{
		m_vertices.push_back(Vector3((i - m_divisionN / 2) * m_unitSize, 0, (-m_divisionM / 2) * m_unitSize));
		m_edges.push_back((i - 1) * (m_divisionM + 1));
		m_edges.push_back(i * (m_divisionM + 1));
		for (int j = 1; j <= m_divisionM; j++)
		{
			m_vertices.push_back(Vector3((i - m_divisionN / 2) * m_unitSize, 0, (j - m_divisionM / 2) * m_unitSize));
			m_edges.push_back(i * (m_divisionM + 1) + j - 1);
			m_edges.push_back(i * (m_divisionM + 1) + j);

			m_edges.push_back((i - 1) * (m_divisionM + 1) + j);
			m_edges.push_back(i * (m_divisionM + 1) + j);
		}
	}
}
