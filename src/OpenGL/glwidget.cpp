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
	/*for (int i = 0; i<=255; i++)
	{	
		QColor c = rainbowScale[i];
		double r = (double)c.red() / 255;
		double g = (double)c.green() / 255;
		double b = (double)c.blue() / 255;
		glColor4d (r, g, b, 1.0);
		
		glRectd(-1.0, -1.0 + 2.0/255.0*i, -0.5, -1.0 + 2.0/255.0*(i+1));
	}*/

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
	/*
	rainbowScale[0] = QColor::fromRgb(0, 0, 0);
	rainbowScale[1] = QColor::fromRgb(45, 0, 36);
	rainbowScale[2] = QColor::fromRgb(56, 0, 46);
	rainbowScale[3] = QColor::fromRgb(60, 0, 49);
	rainbowScale[4] = QColor::fromRgb(67, 0, 54);
	rainbowScale[5] = QColor::fromRgb(70, 0, 59);
	rainbowScale[6] = QColor::fromRgb(71, 0, 61);
	rainbowScale[7] = QColor::fromRgb(75, 0, 68);
	rainbowScale[8] = QColor::fromRgb(74, 0, 73);
	rainbowScale[9] = QColor::fromRgb(74, 0, 77);
	rainbowScale[10] = QColor::fromRgb(73, 0, 81);
	rainbowScale[11] = QColor::fromRgb(71, 0, 87);
	rainbowScale[12] = QColor::fromRgb(69, 1, 90);
	rainbowScale[13] = QColor::fromRgb(68, 2, 94);
	rainbowScale[14] = QColor::fromRgb(66, 3, 97);
	rainbowScale[15] = QColor::fromRgb(63, 6, 102);
	rainbowScale[16] = QColor::fromRgb(61, 7, 106);
	rainbowScale[17] = QColor::fromRgb(58, 10, 109);
	rainbowScale[18] = QColor::fromRgb(56, 12, 113);
	rainbowScale[19] = QColor::fromRgb(53, 15, 116);
	rainbowScale[20] = QColor::fromRgb(48, 18, 119);
	rainbowScale[21] = QColor::fromRgb(47, 20, 121);
	rainbowScale[22] = QColor::fromRgb(44, 23, 124);
	rainbowScale[23] = QColor::fromRgb(41, 27, 128);
	rainbowScale[24] = QColor::fromRgb(40, 28, 129);
	rainbowScale[25] = QColor::fromRgb(37, 32, 132);
	rainbowScale[26] = QColor::fromRgb(34, 36, 134);
	rainbowScale[27] = QColor::fromRgb(29, 43, 137);
	rainbowScale[28] = QColor::fromRgb(25, 52, 138);
	rainbowScale[29] = QColor::fromRgb(24, 57, 139);
	rainbowScale[30] = QColor::fromRgb(24, 62, 141);
	rainbowScale[31] = QColor::fromRgb(24, 64, 142);
	rainbowScale[32] = QColor::fromRgb(23, 65, 142);
	rainbowScale[33] = QColor::fromRgb(23, 69, 143);
	rainbowScale[34] = QColor::fromRgb(23, 71, 142);
	rainbowScale[35] = QColor::fromRgb(23, 71, 142);
	rainbowScale[36] = QColor::fromRgb(23, 73, 142);
	rainbowScale[37] = QColor::fromRgb(23, 75, 142);
	rainbowScale[38] = QColor::fromRgb(23, 75, 142);
	rainbowScale[39] = QColor::fromRgb(23, 78, 142);
	rainbowScale[40] = QColor::fromRgb(23, 80, 142);
	rainbowScale[41] = QColor::fromRgb(23, 80, 142);
	rainbowScale[42] = QColor::fromRgb(23, 82, 141);
	rainbowScale[43] = QColor::fromRgb(23, 85, 141);
	rainbowScale[44] = QColor::fromRgb(23, 85, 141);
	rainbowScale[45] = QColor::fromRgb(23, 87, 140);
	rainbowScale[46] = QColor::fromRgb(23, 87, 140);
	rainbowScale[47] = QColor::fromRgb(24, 90, 140);
	rainbowScale[48] = QColor::fromRgb(24, 90, 140);
	rainbowScale[49] = QColor::fromRgb(24, 93, 139);
	rainbowScale[50] = QColor::fromRgb(24, 93, 139);
	rainbowScale[51] = QColor::fromRgb(24, 93, 139);
	rainbowScale[52] = QColor::fromRgb(24, 93, 139);
	rainbowScale[53] = QColor::fromRgb(24, 97, 139);
	rainbowScale[54] = QColor::fromRgb(24, 97, 139);
	rainbowScale[55] = QColor::fromRgb(25, 101, 138);
	rainbowScale[56] = QColor::fromRgb(25, 101, 138);
	rainbowScale[57] = QColor::fromRgb(25, 104, 137);
	rainbowScale[58] = QColor::fromRgb(25, 104, 137);
	rainbowScale[59] = QColor::fromRgb(25, 104, 137);
	rainbowScale[60] = QColor::fromRgb(26, 108, 137);
	rainbowScale[61] = QColor::fromRgb(26, 108, 137);
	rainbowScale[62] = QColor::fromRgb(27, 111, 136);
	rainbowScale[63] = QColor::fromRgb(27, 111, 136);
	rainbowScale[64] = QColor::fromRgb(27, 111, 136);
	rainbowScale[65] = QColor::fromRgb(27, 115, 135);
	rainbowScale[66] = QColor::fromRgb(27, 115, 135);
	rainbowScale[67] = QColor::fromRgb(28, 118, 134);
	rainbowScale[68] = QColor::fromRgb(28, 118, 134);
	rainbowScale[69] = QColor::fromRgb(29, 122, 133);
	rainbowScale[70] = QColor::fromRgb(29, 122, 133);
	rainbowScale[71] = QColor::fromRgb(29, 122, 133);
	rainbowScale[72] = QColor::fromRgb(29, 122, 133);
	rainbowScale[73] = QColor::fromRgb(29, 125, 132);
	rainbowScale[74] = QColor::fromRgb(29, 125, 132);
	rainbowScale[75] = QColor::fromRgb(30, 128, 131);
	rainbowScale[76] = QColor::fromRgb(30, 128, 131);
	rainbowScale[77] = QColor::fromRgb(31, 131, 130);
	rainbowScale[78] = QColor::fromRgb(31, 131, 130);
	rainbowScale[79] = QColor::fromRgb(31, 131, 130);
	rainbowScale[80] = QColor::fromRgb(32, 134, 128);
	rainbowScale[81] = QColor::fromRgb(32, 134, 128);
	rainbowScale[82] = QColor::fromRgb(33, 137, 127);
	rainbowScale[83] = QColor::fromRgb(33, 137, 127);
	rainbowScale[84] = QColor::fromRgb(33, 137, 127);
	rainbowScale[85] = QColor::fromRgb(34, 140, 125);
	rainbowScale[86] = QColor::fromRgb(34, 140, 125);
	rainbowScale[87] = QColor::fromRgb(35, 142, 123);
	rainbowScale[88] = QColor::fromRgb(35, 142, 123);
	rainbowScale[89] = QColor::fromRgb(36, 145, 121);
	rainbowScale[90] = QColor::fromRgb(36, 145, 121);
	rainbowScale[91] = QColor::fromRgb(36, 145, 121);
	rainbowScale[92] = QColor::fromRgb(37, 147, 118);
	rainbowScale[93] = QColor::fromRgb(37, 147, 118);
	rainbowScale[94] = QColor::fromRgb(38, 150, 116);
	rainbowScale[95] = QColor::fromRgb(38, 150, 116);
	rainbowScale[96] = QColor::fromRgb(40, 152, 113);
	rainbowScale[97] = QColor::fromRgb(40, 152, 113);
	rainbowScale[98] = QColor::fromRgb(41, 154, 111);
	rainbowScale[99] = QColor::fromRgb(41, 154, 111);
	rainbowScale[100] = QColor::fromRgb(42, 156, 108);
	rainbowScale[101] = QColor::fromRgb(42, 156, 108);
	rainbowScale[102] = QColor::fromRgb(43, 158, 106);
	rainbowScale[103] = QColor::fromRgb(43, 158, 106);
	rainbowScale[104] = QColor::fromRgb(43, 158, 106);
	rainbowScale[105] = QColor::fromRgb(45, 160, 104);
	rainbowScale[106] = QColor::fromRgb(45, 160, 104);
	rainbowScale[107] = QColor::fromRgb(46, 162, 101);
	rainbowScale[108] = QColor::fromRgb(46, 162, 101);
	rainbowScale[109] = QColor::fromRgb(48, 164, 99);
	rainbowScale[110] = QColor::fromRgb(48, 164, 99);
	rainbowScale[111] = QColor::fromRgb(50, 166, 97);
	rainbowScale[112] = QColor::fromRgb(50, 166, 97);
	rainbowScale[113] = QColor::fromRgb(51, 168, 95);
	rainbowScale[114] = QColor::fromRgb(53, 170, 93);
	rainbowScale[115] = QColor::fromRgb(53, 170, 93);
	rainbowScale[116] = QColor::fromRgb(53, 170, 93);
	rainbowScale[117] = QColor::fromRgb(55, 172, 91);
	rainbowScale[118] = QColor::fromRgb(55, 172, 91);
	rainbowScale[119] = QColor::fromRgb(57, 174, 88);
	rainbowScale[120] = QColor::fromRgb(57, 174, 88);
	rainbowScale[121] = QColor::fromRgb(59, 175, 86);
	rainbowScale[122] = QColor::fromRgb(62, 177, 84);
	rainbowScale[123] = QColor::fromRgb(64, 178, 82);
	rainbowScale[124] = QColor::fromRgb(64, 178, 82);
	rainbowScale[125] = QColor::fromRgb(67, 180, 80);
	rainbowScale[126] = QColor::fromRgb(67, 180, 80);
	rainbowScale[127] = QColor::fromRgb(69, 181, 79);
	rainbowScale[128] = QColor::fromRgb(72, 183, 77);
	rainbowScale[129] = QColor::fromRgb(72, 183, 77);
	rainbowScale[130] = QColor::fromRgb(72, 183, 77);
	rainbowScale[131] = QColor::fromRgb(75, 184, 76);
	rainbowScale[132] = QColor::fromRgb(77, 186, 74);
	rainbowScale[133] = QColor::fromRgb(80, 187, 73);
	rainbowScale[134] = QColor::fromRgb(83, 189, 72);
	rainbowScale[135] = QColor::fromRgb(87, 190, 72);
	rainbowScale[136] = QColor::fromRgb(91, 191, 71);
	rainbowScale[137] = QColor::fromRgb(95, 192, 70);
	rainbowScale[138] = QColor::fromRgb(99, 193, 70);
	rainbowScale[139] = QColor::fromRgb(103, 194, 70);
	rainbowScale[140] = QColor::fromRgb(107, 195, 70);
	rainbowScale[141] = QColor::fromRgb(111, 196, 70);
	rainbowScale[142] = QColor::fromRgb(111, 196, 70);
	rainbowScale[143] = QColor::fromRgb(115, 196, 70);
	rainbowScale[144] = QColor::fromRgb(119, 197, 70);
	rainbowScale[145] = QColor::fromRgb(123, 197, 70);
	rainbowScale[146] = QColor::fromRgb(130, 198, 71);
	rainbowScale[147] = QColor::fromRgb(133, 199, 71);
	rainbowScale[148] = QColor::fromRgb(137, 199, 72);
	rainbowScale[149] = QColor::fromRgb(140, 199, 72);
	rainbowScale[150] = QColor::fromRgb(143, 199, 73);
	rainbowScale[151] = QColor::fromRgb(143, 199, 73);
	rainbowScale[152] = QColor::fromRgb(147, 199, 73);
	rainbowScale[153] = QColor::fromRgb(150, 199, 74);
	rainbowScale[154] = QColor::fromRgb(153, 199, 74);
	rainbowScale[155] = QColor::fromRgb(156, 199, 75);
	rainbowScale[156] = QColor::fromRgb(160, 200, 76);
	rainbowScale[157] = QColor::fromRgb(167, 200, 78);
	rainbowScale[158] = QColor::fromRgb(170, 200, 79);
	rainbowScale[159] = QColor::fromRgb(173, 200, 79);
	rainbowScale[160] = QColor::fromRgb(173, 200, 79);
	rainbowScale[161] = QColor::fromRgb(177, 200, 80);
	rainbowScale[162] = QColor::fromRgb(180, 200, 81);
	rainbowScale[163] = QColor::fromRgb(183, 199, 82);
	rainbowScale[164] = QColor::fromRgb(186, 199, 82);
	rainbowScale[165] = QColor::fromRgb(190, 199, 83);
	rainbowScale[166] = QColor::fromRgb(196, 199, 85);
	rainbowScale[167] = QColor::fromRgb(199, 198, 85);
	rainbowScale[168] = QColor::fromRgb(199, 198, 85);
	rainbowScale[169] = QColor::fromRgb(203, 198, 86);
	rainbowScale[170] = QColor::fromRgb(206, 197, 87);
	rainbowScale[171] = QColor::fromRgb(212, 197, 89);
	rainbowScale[172] = QColor::fromRgb(215, 196, 90);
	rainbowScale[173] = QColor::fromRgb(218, 195, 91);
	rainbowScale[174] = QColor::fromRgb(224, 194, 94);
	rainbowScale[175] = QColor::fromRgb(224, 194, 94);
	rainbowScale[176] = QColor::fromRgb(230, 193, 96);
	rainbowScale[177] = QColor::fromRgb(233, 192, 98);
	rainbowScale[178] = QColor::fromRgb(236, 190, 100);
	rainbowScale[179] = QColor::fromRgb(238, 189, 104);
	rainbowScale[180] = QColor::fromRgb(240, 188, 106);
	rainbowScale[181] = QColor::fromRgb(240, 188, 106);
	rainbowScale[182] = QColor::fromRgb(242, 187, 110);
	rainbowScale[183] = QColor::fromRgb(244, 185, 114);
	rainbowScale[184] = QColor::fromRgb(245, 184, 116);
	rainbowScale[185] = QColor::fromRgb(247, 183, 120);
	rainbowScale[186] = QColor::fromRgb(248, 182, 123);
	rainbowScale[187] = QColor::fromRgb(248, 182, 123);
	rainbowScale[188] = QColor::fromRgb(250, 181, 125);
	rainbowScale[189] = QColor::fromRgb(251, 180, 128);
	rainbowScale[190] = QColor::fromRgb(252, 180, 130);
	rainbowScale[191] = QColor::fromRgb(253, 180, 133);
	rainbowScale[192] = QColor::fromRgb(253, 180, 133);
	rainbowScale[193] = QColor::fromRgb(254, 180, 134);
	rainbowScale[194] = QColor::fromRgb(254, 179, 138);
	rainbowScale[195] = QColor::fromRgb(255, 179, 142);
	rainbowScale[196] = QColor::fromRgb(255, 179, 145);
	rainbowScale[197] = QColor::fromRgb(255, 179, 145);
	rainbowScale[198] = QColor::fromRgb(255, 179, 152);
	rainbowScale[199] = QColor::fromRgb(255, 180, 161);
	rainbowScale[200] = QColor::fromRgb(255, 180, 164);
	rainbowScale[201] = QColor::fromRgb(255, 180, 167);
	rainbowScale[202] = QColor::fromRgb(255, 180, 167);
	rainbowScale[203] = QColor::fromRgb(255, 181, 169);
	rainbowScale[204] = QColor::fromRgb(255, 181, 170);
	rainbowScale[205] = QColor::fromRgb(255, 182, 173);
	rainbowScale[206] = QColor::fromRgb(255, 183, 176);
	rainbowScale[207] = QColor::fromRgb(255, 183, 176);
	rainbowScale[208] = QColor::fromRgb(255, 184, 179);
	rainbowScale[209] = QColor::fromRgb(255, 185, 179);
	rainbowScale[210] = QColor::fromRgb(255, 185, 182);
	rainbowScale[211] = QColor::fromRgb(255, 186, 182);
	rainbowScale[212] = QColor::fromRgb(255, 186, 182);
	rainbowScale[213] = QColor::fromRgb(255, 187, 185);
	rainbowScale[214] = QColor::fromRgb(255, 188, 185);
	rainbowScale[215] = QColor::fromRgb(255, 189, 188);
	rainbowScale[216] = QColor::fromRgb(255, 189, 188);
	rainbowScale[217] = QColor::fromRgb(255, 190, 188);
	rainbowScale[218] = QColor::fromRgb(255, 191, 191);
	rainbowScale[219] = QColor::fromRgb(255, 192, 191);
	rainbowScale[220] = QColor::fromRgb(255, 194, 194);
	rainbowScale[221] = QColor::fromRgb(255, 194, 194);
	rainbowScale[222] = QColor::fromRgb(255, 197, 197);
	rainbowScale[223] = QColor::fromRgb(255, 198, 198);
	rainbowScale[224] = QColor::fromRgb(255, 200, 200);
	rainbowScale[225] = QColor::fromRgb(255, 201, 201);
	rainbowScale[226] = QColor::fromRgb(255, 201, 201);
	rainbowScale[227] = QColor::fromRgb(255, 202, 202);
	rainbowScale[228] = QColor::fromRgb(255, 203, 203);
	rainbowScale[229] = QColor::fromRgb(255, 205, 205);
	rainbowScale[230] = QColor::fromRgb(255, 206, 206);
	rainbowScale[231] = QColor::fromRgb(255, 206, 206);
	rainbowScale[232] = QColor::fromRgb(255, 208, 208);
	rainbowScale[233] = QColor::fromRgb(255, 209, 209);
	rainbowScale[234] = QColor::fromRgb(255, 211, 211);
	rainbowScale[235] = QColor::fromRgb(255, 215, 215);
	rainbowScale[236] = QColor::fromRgb(255, 216, 216);
	rainbowScale[237] = QColor::fromRgb(255, 216, 216);
	rainbowScale[238] = QColor::fromRgb(255, 218, 218);
	rainbowScale[239] = QColor::fromRgb(255, 219, 219);
	rainbowScale[240] = QColor::fromRgb(255, 221, 221);
	rainbowScale[241] = QColor::fromRgb(255, 223, 223);
	rainbowScale[242] = QColor::fromRgb(255, 226, 226);
	rainbowScale[243] = QColor::fromRgb(255, 228, 228);
	rainbowScale[244] = QColor::fromRgb(255, 230, 230);
	rainbowScale[245] = QColor::fromRgb(255, 230, 230);
	rainbowScale[246] = QColor::fromRgb(255, 232, 232);
	rainbowScale[247] = QColor::fromRgb(255, 235, 235);
	rainbowScale[248] = QColor::fromRgb(255, 237, 237);
	rainbowScale[249] = QColor::fromRgb(255, 240, 240);
	rainbowScale[250] = QColor::fromRgb(255, 243, 243);
	rainbowScale[251] = QColor::fromRgb(255, 246, 246);
	rainbowScale[252] = QColor::fromRgb(255, 249, 249);
	rainbowScale[253] = QColor::fromRgb(255, 251, 251);
	rainbowScale[254] = QColor::fromRgb(255, 253, 253);
	rainbowScale[255] = QColor::fromRgb(255, 255, 255);*/
}









