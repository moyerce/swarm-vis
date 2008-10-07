#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
 

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(parent)
 {
	maxSize = 800; //the maximum range HARD-CODED
	initialized = false;
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
     return QSize(800, 600);
 }

 void GLWidget::setTimeIndex(int timeIndex)
 {   
	 cout << "TimeIndex:"<< timeIndex << endl;
	 currentTime = timeIndex;
	 initialized = true;
	
	 timeIndexChanged(currentTime); //signal that a new time index was received
	 updateGL();
 }

void GLWidget::initializeGL ()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable (GL_DEPTH_TEST);
	glPointSize(5.0);
	glEnable(GL_POINT_SMOOTH);

}

void  GLWidget::paintGL()
{	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	glTranslated (0.0, 0.0, 0.0);
	glColor3d (0.0, 0.0, 1.0); //set color to blue
	glScaled (300.0, 300.0, 300.0);

	if (initialized)
	{
	        vector<Agent> currentAgents = agent_array[currentTime];
		vector<Agent>::iterator iter; //the iterator for the agent vector
		iter = currentAgents.begin(); //set the start of the iterator
		glBegin (GL_POINTS);	
		while( iter != currentAgents.end() )
		{
		      Agent a = *iter;
	     
		      glVertex3d ( a.getX()/maxSize,  a.getY()/maxSize, a.getZ()/maxSize);
		      iter++;
		}
		glEnd ();
	}

	/*glBegin (GL_POINTS);
	double x1 = 792.187751302;
	double y1 = 628.139687028;
	double x2 = 262.167912924;
	double y2 = 319.166205559;
	glVertex3d (-x1/maxSize, -y1/maxSize, 0.0);
	glVertex3d (x2/maxSize, -y2/maxSize, 0.0);
	glVertex3d (0.0, 1.0, 0.0);
	glEnd ();*/
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
    //lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
   /* int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();*/
}

void GLWidget::setInput(vector<Agent> * agents)
{
	agent_array = agents; 
}
