#include "dep\shapes.h"
#include "Graph.h"
#include "Solver.h"
#include <iostream>
#include <GLFW\glfw3.h>
#include <cassert>
#include <time.h>
#include "Behaviors.h"


/*
	Current is the most recent node on the smoothed path.
	Next is the position that comes after current
	Previous is the position before Next.

		for the current position:
			cast a line to the next position:
				if that line collides with ANY obstacles:
					add Previous to the smoothed path.
					set current to the previous.

*/

std::vector<vec2> smoothPath(const std::vector<vec2> &path,
	const std::vector<aabb> &obstacles)
{
	std::vector<vec2> spath;
	spath.push_back(path[0]);
	
	for (int i = 1; i < path.size(); ++i)
	{
		line raycast = line(spath.back(), path[i]);

		bool collision = false;
		for each(auto o in obstacles) // Have we collided with ANYTHING AT ALL?
		{
			if (line_aabb(raycast, o).result == true)
				collision = true;
		}
		if (collision)
			spath.push_back(path[i - 1]);
		// IF we have collided, then we want to push_back path[i-1]
	}
	spath.push_back(path.back());

	return spath;
}

enum boxTypes{ small, med, large, x_large};
aabb boxGen(int id, bool xy_constraints)
{
	int min, max;
	int xPos, yPos, xDim,yDim;
	switch (id)
		{
		case 0:
		{
			min = 10;
			max = 30;
		}
			break;
		case 1:
		{
			min = 31;
			max = 93;
		}
			break;
		case 2:
		{
			min = 94;
			max = 282;
		}
			break;
		case 3:
		{
			min = 283;
			max = 849;
		}
	}

	switch (xy_constraints)
	{
	case true:
	{
		xDim = yDim = rand() % max + min;
		xPos = rand() % 630 + 20;
		yPos = rand() % 470 + 20;
	}
	break;
	case false:
	{
		xDim = rand() % max + min;
		yDim = rand() % max + min;
		xPos = rand() % max + min;
		yPos = rand() % max + min;
	}
	break;
	}

	return aabb(vec2(xPos, yPos), vec2(xDim, yDim) );
}

