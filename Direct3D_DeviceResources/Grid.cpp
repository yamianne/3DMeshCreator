#include "pch.h"
#include "Grid.h"

Grid::Grid(ID3D11Device* device, std::string name, int divisionN, int divisionM, float unitSize) :
	Mesh(name, D3D11_PRIMITIVE_TOPOLOGY_LINELIST),
	m_divisionN(divisionN),
	m_divisionM(divisionM),
	m_unitSize(unitSize)
{
	RecalculateGeometry();
	Init(device);
};

void Grid::RecalculateGeometry()
{
	m_vertices.push_back(Vector3((-m_divisionN / 2) * m_unitSize, 0, (-m_divisionM / 2) * m_unitSize));
	for (int j = 1; j <= m_divisionM; j++)
	{
		m_vertices.push_back(Vector3((-m_divisionN / 2) * m_unitSize, 0, (j - m_divisionM / 2) * m_unitSize));
		m_edges.push_back(Edge(j - 1, j));
	}

	for (int i = 1; i <= m_divisionN; i++)
	{
		m_vertices.push_back(Vector3((i - m_divisionN / 2) * m_unitSize, 0, (-m_divisionM / 2) * m_unitSize));
		m_edges.push_back(Edge((i - 1) * (m_divisionM + 1), i * (m_divisionM + 1)));
		for (int j = 1; j <= m_divisionM; j++)
		{
			m_vertices.push_back(Vector3((i - m_divisionN / 2) * m_unitSize, 0, (j - m_divisionM / 2) * m_unitSize));
			m_edges.push_back(Edge(i * (m_divisionM + 1) + j - 1, i * (m_divisionM + 1) + j));

			m_edges.push_back(Edge((i - 1) * (m_divisionM + 1) + j, i * (m_divisionM + 1) + j));
		}
	}
}
