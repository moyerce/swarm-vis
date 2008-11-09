#ifndef SWARMVIS_H
#define SWARMVIS_H

#include <QtGui/QMainWindow>
#include "ui_swarmvis.h"
#include "Thread/mythread.h"
#include "OpenGL/glwidget.h"
#include "Agent/AgentArray.h"
#include "Agent/Agent.h"
#include <qfiledialog.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <sstream>


class SwarmVis : public QMainWindow
{
    Q_OBJECT

public:
    SwarmVis(QWidget *parent = 0);
    ~SwarmVis();

signals:
	void playSignal();
    void startSignal();
	void selectedAgentsChanged(QList<QListWidgetItem*>);
	void updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>);
       
private slots:
	void btnPlay_clicked();
	void btnClearSelection_clicked();
	void btnSelectAll_clicked();
	void threadFinished();
	void menubar_action_handler(QAction* action);
	void listSelectionChanged();
	void typeSelectionChanged();
	void btnSetColor_clicked();
	    
private:
    Ui::SwarmVisClass ui;
    MyThread *thread;
    GLWidget *glWidget;
    AgentArray *agents;
	QList<int> typeList;
	QListWidgetItem *typeSelectedItem;
	QList<int> agentTypeIndex;
	QList<double> agentR;
	QList<double> agentG;
	QList<double> agentB;
	QList<double> agentO;

    void createThread();
	void populateListView();
};

#endif // SWARMVIS_H
