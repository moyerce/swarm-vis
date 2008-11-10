#ifndef AGENT_H_
#define AGENT_H_

#include <QColor>

class Agent
{
public:
	/** Default constructor.*/
	Agent();
	/** Constructor.
	 * Creates a new agent object with x, y, z position in space
	 * @param the x,y,z coordinates of the agent.
	 * @return
	 */
	Agent(double x, double y, double z, int type);
	/** Destructor.*/
	virtual ~Agent();
	/** Set the X-Coordinate of the Agent
	 * @param the x-coordinate of the agent.  
	 */
	void setX(double x);
	/** Set the Y-Coordinate of the Agent
	 * @param the y-coordinate of the agent.  
	 */	
	void setY(double y);	
	/** Set the Z-Coordinate of the Agent
	 * @param the z-coordinate of the agent.  
	 */		
	void setZ(double z);
	
	void setType(int type) { this->type = type; }
	void setColor(QColor color){ this->color = color; };
	void setSize(double size){ this->size = size; };
	void setGlyph(int glyph){ this->glyph = glyph; };
	
	/** Get the X-Coordinate of the Agent
	 * @return the x-coordinate of the agent.
	 */
	double getX();
	/** Get the Y-Coordinate of the Agent
	 * @return the y-coordinate of the agent.  
	 */
	double getY();
	/** Get the Z-Coordinate of the Agent
	 * @return the z-coordinate of the agent.  
	 */
	double getZ();
	
	int getType(){ return type; };
	QColor getColor(){ return color; };
	double getSize(){ return size; };
	int getGlyph(){ return glyph; };
	
	
private:
	double x;  // the x-coordinate
	double y;  // the z-coordinate
	double z;  // the y-coordinate
	int type;  // the type of agent
	QColor color;
	double size;
	int glyph;

};

#endif /*AGENT_H_*/
