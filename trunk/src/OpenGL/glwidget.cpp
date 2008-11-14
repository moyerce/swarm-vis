#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
 

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(parent)
 {	
	initialized = false;

	xRot = 0;
    yRot = 0;
    zRot = 0;
    pointSize = 3.0;
    trailLength = 0;
	zoom = 400.0;
	showPaths = false;
	showBoundingBox = true;
	depthChecking = true;
	sameColor = false;

	trackColorR = 0.0;
	trackColorG = 0.0;
	trackColorB = 0.0;
	trackColorO = 0.35;

	colorOverride = false;

 }



void GLWidget::initializeGL ()
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	//glEnable (GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	//glViewport(0, 0, 800, 600);
	//gluPerspective(45.0,800/600,0.1,100.0);
}

void  GLWidget::paintGL()
{
	if (depthChecking) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);  


	glScaled(zoom, zoom, 0.0);	
	//rotate the view
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	//glPointSize(pointSize);
	
	if (showBoundingBox) paintAxis();

	if (initialized)
	{			
	    std::vector<Agent> currentAgents = agent_array[currentTime];
		std::vector<Agent>::iterator iter; //the iterator for the agent vector
		iter = currentAgents.begin(); //set the start of the iterator
		int i = 0;
		
		while( iter != currentAgents.end() )
		{		
			Agent a = *iter;					
			if (agentTypes.contains(a.getType()) && colorOverride)
			{
				glPointSize(a.getSize());
	
				double r = agentTypeColorR.at(agentTypes.indexOf(a.getType()));
				double g = agentTypeColorG.at(agentTypes.indexOf(a.getType()));
				double b = agentTypeColorB.at(agentTypes.indexOf(a.getType()));
				double o = agentTypeColorO.at(agentTypes.indexOf(a.getType()));
				//std::cout << c[0] <<"::"<<c[1] <<"::" <<c[2]<<"::"<<c[3] << std::endl;
				glColor4d (r, g, b, o);				
				glBegin (GL_POINTS);
					glVertex3d (a.getX(),  a.getY(), a.getZ());
				glEnd ();
				if (showPaths)
				{
					if (sameColor)
						glColor4d (r, g, b, trackColorO);						
					else
						glColor4d (trackColorR, trackColorG, trackColorB, trackColorO);
					showSelectedPaths(i);
					//std::cout << a.getX() <<  a.getY() << a.getZ() <<std::endl;
				}
				buildTrail(i, trailLength);
			}
			else
			{				
				glPointSize(a.getSize()); //set the agent's point size
	
				QColor c = a.getColor(); //set the agents color				
				double r = (double)c.red() / 255;
				double g = (double)c.green() / 255;
				double b = (double)c.blue() / 255;
				double o = (double)c.alpha() / 255;
				
				glColor4d (r, g, b, o);

				glBegin (GL_POINTS);
					glVertex3d ( a.getX(),  a.getY(), a.getZ());
				glEnd ();
				if (showPaths)
				{
					if (sameColor)
						glColor4d (r, g, b, trackColorO);
					else
						glColor4d (trackColorR, trackColorG, trackColorB, trackColorO);						
					showSelectedPaths(i);
					//std::cout << a.getX() <<  a.getY() << a.getZ() <<std::endl;
					//buildTrail(i, trailLength);
				}
				buildTrail(i, trailLength);
				
			}			
		      //glVertex3d (translatePoint(a.getX(),maxSize),  translatePoint(a.getY(),maxSize), translatePoint(a.getZ(),maxSize));		      
		    iter++;
			i++;
		}
		//buildTrail(trailLength);		
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
	glColor4d (0.0, 0.0, 0.0, 1.0);
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
 
 void GLWidget::setTrailLength(int length)
 {
	 trailLength = length;
	 updateGL();
 }
 
 void GLWidget::buildTrail(int agentIndex, int length)
 {	
	 glPointSize(2);	 
	 for(int i = 1; i <= length; i++)
	 {
		int k = currentTime - i;
		if ((k) >= 0)
		{
			int k = currentTime - i;
			Agent a = agent_array[k][agentIndex];
			Agent ab;
			if (k != 0)
	 			ab = agent_array[k - 1][agentIndex];
			else
				ab = a;
	 		
	 		
			QColor c = a.getColor(); //set the agents color				
			double r = (double)c.red() / 255;
			double g = (double)c.green() / 255;
			double b = (double)c.blue() / 255;
			//double o = (double)c.alpha() / 255;
			
			glColor4d (r, g, b, 0.7 - 0.7*(double)i / (double)length);
	 		glBegin (GL_LINES);
	     
	 		glVertex3d ( a.getX(),  a.getY(), a.getZ());
			glVertex3d ( ab.getX(),  ab.getY(), ab.getZ());	 	
	 		glEnd ();
		}
	 }	 
 }
 void GLWidget::showSelectedPaths(int index)
 {	
	glPointSize(2);
	//glColor3d (1.0, 1.0, 1.0); //set color to blue

	//std::cout<< "Index: "<< index<< std::endl;
	glBegin(GL_LINES);
	
	for (int i = 0; i < selectedTrackAgents.size(); i++)
	{
		int agent = selectedTrackAgents.at(i);		
		if (index == agent)
		{
			//std::cout<< "AgentID: "<< agent<< std::endl;
			for(int j = 0; j < maxTimeIndex; j++)
			{
				int k = j+ 1;
				glVertex3d(agent_array[j][index].getX(), agent_array[j][index].getY(), agent_array[j][index].getZ());
				if (k < maxTimeIndex)
					glVertex3d(agent_array[k][index].getX(), agent_array[k][index].getY(), agent_array[k][index].getZ());
			}
		}
	}
	glEnd();	
 }

void GLWidget::toggleShowPaths(bool toggle)
{
	showPaths = toggle;
	//std::cout<< showPaths << std::endl;
	updateGL();
}

void GLWidget::setTrackColor(double r, double g, double b, double o)
{
	trackColorR = r;
	trackColorG = g;
	trackColorB = b;
	trackColorO = o;
	updateGL();
}

void GLWidget::selectedTrackAgentsChanged(QList<int> selection)
{	
	selectedTrackAgents = selection;
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

void GLWidget::showPathWithSameColor(bool value)
{
	sameColor = value;
	updateGL();
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
	 //std::cout << "TimeIndex:"<< timeIndex << std::endl;
	 currentTime = timeIndex;
	 initialized = true;
	
	 timeIndexChanged(currentTime); //signal that a new time index was received
	 updateGL();
 }

void GLWidget::override_toggled(bool value)
{
	colorOverride = value;
	updateGL();
}

void GLWidget::depthChecking_toggled(bool value)
{
	depthChecking = value;
	updateGL();
}

