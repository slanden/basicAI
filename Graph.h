#pragma once
#include <cstring>
#include "dep\cmathutils.h"
#include"dep\shapes.h"
#include <vector>

class Graph
{
	vec2 *m_positions;
	int m_nNodes;
	int m_maxNodes;
	float **m_adjacencyMatrix;	//edges

public:
	vec2 &getPosition(unsigned i)				    { return m_positions[i]; }
	const vec2 *getPositions()				  const { return m_positions; }
	float **getadjacencyMatrix()			  const { return m_adjacencyMatrix; }
	float getWeight(unsigned nid1, unsigned nid2) const { return m_adjacencyMatrix[nid1][nid2]; }
	const unsigned int size()				  const { return m_nNodes; }

	Graph(int a_nNodes) : m_maxNodes(a_nNodes), m_nNodes(0)
	{
		m_positions = new vec2[m_maxNodes];
		m_adjacencyMatrix = new float *[m_maxNodes];

		for (int i = 0; i < m_maxNodes; ++i)
		{
			m_adjacencyMatrix[i] = new float[m_maxNodes];
			memset(m_adjacencyMatrix[i], 0, 4 * m_maxNodes);
		}
		memset(m_positions, 0, sizeof(vec2) * m_maxNodes);
	}

	unsigned int addNode(const vec2 &pos);
	bool setEdge(unsigned int nid1, unsigned int nid2, float weight = 1);

	int findNode(const vec2 &pos);

	~Graph()
	{
		for (int i = 0; i < m_maxNodes; ++i)
		{
			delete [] m_adjacencyMatrix[i];
		}
		delete[] m_adjacencyMatrix;
		delete[] m_positions;
	}

	static Graph *makeGrid(int rows, int cols, float width, float height);
	void removeBlock(std::vector<aabb> a);

};

