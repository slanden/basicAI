#include "Graph.h"
#include "Solver.h"
#include <iostream>


unsigned int Graph::addNode(const vec2 &pos)
{
	if (m_nNodes == m_maxNodes) return -1;

	m_positions[m_nNodes] = pos;
	m_nNodes++;
	return m_nNodes -1;
}

bool Graph::setEdge(unsigned int nid1, unsigned int nid2, float weight)
{
	if (nid1 >= m_nNodes || nid2 >= m_nNodes || weight < 0)
		return false;
	m_adjacencyMatrix[nid1][nid2] = weight;
	m_adjacencyMatrix[nid2][nid1] = weight;
	return true;
}

int Graph::findNode(const vec2 &pos)
{
	for (int i = 0; i < m_nNodes; ++i)
	{
		//distanceSqr(pos, m_positions[0]);
	}
	
	return 1;
}

Graph *Graph::makeGrid(int rows, int cols, float width, float height)
{
	Graph *r = new Graph(rows*cols);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			int nid = r->addNode(vec2((j + 1) * width / (cols + 1), (i + 1) * height / (rows + 1) ));

			if (i != 0)
			r->setEdge( (i - 1) * cols + j, i * cols + j, 10);

			if (j != 0)
			r->setEdge   (i * cols + j - 1, i * cols + j, 10);

			if (i != 0 && j != 0)// diags right
				r->setEdge((i - 1) * cols + j - 1, i * cols + j,14);

			if (i != 0 && j != cols-1)// diags left
				r->setEdge((i - 1) * cols + j + 1, i * cols + j, 14);
		}

	}

	//for (int i = 0; i < r->size(); ++i)
	//{
	//	r->setEdge(i-1,i);
	//}

	return r;
}

void Graph::removeBlock(std::vector<aabb> a)
{
	for each(aabb o in a)
	{
		for (int i = 0; i < m_nNodes; ++i)
		{
			for (int j = 0; j < m_nNodes; ++j)
			{
				//if (m_positions[i].x > o.min().x && m_positions[i].y > o.min().y &&
				//	m_positions[i].x < o.max().x && m_positions[i].y < o.max().y)
				if (m_positions[i].x > o.min().x && m_positions[i].y > o.min().y &&
					m_positions[i].x < o.max().x && m_positions[i].y < o.max().y)
				{
					m_adjacencyMatrix[i][j] = 0;
					m_adjacencyMatrix[j][i] = 0;
				}

				if (m_adjacencyMatrix[i][j] != 0 || m_adjacencyMatrix[j][i] != 0)
				{
					if (line_aabb(line(m_positions[i], m_positions[j]), o).result)
					{
						m_adjacencyMatrix[i][j] = 0;
						m_adjacencyMatrix[j][i] = 0;
					}
				}

			}
		}
	}
}

void drawGrid(const mat4 &m, const Graph &g, const Solver &md, int start, int end)
{
	for (int i = 0; i < g.size(); ++i)
	{
		vec4 color;

		switch (md.getMD()[i].state)
		{
		case eFrontier:		color = { .357f, .68f, .40f, 1 }; break;
		case eDiscovered:	color = { .357f, .40f, .68f, 1 }; break;
		case eExplored:		color = { 106.f / 255, 106.f / 255, 106.f / 255, 1 }; break;
		case ePath:			color = { .5f, .30f, .30f, 1 }; break;
		}

		if (start == i) color = { .7f, .1f, .1f, 1 };
		if (end == i) color = { .68f, .40f, .357f, 1 };

		draw_point(m, point(g.getPositions()[i]), color);

		for (int j = i + 1; j < g.size(); ++j)
			if (g.getWeight(i, j) > 0)
			{
				draw_line(m, line(g.getPositions()[i], g.getPositions()[j]),vec4(.2,.2,.2, 1));
			}
	}
}

//int balls(int b)
//{
//	int t = 3;
//	switch (b)
//	{
//	case 1: t = 12;
//	case 2: t = 4;
//	case 5: t = 6;
//	}
//	return t;
//}
/*
	int array[rows * cols];
	int array2d[rows][cols];

	for(int x = 0; x < rows; ++x)
		for(int y = 0; y < cols; ++y)
		{
			array[x*cols + y];
			array2d[x][y];
			left
			arra
			
			
			
			y171 [(x-1)*cols + y];
			array2d[x-1][y];
			right
			array[(x+1)*cols + y];
			array2d[x+1][y];
			up
			array[(x)*cols + y +1];
			array2d[x][y+1];
			down
			array[(x)*cols + y -1];
			array2d[x][y-1];

		}
*/