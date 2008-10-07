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
     void setInput( vector<Agent> * agents);

 public slots:
     void setTimeIndex(int timeIndex);
     
 signals:
     void timeIndexChanged(int timeIndex);
     
 protected:
     void initializeGL();
     void paintGL();
     /*need to change the following functions*/
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);

 private:
	 vector<Agent> *agent_array;
	 double maxSize;
	 int currentTime;
	 bool initialized;
     
 };

 #endif
