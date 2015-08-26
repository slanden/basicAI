#pragma once
#include "dep\cmathutils.h"

vec2 Seek(vec2 target, const vec2 &position, const vec2 &velocity, const float agility)
{
	return normal(target - position) * agility - velocity;
}

vec2 Pursue(vec2 target, vec2 tVel, const vec2 &position, const vec2 &velocity, const float agility)
{
	return normal(target + tVel - position) * agility - velocity;
}

vec2 Wander(float distance, float radius, float jitter, const vec2 &velocity, const float agility)
{
	vec2 target;

	target = vec2::randNormal() * radius;	// Random position on a circle
	target += vec2::randNormal() * jitter;	// add a random jitter
	target = normal(target) * radius;		// Move the new random location back to the radius length
	target += normal(velocity) * distance;	// move the circle some distance from the player (makes the random wander target in a 'cone')
	
	return target * agility - velocity;     // target is now the location we're seeking, just apply the seek algorithm
}



/*
	Wherever the entity's information gets integrated:
		
		update:
					force = Seek(...) * .60 + Wander(...) * .40;
					velocity += force * dt
					position += velocity * dt;				
	*/