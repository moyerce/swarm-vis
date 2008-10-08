#ifndef AGENTARRAY_H_
#define AGENTARRAY_H_

#include "Agent.h"
#include <string>
#include <sstream>
#include <vector>

class AgentArray
{
public:
	/** Default constructor.
	 *  Reads the files containing the agent data
	 *  @param string filename - the path and basename of the files to read
	 * 		   int TimeSteps - a hint on how many files there are
	 * */	
	AgentArray(std::string filename, int numTimeSteps);
	
	/** Destructor.*/
	virtual ~AgentArray();
	
	/** Get a the agents
	 * @return a vector of agents for the time index
	 */
	std::vector<Agent> * getAgentsVector();
	
	/** Get the number of time steps that have agent data
	 * @return the number of time steps
	 */
	int getTimeSteps(){ return timeSteps; };
	
	//FUNCTIONALITY NEEDS TO BE IMPLEMENTED
	double getRangeX(){ return rangeX; };
	double getRangeY(){ return rangeY; };
	double getRangeZ(){ return rangeZ; };
		
	
private:
	int timeSteps;
	std::vector<Agent> *agent_array;
	double rangeX;
	double rangeY;
	double rangeZ;

	/** Prepend a string with zeros (0) and append an integer
	 * @param int length - the total lenght of the final string
	 * 		  int n - the integer to be padded
	 * @return a string of length "length" representing n padded with leading zeros (0) 
	 */
	std::string padString(int length, int n);
	/** Reads positional data for an agent from a file
	 * @param string basefile - the path to the file including the filename
	 * @return a vector containing the agents found in the file
	 */
	std::vector<Agent> parseAgentsFromFile(std::string basefile);
	
	
};

#endif /*AGENTARRAY_H_*/
