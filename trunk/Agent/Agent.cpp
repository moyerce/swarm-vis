#include "Agent.h"


Agent::Agent()
{
}
Agent::Agent(double x, double y, double z)
{
	 this->x = x;
	 this->y = y; 
	 this->z = z; 
}
Agent::~Agent()
{
}

void Agent::setX(double x) { this->x = x; }
void Agent::setY(double y) { this->y = y; }
void Agent::setZ(double z) { this->z = z; }
double Agent::getX(){ return x; }
double Agent::getY(){ return y; }
double Agent::getZ(){ return z; }