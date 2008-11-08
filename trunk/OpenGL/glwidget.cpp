#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
 

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(parent)
 {
	//maxSize = 800; //the maximum range HARD-CODED
	initialized = false;

	xRot = 0;
    yRot = 0;
    zRot = 0;
    pointSize = 3.0;
    trailLength = 0;

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
	//glTranslated(-300.0, 0.0, 0.0);
	glScaled(400.0, 400.0, 0.0);
	//glScaled(800.0, 800.0, 800.0);
	
	//rotate the view
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	glPointSize(pointSize);
	paintAxis();

	if (initialized)
	{
		
		buildTrail(trailLength);
		//showPath();
		
		glColor3d (0.0, 0.0, 1.0); //set color to blue
		
	    std::vector<Agent> currentAgents = agent_array[currentTime];
		std::vector<Agent>::iterator iter; //the iterator for the agent vector
		iter = currentAgents.begin(); //set the start of the iterator
				
		glBegin (GL_POINTS);	
		while( iter != currentAgents.end() )
		{
		      Agent a = *iter;
	     
		      glVertex3d ( a.getX(),  a.getY(), a.getZ());
		      //glVertex3d (translatePoint(a.getX(),maxSize),  translatePoint(a.getY(),maxSize), translatePoint(a.getZ(),maxSize));		      
		      iter++;
		}
		glEnd ();
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
	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	 glDisable(GL_BLEND);
 }
 void GLWidget::showPath()
 {
	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 glPointSize(1);
	 for(int i = 0; i < maxTimeIndex; i++)
	 {		 
		 std::vector<Agent> currentAgents = agent_array[i];
	 	 std::vector<Agent>::iterator iter; //the iterator for the agent vector
	 	 iter = currentAgents.begin(); //set the start of the iterator
	 		
	 	 //glColor3d (1.0, 1.0, 1.0); //set color to blue
	 	 glColor4d (0.5, 0.0, 0.0, 0.5);
	 	 	
	 	 glBegin (GL_POINTS);	
	 	 while( iter != currentAgents.end() )
	 	 {
	 	 	Agent a = *iter;		     
	 	 	glVertex3d ( a.getX(),  a.getY(), a.getZ());
	 	 	iter++;
	 	 }
	 	 glEnd ();	 	 
	 }
	 glPointSize(pointSize);
	 glDisable(GL_BLEND);
 }


