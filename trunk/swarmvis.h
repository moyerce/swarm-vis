#ifndef SWARMVIS_H
#define SWARMVIS_H

#include <QtGui/QMainWindow>
#include "ui_swarmvis.h"
#include "Thread/mythread.h"
#include "OpenGL/glwidget.h"
#include "Agent/AgentArray.h"
#include "Agent/Agent.h"
#include <qfiledialog.h>


class SwarmVis : public QMainWindow
{
    Q_OBJECT

public:
    SwarmVis(QWidget *parent = 0);
    ~SwarmVis();

signals:
	void playSignal();
       
private slots:
	void btnPlay_clicked();
	void btnStop_clicked();
	void menubar_action_handler(QAction* action);
	    
private:
    Ui::SwarmVisClass ui;
    MyThread *thread;
    GLWidget *glWidget;
    AgentArray *agents;
};

#endif // SWARMVIS_H
