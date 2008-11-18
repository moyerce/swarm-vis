#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
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
	depthChecking = false;
	dumpImage = false;
	sameColor = false;
	colorOverride = false;
	trackColorR = 0.0;
	trackColorG = 0.0;
	trackColorB = 0.0;
	trackColorO = 0.35;
}

void GLWidget::initializeGL()
{
	glClearColor (1.0, 1.0, 1.0, 0.0);	
	glEnable(GL_POINT_SMOOTH);
}

void  GLWidget::paintGL()
{
	if (depthChecking) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
				
				int index = agentTypes.indexOf(a.getType(), 0);
				QColor c = agentTypeColors.at(index);
				//double r = agentTypeColorR.at(agentTypes.indexOf(a.getType()));
				//double g = agentTypeColorG.at(agentTypes.indexOf(a.getType()));
				//double b = agentTypeColorB.at(agentTypes.indexOf(a.getType()));
				//double o = agentTypeColorO.at(agentTypes.indexOf(a.getType()));
				double r = (double)c.red() / 255;
				double g = (double)c.green() / 255;
				double b = (double)c.blue() / 255;
				double o = (double)c.alpha() / 255;

				glColor4d (r, g, b, o);

				glBegin (GL_POINTS);
					glVertex3d (a.getX(), a.getY(), a.getZ());
				glEnd ();
				if (showPaths)
				{
					if (sameColor) glColor4d (r, g, b, trackColorO);
					else glColor4d(trackColorR, trackColorG, trackColorB, trackColorO);
					
					showSelectedPaths(i);
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
					glVertex3d (a.getX(), a.getY(), a.getZ());
				glEnd ();
				if (showPaths)
				{
					if (sameColor) glColor4d (r, g, b, trackColorO);
					else glColor4d(trackColorR, trackColorG, trackColorB, trackColorO);
					
					showSelectedPaths(i);
				}
				buildTrail(i, trailLength);
			}
			iter++;
			i++;
		}
	}
	
	if(dumpImage)
	{
		// dump the frame to an image
		QImage qimg = this->grabFrameBuffer();


		// get the file path
		QString path =
			dumpImageFolderPath +
			QString("/frame") +
			(QString::number(currentTime)).rightJustified(8, '0') +
			QString(".png");

		// save the image to a file
		qimg.save(path, "png");	
	}
}

void GLWidget::paintAxis()
{
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
		// don't let it zoom so far out it starts zooming back in
		if (zoom < 0)
		{
			zoom = 0;
		}
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

	if (event->buttons() & Qt::LeftButton)
	{
        	setXRotation(xRot + 8 * dy);
        	setYRotation(yRot + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton)
	{
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
	if (angle != xRot)
	{
		xRot = angle;
		updateGL();
	}
}

void GLWidget::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != yRot)
	{
		yRot = angle;
		updateGL();
	}
}

void GLWidget::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != zRot)
	{
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
	for(int i = 0; i <= length; i++)
	{
	int k = currentTime - i;
		if (k >= 0)
		{
			int k = currentTime - i;
			Agent a = agent_array[k][agentIndex];
			Agent ab;
			if (k != 0)
	 			ab = agent_array[k - 1][agentIndex];
			else
				ab = a;

			double r, g, b;
			QColor c;

			if ( agentTypes.contains(a.getType()) && colorOverride )
			{
				int index = agentTypes.indexOf(a.getType(), 0);
				c = agentTypeColors.at(index);
				//c = a.getColor(); //set the agents color
				r = (double)c.red() / 255;
				g = (double)c.green() / 255;
				b = (double)c.blue() / 255;
				//o = (double)c.alpha() / 255;
			}
			else
			{
				c = a.getColor(); //set the agents color
				r = (double)c.red() / 255;
				g = (double)c.green() / 255;
				b = (double)c.blue() / 255;
			}

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
	glBegin(GL_LINES);	
	for (int i = 0; i < selectedTrackAgents.size(); i++)
	{
		int agent = selectedTrackAgents.at(i);
		if (index == agent)
		{
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

void GLWidget::updateAgentTypesColor(QList<std::string> type, QList<QColor> color)
{
	agentTypes = type;
	agentTypeColors = color;
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

void GLWidget::dumpImage_toggled(bool value)
{
	dumpImage = value;
}

void GLWidget::imageFolder_changed(const QString &txt)
{
	dumpImageFolderPath = txt;
}