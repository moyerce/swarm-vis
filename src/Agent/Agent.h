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
		  int type - the type of the agent
	 * @return
	 */
	Agent(double x, double y, double z, int type);
	/** Destructor.*/
	virtual ~Agent();
	/** Set the X-Coordinate of the Agent
	 * @param the x-coordinate of the agent
	 */
	void setX(double x);
	/** Set the Y-Coordinate of the Agent
	 * @param the y-coordinate of the agent
	 */
	void setY(double y);	
	/** Set the Z-Coordinate of the Agent
	 * @param the z-coordinate of the agent
	 */
	void setZ(double z);
	/** Set the type of the Agent
	 * @param int type - the type of the agent
	 */
	void setType(int type) { this->type = type; }
	/** Set the color of the Agent
	 *  @param QColor color - and rgba color representation
	 */
	void setColor(QColor color){ this->color = color; };
	/** Set the size of the Agent to be used in OpenGL
	 *  @param double size - the size
	 */
	void setSize(double size){ this->size = size; };
	/** Set the glyph used to represent the Agent
	 *  @param int glyph - an index used to pick a glyph to represent the agent
	 */
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
	 * @return the z-coordinate of the agent
	 */
	double getZ();
	/** Get the type of the Agent
	 * @return the type
	 */
	int getType(){ return type; };
	/** Get the color of the Agent
	 * @return the color
	 */
	QColor getColor(){ return color; };
	/** Get the size of the Agent
	 * @return the size
	 */
	double getSize(){ return size; };
	/** Get the glyph to use for the Agent
	 * @return the glyph indicator
	 */
	int getGlyph(){ return glyph; };

private:
	double x;  // the x-coordinate
	double y;  // the z-coordinate
	double z;  // the y-coordinate
	int type;  // the type of agent
	QColor color;	// the color of the Agent
	double size;	// the size (mostly for the point size in openGL
	int glyph;	// the glyph to use to represent the agent
};

#endif /*AGENT_H_*/
