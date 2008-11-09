#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
 

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(parent)
 {	
	COLORDIVISOR = 10000;
	initialized = false;

	xRot = 0;
    yRot = 0;
    zRot = 0;
    pointSize = 3.0;
    trailLength = 0;
	zoom = 400.0;
	showPaths = false;
	showBoundingBox = true;

	agentComponentR = 0.0;
	agentComponentG = 0.0;
	agentComponentB = 1.0;
	agentComponentO = 1.0;
	individualAgentComponentR = 0.5;
	individualAgentComponentG = 0.5;
	individualAgentComponentB = 0.5;
	individualAgentComponentO = 0.5;

 }

 GLWidget::~GLWidget()
 {   
 }

 QSize GLWidget::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(600, 600);
 }

 void GLWidget::setTimeIndex(int timeIndex)
 {   
	 std::cout << "TimeIndex:"<< timeIndex << std::endl;
	 currentTime = timeIndex;
	 initialized = true;
	
	 timeIndexChanged(currentTime); //signal that a new time index was received
	 updateGL();
 }

void GLWidget::initializeGL ()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	//glViewport(0, 0, 800, 600);
	//gluPerspective(45.0,800/600,0.1,100.0);
	

}

void  GLWidget::paintGL()
{		
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTranslated(-300.0, 0.0, 0.0);
	//glScaled(400.0, 400.0, 0.0);
	glScaled(zoom, zoom, 0.0);
	
	//rotate the view
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	glPointSize(pointSize);
	
	if (showBoundingBox) paintAxis();

	if (initialized)
	{		
		glColor4d (agentComponentR, agentComponentG, agentComponentB, agentComponentO);
		
	    std::vector<Agent> currentAgents = agent_array[currentTime];
		std::vector<Agent>::iterator iter; //the iterator for the agent vector
		iter = currentAgents.begin(); //set the start of the iterator
				
			
		while( iter != currentAgents.end() )
		{
			Agent a = *iter;			
			if (agentTypes.contains(a.getType()))
			{
				//std::cout << "Index:" << agentTypes.indexOf(a.getType())<<std::endl;
				double r = agentTypeColorR.at(agentTypes.indexOf(a.getType()));
				double g = agentTypeColorG.at(agentTypes.indexOf(a.getType()));
				double b = agentTypeColorB.at(agentTypes.indexOf(a.getType()));
				double o = agentTypeColorO.at(agentTypes.indexOf(a.getType()));
				//std::cout << c[0] <<"::"<<c[1] <<"::" <<c[2]<<"::"<<c[3] << std::endl;
				glColor4d (r, g, b, o);
			}
			else
			{
				glColor4d (agentComponentR, agentComponentG, agentComponentB, agentComponentO);
				//std::cout << "ERROR"<<std::endl;
			}
			glBegin (GL_POINTS);
				glVertex3d ( a.getX(),  a.getY(), a.getZ());
			glEnd ();
		      //glVertex3d (translatePoint(a.getX(),maxSize),  translatePoint(a.getY(),maxSize), translatePoint(a.getZ(),maxSize));		      
		      iter++;
		}
		

		buildTrail(trailLength);		
		if (showPaths) showSelectedPaths();
	}
}