void main()
{
	//auto a = line({ 0, 0 }, { 5, 5 }); // slope?? 1
	//auto b = line({ 5, 0 }, {0, 5});	  // slope?? -1
	//assert(line_line(a,b).result == true);
	// a = line({ 0, 0 }, { 2.4f, 2.4f }); // slope?? 1
	// b = line({ 5, 0 }, { 0, 5 });	  // slope?? -1
	//assert(line_line(a, b).result == false);
	//auto a = line({0,1}, {5,1});
	//auto b = aabb({5,0}, {4,4});
	//assert(line_aabb(a,b).result == true);
	//assert(line_aabb(line({ 1, 1 }, { 10, 10 }), aabb({ 700, 700 }, { 10, 10 })).result == false);
	//assert(line_aabb(line({ 1, 1 }, { 800, 800 }), aabb({ 400, 400 }, { 100, 100 })).result == true);
	srand(time(NULL));
	short sWidth = 640;
	short sHeight = 480;
	auto w = openContext();
	auto m = mat4::ortho(0, sWidth, 0, sHeight, 0, 100);

	Graph *g = Graph::makeGrid(12, 16, sWidth, sHeight);

	float delay = 0.9;
	bool newPath = true;
	int lastGoal = 0;
	int nextGoal = 0;
	Solver dfs(g->size(), g->getadjacencyMatrix(), g->getPositions());
	dfs.path(0, lastGoal);
	glfwSetTime(0);

	float current = 0;
	float dt = 0;
	float total = 0;
	vec2 previousPosition;
	std::vector<aabb> obstacles;

	//vec2 agility = { 1, 1 };
	vec2 velocity = { 5, 5 };
	Entity e = Entity(vec2(400,400), 10, velocity, 10);
	//Entity e2 = e;

	//aabb a = aabb({ 2, 2 }, {4,4})
	//aabb a = boxGen(small, true);
	//std::cout << a.position.x << "," << a.position.y << "," << a.dimension.x << "," << a.dimension.y << std::endl;

	//obstacles.push_back(aabb({ 300, 300 }, { 200, 200 }));

	for (int i = 0; i < 5; ++i)
	{
		aabb a = boxGen(rand() % 3, true);
		obstacles.push_back(a);
	}

	//obstacles.push_back(aabb(boxGen(small, true)));
	//obstacles.push_back(aabb(boxGen(small, true)));
	//obstacles.push_back(aabb(boxGen(small, true)));
	//obstacles.push_back(aabb(boxGen(small, true)));
	//obstacles.push_back(aabb(boxGen(small, true)));
	//obstacles.push_back(aabb(boxGen(small, true)));
	
	g->removeBlock(obstacles);

	//const aabb box01({ 300, 300 }, { 200, 200 });
	//obstacles.push_back(box01);

	//while (updateContext(w))
	//{
	//	current = glfwGetTime();
	//	dt = current - total;
	//	total = current;
	//	if (dt > 0.16) dt = 0.16;

	//	e.force = Wander(1, 20, 8, e.velocity, e.agility);
	//	e.integrate(dt);
	//	draw_point(m, point(e.position));
	//}
	while (updateContext(w))
	{
		//draw rectangle
		//draw_aabb(m, box, vec4(1, 1, 1, 1));
		for (int i = 0; i < obstacles.size(); ++i)
			draw_aabb(m, obstacles[i]);

		//previousPosition = e.position;

		// calculate DT
		current = glfwGetTime();
		dt = current - total;
		total = current;
		if (dt > 0.16) dt = 0.16;
		///////////////////////////////////////

		
		if (glfwGetTime() >= delay) // Apply delay, if desired, for calculating a new path
			if (newPath)
			{
				glfwSetTime(0);
				lastGoal = nextGoal;
				bool trying = true;
				while (trying)
				{
					nextGoal = rand() % g->size();
					
					for (int i = 0; i < g->size(); ++i)
					{
						if (g->getadjacencyMatrix()[nextGoal][i] > 0)
						{
							trying = false;
							break;
						}
					}
				}
				e.path = Solver::find_path(g->size(), g->getadjacencyMatrix(),
									g->getPositions(), lastGoal, nextGoal);

				e.path = smoothPath(e.path, obstacles);


				//e.position = g->getPosition(lastGoal);
				e.index = 0;
				newPath = false;
			}

		
		draw_point(m, point(e.position));
		for (int i = 1; i < e.path.size(); ++i)
		{
			draw_line(m, line(e.path[i - 1], e.path[i]), vec4(1, 0, 0, 1));
		}



		if (e.index < e.path.size())
		{	
			if (distance(e.position, e.path[e.index]) < 5) e.index++; //when we hit a node, increment the index
			
			if (e.index < e.path.size()/* && e.path[e.index] != e.position*/)
			{

				//if (distance(previousPosition, e.position) >= 30)
				//{
				//	std::cout << distance(previousPosition, e.position) << std::endl;
				//	std::cout << previousPosition.x << " " << previousPosition.y << std::endl;
				//	std::cout << e.position.x << " " << e.position.y << std::endl << std::endl;
				//}

				// seek
				//auto norm = normal(e.path[e.index] - e.position) * e.agility - e.velocity;
				//								
				//e.velocity += norm * dt;
				////if (abs(length(norm) - 1) < 0.1)
				//e.position += e.velocity * dt;
			
				e.force = Seek(e.path[e.index], e.position, e.velocity, e.agility); //???

				e.integrate(dt);
			}

			
		}
		else newPath = true;




		/*{
			delay = 0;
			glfwSetTime(0);

			if (newPath)
			{
				newPath = false;
				lastGoal = nextGoal;
				bool trying = true;
				while (trying)
				{
					nextGoal = rand() % g->size();
					for (int i = 0; i < g->size(); ++i)
					{
						if (g->getadjacencyMatrix()[nextGoal][i] > 0)
							trying = false;
					}
				}
				
				dfs.path(lastGoal, nextGoal);
			}
			if (dfs.step())
			{
				newPath = true;
				delay = .2;
			}
		}*/

		//drawGrid(m,*g, dfs, lastGoal, nextGoal);
	}
	delete g;
	closeContext(w);
}