#ifndef AGENTARRAY_H_
#define AGENTARRAY_H_

#include "Agent.h"
#include <string>
#include <sstream>
#include <vector>

using namespace::std;


class AgentArray
{
public:
	AgentArray();
	virtual ~AgentArray();
	vector<Agent> * getAgentVector(int timeIndex);
	int getTimeSteps(){ return timeSteps; };
		
	
private:
	int timeSteps;
	vector<Agent> *agent_array;
	//Agent **agents;
	std::string intToFileString(int length, int n);
	vector<Agent> parseAgentsFromFile(std::string basefile);
	
	
};

#endif /*AGENTARRAY_H_*/
