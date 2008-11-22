#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include <QGLWidget>
#include <QList>
#include <QListWidgetItem>
#include <string>
#include <vector>
#include <iostream>
#include "src/Agent/AgentArray.h"
#include "src/Agent/Agent.h"

class GLWidget : public QGLWidget
{
     Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setInput( std::vector<Agent> * agents, int maxTime, double maxDistance);

protected:
	void initializeGL();
	void paintGL();
	/*need to change the following functions*/
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	std::vector<Agent> *agent_array;
	double maxSize;
	int maxTimeIndex;
	int currentTime;
	bool initialized;
	double zoom;
	int xRot;
	int yRot;
	int zRot;
	double pointSize;
	int trailLength;
	QPoint lastPos;
	bool showPaths;
	bool showBoundingBox;
	bool depthChecking;
	bool showAgentVelocity;
	bool showTrackVelocity;
	bool dumpImage;
	QString dumpImageFile;
	QString dumpImagePath;
	bool sameColor;
	double trackColorR;
	double trackColorG;
	double trackColorB;
	double trackColorO;
	QList<int> selectedTrackAgents;
	QList<std::string> agentTypes;
	QList<QColor> agentTypeColors;
	bool colorOverride;
	double maxDistance;
	QColor rainbowScale[256];

	void buildTrail(int agentIndex, int length);
	void showSelectedPaths(int index, double r, double g, double b, double o, bool showSpeed);
	void paintAxis();
	void normalizeAngle(int *angle);
	void makeSpeedOfAgentColor(int agent, int time, double o);
	void drawAgent(Agent a);
	void initRainbowScale();

public slots:
	void setTimeIndex(int timeIndex);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);	
	void setTrailLength(int length);
	void toggleShowPaths(bool toggle);	
	void setTrackColor(double r, double g, double b, double o);
	void selectedTrackAgentsChanged(QList<int> selection);
	void boundingBox_toggled(bool value);	
	void updateAgentTypesColor(QList<std::string>, QList<QColor>);
	void showPathWithSameColor(bool value);
	void override_toggled(bool value);
	void depthChecking_toggled(bool value);
	void dumpImage_toggled(bool value);
	void imageFolder_changed(const QString &txt);
	void agentVelocity_toggled(bool);
	void trackVelocity_toggled(bool);
	
signals:
      void timeIndexChanged(int timeIndex);
};

#endif
