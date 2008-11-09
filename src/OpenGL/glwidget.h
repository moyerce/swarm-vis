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
	int COLORDIVISOR;
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

	double agentComponentR;
	double agentComponentG;
	double agentComponentB;
	double agentComponentO;

	double individualAgentComponentR;
	double individualAgentComponentG;
	double individualAgentComponentB;
	double individualAgentComponentO;
	QList<QListWidgetItem*> selectedAgents;

	void buildTrail(int length);
	void showSelectedPaths();
	void paintAxis();
	void normalizeAngle(int *angle);

	QList<int> agentTypes;
	QList<double> agentTypeColorR;
	QList<double> agentTypeColorG;
	QList<double> agentTypeColorB;
	QList<double> agentTypeColorO;
	 


 public slots:
	void setTimeIndex(int timeIndex);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setPointSize(double size);
	void setTrailLength(int length);
	void toggleShowPaths(bool toggle);
	void agentsColorValueR(int value);
	void agentsColorValueG(int value);
	void agentsColorValueB(int value);
	void agentsColorValueO(int value);
	void individualAgentsColorValueR(int value);
	void individualAgentsColorValueG(int value);
	void individualAgentsColorValueB(int value);
	void individualAgentsColorValueO(int value);
	void selectedAgentsChanged(QList<QListWidgetItem*> selection);
	void boundingBox_toggled(bool value);	
	void updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>);
	
  signals:
      void timeIndexChanged(int timeIndex);
      
 };

 #endif
