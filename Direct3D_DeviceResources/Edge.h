#pragma once
class Edge
{
	int m_startVertexIdx;
	int m_endVertexIdx;
public:
	Edge(int startVertexIdx, int endVertexIdx) : m_startVertexIdx(startVertexIdx), m_endVertexIdx(endVertexIdx) {};
	int GetStartVertexIndex() { return m_startVertexIdx; }
	int GetEndVertexIndex() { return m_endVertexIdx; }
};

