#include "AgentArray.h"

AgentArray::AgentArray(std::string directory, std::string filename)
{
	readInfoFile(directory, filename);
}

AgentArray::~AgentArray()
{
}

void AgentArray::readInfoFile(std::string directory, std::string filename)
{
		//read the info.txt file
		std::string fname = directory + filename;
		const char *name = fname.c_str();
		std::ifstream file;
		
		file.open(name);
		if(!file) std::cerr << "Error: "<< filename << " could not be opened" << std::endl;

		char word[255];
		bool processing_files = false;
		
		int i = 0;
		while ( !file.eof() )
		{
			file.getline(word, 255, '\n');
			std::string s = word;
			if (!processing_files)
			{
				processing_files = parseStringContent(s);
			}
			else
			{
				if (s != "")
				{
					agent_array[i] = parseAgentsFromFile(directory + s);
					i++;
				}
			}
			
		}
		file.close();
}

int AgentArray::getValue(std::string s)
{
	int index = s.find("=", 0);
	std::stringstream ss(s.substr(index + 1, s.length()));
	int n;
	ss >> n;
	return n;
}

bool AgentArray::parseStringContent(std::string s)
{
	std::string::size_type loc;
	
	loc = s.find( "FILES", 0 );
	if( loc != std::string::npos) return true;
	loc = s.find( "DIMENSIONS", 0 );
	if( loc != std::string::npos) dimensions = getValue(s);
	loc = s.find( "AGENTS", 0 );
	if( loc != std::string::npos) numAgents = getValue(s);
	loc = s.find( "FRAMES", 0 );
	if( loc != std::string::npos)
	{
		timeSteps = getValue(s);
		agent_array = new std::vector<Agent>[timeSteps];
	}
	loc = s.find( "RANGEX", 0 );
	if( loc != std::string::npos) rangeX = getValue(s);
	loc = s.find( "RANGEY", 0 );
	if( loc != std::string::npos) rangeY = getValue(s);
	loc = s.find( "RANGEZ", 0 );
	if( loc != std::string::npos) rangeZ = getValue(s);
	loc = s.find( "AGENTTYPES", 0 );
	if( loc != std::string::npos) agentTypes = getValue(s);
	
	return false;
}

std::vector<Agent> AgentArray::parseAgentsFromFile(std::string filename)
{
	std::vector<Agent> agentVector;
	const char *name = filename.c_str();	
	
	std::ifstream file;
	double x;
	double y;
	double z;
	int type;
	
	file.open(name);
	if(!file) std::cerr << "Error: file could not be opened" << std::endl;
	
	int i = 0;
	while ( !file.eof() && (numAgents > i) )
	{
		file >> x;
		file >> y;
		// if dimensions was specified
		if (dimensions == 3) file >> z;
		else x = 0.0;
		// if types are present in the file
		if (agentTypes == 1) file >> type;
		else type = 0;
		// create a new agent
		Agent *b = new Agent(normalize(x, rangeX), normalize(y, rangeY), normalize(z, rangeZ), type);
		i++;
		agentVector.push_back(*b);
	}
	file.close();
	return agentVector;
}

std::vector<Agent> * AgentArray::getAgentsVector()
{
	return agent_array;
}

double AgentArray::normalize(double val, double maxRange)
{
	double mid = maxRange / 2;
	
	if (val == mid) return 0;
	else if (val > mid) return (val - mid) / mid;
	else return -1.0 + (val / mid);
}