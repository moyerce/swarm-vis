#include "AgentArray.h"
#include <fstream>
#include <iostream>
using namespace std;



AgentArray::AgentArray(std::string filename, int numTimeSteps)
{	
	timeSteps = numTimeSteps;
	agent_array = new std::vector<Agent>[timeSteps];
	
	for (int i = 0; i < timeSteps; i++)
	{		
		std::string current_filename = filename + padString(6, i + 1) + ".txt";		
		agent_array[i] = parseAgentsFromFile(current_filename);
		
	}
}

AgentArray::~AgentArray()
{
}

std::string AgentArray::padString(int length, int n)
{
	stringstream s;
	s << n;
	std::string s2 = s.str();
	std::string prepended = "";
	for (int i = 0; i < length - s2.size(); i++)
	{
		prepended += "0";
	}
	return prepended + s.str();
}

std::vector<Agent> AgentArray::parseAgentsFromFile(std::string basefile)
{
	std::vector<Agent> agentVector;
	const char *filename = basefile.c_str();	
	
	ifstream file;
	double x;
	double y;
		
	cout << "Opening File:" << filename << endl;
	file.open(filename);
	if(!file)
	{
		cerr << "Error: file could not be opened" << endl;	    
	}

	
	file >> x;
	file >> y;
	double z = 0.0;
	Agent *a = new Agent(x, y, z);
	//agents = new *agents[10];
	agentVector.push_back(*a);
	
	
	//a->setZ(z);
	
	while ( !file.eof() )
	{ // keep reading until end-of-file
		file >> x; // sets EOF flag if no value found
		file >> y;
		Agent *b = new Agent(x, y, z);			
		agentVector.push_back(*b);
		//cout << x << endl;
		//cout << y << endl;
	}
	
	file.close();
	cout << "End-of-file reached.." << endl;
	
	return agentVector;
}

vector<Agent> * AgentArray::getAgentsVector()
{
	return agent_array;
}