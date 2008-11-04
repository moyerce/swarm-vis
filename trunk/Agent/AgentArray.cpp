#include "AgentArray.h"
#include <fstream>
#include <iostream>
using namespace std;



AgentArray::AgentArray(std::string directory, std::string filename)
{
	readInfoFile(directory, filename);	
	/*for (int i = 0; i < timeSteps; i++)
	{		
		std::string current_filename = filename + padString(6, i + 1) + ".txt";		
		agent_array[i] = parseAgentsFromFile(current_filename);
		
	}*/	
}

AgentArray::~AgentArray()
{
}

void AgentArray::readInfoFile(std::string directory, std::string filename)
{	
		//read the info.txt file
		std::string fname = directory + filename;
		const char *name = fname.c_str();	
		ifstream file;		
		cout << "Opening File:" << filename << endl;
		file.open(name);
		if(!file)
		{
				cerr << "Error: file could not be opened" << endl;	    
		}
		char word[255];
		bool processing_files = false;
		
		int i = 0;
		while ( !file.eof() )
		{
			file.getline(word, 255, '\n');
			std::string s = word;			
			cout << s << endl;
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
	int index = s.find("=",0);		
	stringstream ss(s.substr(index+1,s.length()));		
	int n;
	ss >> n;	
	return n;
}

bool AgentArray::parseStringContent(std::string s)
{
	string::size_type loc;
	
	loc = s.find( "FILES", 0 );
	if( loc != string::npos) return true;
		
	loc = s.find( "DIMENSIONS", 0 );
	if( loc != string::npos) dimensions = getValue(s);
	loc = s.find( "AGENTS", 0 );
	if( loc != string::npos) numAgents = getValue(s);
	loc = s.find( "FRAMES", 0 );
	if( loc != string::npos)
	{
		timeSteps = getValue(s);		
		agent_array = new std::vector<Agent>[timeSteps];		
	}
	loc = s.find( "RANGEX", 0 );
	if( loc != string::npos) rangeX = getValue(s);
	loc = s.find( "RANGEY", 0 );
	if( loc != string::npos) rangeY = getValue(s);
	loc = s.find( "RANGEZ", 0 );
	if( loc != string::npos) rangeZ = getValue(s);
	
	return false;
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

std::vector<Agent> AgentArray::parseAgentsFromFile(std::string filename)
{
	std::vector<Agent> agentVector;
	const char *name = filename.c_str();	
	
	ifstream file;
	double x;
	double y;
	double z;
		
	cout << "Opening File:" << filename << endl;
	file.open(name);
	if(!file)
	{
		cerr << "Error: file could not be opened" << endl;	    
	}
	
	file >> x;
	file >> y;
	if (dimensions == 3)
	{
		file >> z;
	}
	else
	{
		z = 0.0;
	}
	
	Agent *a = new Agent(normalize(x, rangeX), normalize(y, rangeY), normalize(z, rangeZ));	
	agentVector.push_back(*a);
	
	while ( !file.eof() )
	{ // keep reading until end-of-file
		file >> x; // sets EOF flag if no value found
		file >> y;		
		if (dimensions == 3)
		{
			file >> z;
		}
		Agent *b = new Agent(normalize(x, rangeX), normalize(y, rangeY), normalize(z, rangeZ));			
		agentVector.push_back(*b);		
	}
	
	file.close();
	cout << "End-of-file reached.." << endl;
	
	return agentVector;
}

vector<Agent> * AgentArray::getAgentsVector()
{
	return agent_array;
}

double AgentArray::normalize(double val, double maxRange)
{
	double mid = maxRange / 2;
	if (val == mid) 
	{
		return 0;
	}
	else if (val > mid)//between 0 and 1
	{
		return (val - mid) / mid;
	}
	else
	{
		return -1.0 + (val / mid);
	}
}