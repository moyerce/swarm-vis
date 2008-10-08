#include "swarmvis.h"
#include <string>
#include <vector>

SwarmVis::SwarmVis(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidget = new GLWidget(this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(glWidget);
	ui.groupBox->setLayout(layout);
	
	int MAXTIME = 4427;
	double DELAY = 0.1;
	
	thread = new MyThread(this, MAXTIME, DELAY);
	
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setTimeIndex(int)));
	connect(thread, SIGNAL(playSignal()), ui.timeSlider, SLOT(addStep()));
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), thread, SLOT(setTimeIndex(int)));
	connect(ui.dSpinBoxDelay, SIGNAL(valueChanged(double)), thread, SLOT(setDelay(double)));
	
	connect(ui.dSpinBoxPointSize, SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSize(double)));
	connect(ui.spinBoxTrailLength, SIGNAL(valueChanged(int)), glWidget, SLOT(setTrailLength(int)));
}

SwarmVis::~SwarmVis()
{

}

void SwarmVis::btnPlay_clicked()
{	
	thread->start(); // after this, thread's run() method starts
}

void SwarmVis::btnStop_clicked()
{	
	thread->terminate(); // after this, thread's run() method starts 
}

void SwarmVis::menubar_action_handler(QAction* action)
{
	if (action->text().compare("&Quit") == 0)
	{
		exit(0);
	}
	if (action->text().compare("&Load Agent Data") == 0)
	{		 
		int timeSteps = 4428; //HARD CODED TIMESTEPS
		std::string filename = "/home/niels/workspace5/SwarmData/frame";
		
		agents = new AgentArray(filename, timeSteps);
		ui.timeSlider->setMaximum(agents->getTimeSteps() - 1);
		std::vector<Agent> * v = agents->getAgentsVector();
		glWidget->setInput(v);		
	}	
}
