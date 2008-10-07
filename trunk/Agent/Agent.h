#ifndef AGENT_H_
#define AGENT_H_

class Agent
{
public:
	Agent();
	Agent(double x, double y, double z);
	virtual ~Agent();
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	double getX();
	double getY();
	double getZ();
	
private:
	double x;
	double y;
	double z;

};

#endif /*AGENT_H_*/
