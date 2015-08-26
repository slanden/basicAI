#pragma once
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <functional>

enum MD_STATE {eDiscovered, eExplored, eFrontier, eStart, eGoal, ePath};

struct SolverMetaData
{
	MD_STATE state;
	unsigned previous;
	unsigned int g, f, h;
	unsigned int id;
	SolverMetaData(unsigned int id = 0) : state(eDiscovered), g(INT_MAX), id(id) {}
};

class Solver
{
	SolverMetaData *m_md;
	std::vector<int> solution;
	std::list<int> frontier;
	float **e_am;
	const vec2 *e_pos;
	unsigned m_nNodes;
	unsigned start, goal;

public:
	Solver(unsigned a_nNodes, float **am, const vec2 *pos)
		: m_nNodes(a_nNodes),
		e_am(am),
		e_pos(pos)
	{
		m_md = new SolverMetaData[m_nNodes];
	}

	~Solver()
	{
		delete[] m_md;
	}

	void path(unsigned a_start, unsigned a_goal)
	{
		start = a_start;
		goal = a_goal;

		//memset(m_md, 0, sizeof(SolverMetaData) * m_nNodes);
		for (int i = 0; i < m_nNodes; ++i)
			m_md[i] = SolverMetaData();

		frontier.push_front(start);
		m_md[start].state = eStart;
		m_md[start].previous = start;
		m_md[start].g = 0;
	}

	const SolverMetaData *getMD() const { return m_md; }

	struct CompareFunctor
	{
		Solver *s;
		CompareFunctor(Solver *s) : s(s) {}
		bool operator()(int left, int right)
		{
			return s->getMD()[left].f < s->getMD()[right].f;
		}		
	};

	const std::vector<int> getSolution() const { return solution; }
	std::vector<vec2> getSolvedPath() const
	{
		std::vector <vec2> p;
		for each(int i in solution)p.push_back(e_pos[i]);
		return p;
	}

	//does a solution exist or not
	void doSolve() { while (!step()); }

	bool step()
	{
		if (frontier.empty()) return false;

		frontier.sort(CompareFunctor(this));

		unsigned current = frontier.front();
		frontier.pop_front();
		if (current == goal)
		{
			while (!frontier.empty())frontier.pop_front();
			int t = current;
			do
			{
				solution.push_back(t);
				m_md[t].state = ePath;
				t = m_md[t].previous;

			} while (t != start && t >= 0 && t < m_nNodes);

				m_md[start].state = eStart;
				m_md[goal].state = eGoal;

				solution.push_back(start);
				std::reverse(solution.begin(), solution.end());
				return true;
		}
		m_md[current].state = eExplored;

		for (int i = 0; i < m_nNodes; ++i)
		{			
			if (e_am[current][i] > 0 && m_md[i].state != eExplored)
			{
				//push onto frontier
				if (m_md[i].state == eDiscovered)
				{
					frontier.push_front(i);
					m_md[i].state = eFrontier;
				}
				//calculate G score
				unsigned g = e_am[current][i] + m_md[current].g;
				unsigned h = distance(e_pos[i], e_pos[goal]);
				unsigned f = g + h;
				if (g < m_md[i].f)
				{
					m_md[i].g = g;
					m_md[i].h = h;
					m_md[i].f = f;
					m_md[i].previous = current;
				}
			}
		}
		return false;
	}

	static std::vector<vec2> find_path(unsigned a_nNodes, float **am, const vec2 *pos,
															unsigned start, unsigned goal)
	{
		Solver r(a_nNodes, am, pos);
		r.path(start, goal);
		r.doSolve();
		return r.getSolvedPath();
	}

};

struct Entity
{
	Entity(vec2 ent_pos, float agility, vec2 velocity, float speed) : position(ent_pos), agility(agility), velocity(velocity), speed(speed) {}

	float agility;
	vec2 position,velocity;
	vec2 force;
	std::vector<vec2> path;
	float speed;
	int index;

	void integrate(float dt)
	{
		velocity += force * dt;
		position += velocity * dt;
		force = vec2(0, 0);
	}
};

void drawGrid(const mat4 &m, const Graph &g, const Solver &md, int start, int end);