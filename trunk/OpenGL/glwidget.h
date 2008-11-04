#ifndef GLWIDGET_H
 #define GLWIDGET_H

#include <QGLWidget>
#include <string>
#include <vector>
#include "Agent/AgentArray.h"
#include "Agent/Agent.h"

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

 private:
	 std::vector<Agent> *agent_array;
	 double maxSize;
	 int maxTimeIndex;
	 int currentTime;
	 bool initialized;
	 int xRot;
     int yRot;
     int zRot;
     double pointSize;
     int trailLength;
     QPoint lastPos;
     
     void buildTrail(int length);
     void showPath();
     void paintAxis();
	 void normalizeAngle(int *angle);	 


 public slots:
      void setTimeIndex(int timeIndex);
      void setXRotation(int angle);
      void setYRotation(int angle);
      void setZRotation(int angle);
      void setPointSize(double size);
      void setTrailLength(int length);

      
  signals:
      void timeIndexChanged(int timeIndex);
      
 };

 #endif
