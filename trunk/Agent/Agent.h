#ifndef AGENT_H_
#define AGENT_H_

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
	Agent(double x, double y, double z);
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
	
	
private:
	double x;  // the x-coordinate
	double y;  // the z-coordinate
	double z;  // the y-coordinate

};

#endif /*AGENT_H_*/
