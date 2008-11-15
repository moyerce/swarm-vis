#ifndef GLWIDGET_H
 #define GLWIDGET_H

#include <QGLWidget>
#include <QList>
#include <QListWidgetItem>
#include <string>
#include <vector>
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
	void setInput( std::vector<Agent> * agents, int maxTime);
 
     
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
	bool sameColor;

	double trackColorR;
	double trackColorG;
	double trackColorB;
	double trackColorO;
	QList<int> selectedTrackAgents;

	void buildTrail(int agentIndex, int length);
	void showSelectedPaths(int index);
	void paintAxis();
	void normalizeAngle(int *angle);

	QList<int> agentTypes;
	QList<double> agentTypeColorR;
	QList<double> agentTypeColorG;
	QList<double> agentTypeColorB;
	QList<double> agentTypeColorO;

	bool colorOverride;
	 


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
	void updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>);
	void showPathWithSameColor(bool value);
	void override_toggled(bool value);
	void depthChecking_toggled(bool value);
	
  signals:
      void timeIndexChanged(int timeIndex);
      
 };

 #endif