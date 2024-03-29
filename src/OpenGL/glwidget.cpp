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
	showAgentVelocity = false;
	showTrackVelocity = false;
	initRainbowScale();
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
				//set the color according to type
				glPointSize(a.getSize());
				int index = agentTypes.indexOf(a.getType(), 0);
				//get the color as it is selected by the user for the type
				QColor c = agentTypeColors.at(index);
				double r = (double)c.red() / 255;
				double g = (double)c.green() / 255;
				double b = (double)c.blue() / 255;
				double o = (double)c.alpha() / 255;
				
				if (showAgentVelocity) makeSpeedOfAgentColor(i , currentTime, o);
				else glColor4d (r, g, b, o);
				drawAgent(a);

				if (showPaths)
				{
					if (sameColor) showSelectedPaths(i, r, g, b, trackColorO, showTrackVelocity);
					else showSelectedPaths(i, trackColorR, trackColorG, trackColorB, trackColorO, showTrackVelocity);
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
				
				if (showAgentVelocity) makeSpeedOfAgentColor(i , currentTime, o);
				else glColor4d (r, g, b, o);
				drawAgent(a);

				if (showPaths)
				{
					if (sameColor) showSelectedPaths(i, r, g, b, trackColorO, showTrackVelocity);
					else showSelectedPaths(i, trackColorR, trackColorG, trackColorB, trackColorO, showTrackVelocity);
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
			dumpImagePath +
			dumpImageFile +	
			(QString::number(currentTime)).rightJustified(8, '0') +
			QString(".png");
		// save the image to a file
		qimg.save(path, "png");
	}
}

void GLWidget::drawAgent(Agent a)
{
	glBegin (GL_POINTS);
		glVertex3d (a.getX(), a.getY(), a.getZ());
	glEnd ();
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

void GLWidget::setInput(std::vector<Agent> * agents, int maxTime, double maxDistance)
{
	agent_array = agents;
	maxTimeIndex = maxTime;
	this->maxDistance = maxDistance;
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
	for(int i = 0; i < length; i++)
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

			if (showAgentVelocity)
			{
				c = getSpeedColor(agentIndex, currentTime);
			}
			else if ( agentTypes.contains(a.getType()) && colorOverride )
			{
				int index = agentTypes.indexOf(a.getType(), 0);
				c = agentTypeColors.at(index);
			}
			else
			{
				c = a.getColor(); //set the agents color
			}

			r = (double)c.red() / 255;
			g = (double)c.green() / 255;
			b = (double)c.blue() / 255;				


			glColor4d (r, g, b, 0.7 - 0.7*(double)i / (double)length);
			glBegin (GL_LINES);
				glVertex3d ( a.getX(),  a.getY(), a.getZ());
				glVertex3d ( ab.getX(),  ab.getY(), ab.getZ());
			glEnd ();
		}
	}
}

void GLWidget::showSelectedPaths(int index, double r, double g, double b, double o, bool showSpeedColor)
{
	glColor4d (r, g, b, o);

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
				//determine the color of speed is indicated
				if (showSpeedColor) makeSpeedOfAgentColor(index , j, o);

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

void GLWidget::imageFolder_changed(const QString &fileName)
{
	//dumpImageFile = txt;
	std::string str =  fileName.toStdString();
	size_t found;
	found = str.find_last_of("/");
	std::string directory = str.substr(0, found + 1);
	std::string filename = str.substr(found + 1);
	if (filename.find_last_of(".") != std::string::npos)
	{
		size_t index = filename.find_last_of(".");
		filename = filename.substr(0, index);
	}
	dumpImageFile = filename.c_str();
	dumpImagePath = directory.c_str();

}

QColor GLWidget::getSpeedColor(int agent, int time)
{
	Agent a;
	if (time == 0) a = agent_array[time + 1][agent];
	else a = agent_array[time][agent];
	double d = a.getDistance();

	int cIndex = (int)((255.0 * d) / maxDistance);
	
	QColor c = rainbowScale[255 - cIndex];

	return c;
}

void GLWidget::makeSpeedOfAgentColor(int agent, int time, double o)
{
	QColor c = getSpeedColor(agent, time);
	double r = (double)c.red() / 255;
	double g = (double)c.green() / 255;
	double b = (double)c.blue() / 255;
	glColor4d (r, g, b, o);
}

void GLWidget::agentVelocity_toggled(bool value)
{
	showAgentVelocity = value;
	updateGL();
}

void GLWidget::trackVelocity_toggled(bool value)
{
	showTrackVelocity = value;
	updateGL();
}

void GLWidget::initRainbowScale()
{
	for (int i = 0; i <= 125; i++)
	{
		rainbowScale[i] = QColor::fromRgb(255, 0, i * 2);		
		rainbowScale[i + 126] = QColor::fromRgb(255 - i *2, 0, 255);
	}	
}









