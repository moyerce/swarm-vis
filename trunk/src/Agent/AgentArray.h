#ifndef AGENTARRAY_H_
#define AGENTARRAY_H_

#include "Agent.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>

class AgentArray
{
public:
	/** Default constructor.
	 *  Reads the files containing the agent data
	 *  @param string filename - the path and basename of the info file to read
	 * 		  int TimeSteps - a hint on how many files there are
	 *	The info file assumes a header of the form:
	 *	DIMENSIONS = 3
	 *	AGENTS = 40
	 *	FRAMES = 4
	 *	RANGEX = 400
	 *	RANGEY = 400
	 *	RANGEZ = 400
	 *	AGENTTYPES = 1
	 *
	 *	FILES
	 *	frame000001.txt
	 *	frame000002.txt
	 *	frame000003.txt
	 *	frame000004.txt
	 * */
	AgentArray(std::string directory, std::string filename);
	
	/** Destructor.*/
	virtual ~AgentArray();
	
	/** Get a vector containing the agents
	 * @return a vector of agents for the time index
	 */
	std::vector<Agent> * getAgentsVector();	
	/** Get the number of time steps that have agent data
	 * @return the number of time steps
	 */
	int getTimeSteps(){ return timeSteps; };
	/** Get the number of agent
	 * @return the number of agents
	 */
	int getNumAgents(){ return numAgents; };
	/** Get the maximum range on the x-axis
	 * @return the x-range
	 */
	double getRangeX(){ return rangeX; };
	/** Get the maximum range on the y-axis
	 * @return the y-range
	 */
	double getRangeY(){ return rangeY; };
	/** Get the maximum range on the z-axis
	 * @return the z-range
	 */
	double getRangeZ(){ return rangeZ; };
	/** Get the maximum distance between the agents
	 * @return the maximum distance
	 */
	double getMaxDistance(){ return maxDistance; };

private:
	int dimensions;
	int timeSteps;
	int numAgents;
	std::vector<Agent> *agent_array;
	double rangeX;
	double rangeY;
	double rangeZ;
	int agentTypes;
	double maxDistance;

	/** Reads positional data for an agent from a file
	 * @param string basefile - the path to the file including the filename
	 * @return a vector containing the agents found in the file
	 */
	std::vector<Agent> parseAgentsFromFile(std::string filename);
	/** Reads the info file that contains the header for the agent data
	 * @param string directory - the path to the file
	 * @param string filename - the name of the file
	 */
	void readInfoFile(std::string directory,std::string filename);
	/** Parses the header strings of the info file
	 * @param string s - a particular line in the header
	*/
	bool parseStringContent(std::string s);
	/** Parses an integer value of a line in the header of the info file
	 * @param string s - a string representing an integer
	 * @return the string converted to an integer
	*/
	int getValue(std::string s);
	/** Normalizes a coordinate value by the maximum range on that axis
	 * @param double val - the coordinate value on a particular axis
	 * @param double maxRange - the range
	 * @return the string converted to an integer
	*/
	double normalize(double val, double maxRange);
};

#endif /*AGENTARRAY_H_*/