void GLWidget::paintAxis()
{
	/*glBegin(GL_LINES);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd( );*/
	glColor4d (1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	 	
	 	glVertex3d(-1.0, 1.0,-1.0);
	 	glVertex3d(1.0, 1.0,-1.0);
	 	glVertex3d(-1.0, 1.0,1.0);
	 	glVertex3d(1.0, 1.0,1.0);
	 	
	 	glVertex3d(-1.0, -1.0,-1.0);
	 	glVertex3d(1.0, -1.0,-1.0);
	 	glVertex3d(-1.0, -1.0,1.0);
	 	glVertex3d(1.0, -1.0,1.0);
	 	
	 	glVertex3d(-1.0, 1.0,1.0);
	 	glVertex3d(-1.0, 1.0,-1.0);
	 	glVertex3d(-1.0, -1.0,1.0);
	 	glVertex3d(-1.0, -1.0,-1.0);
	 	
	 	glVertex3d(1.0, 1.0,1.0);
	 	glVertex3d(1.0, 1.0,-1.0);
	 	glVertex3d(1.0, -1.0,1.0);
	 	glVertex3d(1.0, -1.0,-1.0);
	 	
	 	glVertex3d(-1.0, -1.0,1.0);
	 	glVertex3d(-1.0, 1.0, 1.0);
	 	glVertex3d(-1.0, -1.0,-1.0);
	 	glVertex3d(-1.0, 1.0,-1.0);
	 	
	 	glVertex3d(1.0, -1.0,1.0);
	 	glVertex3d(1.0, 1.0, 1.0);
	 	glVertex3d(1.0, -1.0,-1.0);
	 	glVertex3d(1.0, 1.0,-1.0);	 	
	  glEnd();

}

void GLWidget::resizeGL(int width, int height)
{
	glViewport (0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-width, width, -height, height, -100.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	
	if (event->orientation() == Qt::Vertical)
	{
		zoom += numSteps * 20; 		
	} 
	event->accept();
	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();	
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::setInput(std::vector<Agent> * agents, int maxTime)
{
	agent_array = agents;
	maxTimeIndex = maxTime;
}
void GLWidget::normalizeAngle(int *angle)
 {
     while (*angle < 0)
         *angle += 360 * 16;
     while (*angle > 360 * 16)
         *angle -= 360 * 16;
 }

void GLWidget::setXRotation(int angle)
 {
     normalizeAngle(&angle);
     if (angle != xRot) {
         xRot = angle;        
         updateGL();
     }
 }
void GLWidget::setYRotation(int angle)
 {
     normalizeAngle(&angle);
     if (angle != yRot) {
         yRot = angle;         
         updateGL();
     }
 }

 void GLWidget::setZRotation(int angle)
 {
     normalizeAngle(&angle);
     if (angle != zRot) {
         zRot = angle;         
         updateGL();
     }
 }
 
 void GLWidget::setPointSize(double size)
 {
	 pointSize = size;
	 updateGL();
 }
 void GLWidget::setTrailLength(int length)
 {
	 trailLength = length;
	 updateGL();
 }
 
 void GLWidget::buildTrail(int length)
 {	
	 glPointSize(2);	 
	 for(int i = 1; i <= length; i++)
	 {
		if ((currentTime - i) >= 0)
		{
			std::vector<Agent> currentAgents = agent_array[currentTime - i];
	 		std::vector<Agent>::iterator iter; //the iterator for the agent vector
	 		iter = currentAgents.begin(); //set the start of the iterator
		
	 		//glColor3d (1.0, 1.0, 1.0); //set color to blue
	 		glColor4d (1.0, 1.0, 1.0, 1.0 - (double)i / 100.0);
	 	
	 		glBegin (GL_POINTS);	
	 		while( iter != currentAgents.end() )
	 		{
	 			Agent a = *iter;		     
	 			glVertex3d ( a.getX(),  a.getY(), a.getZ());
	 			iter++;
	 		}
	 		glEnd ();
		}
	 }
	 glPointSize(pointSize);	 
 }
 void GLWidget::showSelectedPaths()
 {	
	glPointSize(2);
	//glColor3d (1.0, 1.0, 1.0); //set color to blue
	glColor4d (individualAgentComponentR, individualAgentComponentG, individualAgentComponentB, individualAgentComponentO);	 	 	
	glBegin (GL_POINTS);
	
	for (int i = 0; i < selectedAgents.size(); i++)
	{
		QListWidgetItem *item = selectedAgents.at(i);
		bool * temp;
		int index = item->data(Qt::ToolTipRole).toInt(temp);
		
		for(int j = 0; j < maxTimeIndex; j++)
		{			
			glVertex3d(agent_array[j][index].getX(), agent_array[j][index].getY(), agent_array[j][index].getZ()); 		
		}
	}
	glEnd ();
	glPointSize(pointSize);	
 }

void GLWidget::toggleShowPaths(bool toggle)
{
	showPaths = toggle;
	//std::cout<< showPaths << std::endl;
	updateGL();
}


void GLWidget::agentsColorValueR(int value)
{
	agentComponentR = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::agentsColorValueG(int value)
{
	agentComponentG = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::agentsColorValueB(int value)
{
	agentComponentB = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::agentsColorValueO(int value)
{
	agentComponentO = (double)value /(double)COLORDIVISOR;
	updateGL();
}

void GLWidget::individualAgentsColorValueR(int value)
{
	individualAgentComponentR = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::individualAgentsColorValueG(int value)
{
	individualAgentComponentG = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::individualAgentsColorValueB(int value)
{
	individualAgentComponentB = (double)value /(double)COLORDIVISOR;
	updateGL();
}
void GLWidget::individualAgentsColorValueO(int value)
{
	individualAgentComponentO = (double)value /(double)COLORDIVISOR;
	updateGL();
}

void GLWidget::selectedAgentsChanged(QList<QListWidgetItem*> selection)
{	
	selectedAgents = selection;
	updateGL();
}

void GLWidget::boundingBox_toggled(bool value)
{
	showBoundingBox = value;
	updateGL();
}

void GLWidget::updateAgentTypesColor(QList<int> type, QList<double> r, QList<double> g, QList<double> b, QList<double> o)
{
	agentTypes = type;
	agentTypeColorR = r;
	agentTypeColorG = g;
	agentTypeColorB = b;
	agentTypeColorO = o;
	updateGL();
}

