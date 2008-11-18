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
#include <QColorDialog>
#include <QDialog>
#include <string>
#include <vector>
#include <iostream>

class SwarmVis : public QMainWindow
{
    Q_OBJECT

public:
	SwarmVis(QWidget *parent = 0);
	~SwarmVis();

signals:
	void playSignal();
	void startSignal();
	void selectedTrackAgentsChanged(QList<int>);
	void selectedTypesAgentsChanged(QList<std::string>);
	void updateAgentTypesColor(QList<std::string>, QList<QColor>);
	void trackColorChanged(double, double, double, double);

private slots:
	void btnPlay_clicked();
	void btnClearSelection_clicked();
	void btnSelectAll_clicked();
	void threadFinished();
	void menubar_action_handler(QAction* action);
	void trackSelectionChanged();
	void typeSelectionChanged();
	void agentSelectionChanged();
	void btnSetColor_clicked();
	void btnSetColor_2_clicked();
	void btnSetAgentSize_clicked();
	void setGroupBoxLabel(int frame);	
	void trackColorR_Changed();
	void trackColorG_Changed();
	void trackColorB_Changed();
	void trackColorO_Changed();
	void btnSetGlyph_clicked();
	void btnSaveFrame_clicked();

private:
	Ui::SwarmVisClass ui;
	MyThread *thread;
	GLWidget *glWidget;
	AgentArray *agents;
	//QList<std::string> typeList;	
	//QList<std::string> agentTypeIndex;
	QList<std::string> typeIndex;
	QList<int> agentIndex;
	QList<double> agentR;
	QList<double> agentG;
	QList<double> agentB;
	QList<double> agentO;
	bool isLoaded;

	void createThread();
	void populateListView();
	void loadNewData();
	void setupUI();
};

#endif // SWARMVIS_H
