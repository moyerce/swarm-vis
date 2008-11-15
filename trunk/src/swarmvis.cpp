#include "swarmvis.h"
#include <string>
#include <vector>
#include <iostream>

SwarmVis::SwarmVis(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidget = new GLWidget(this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(glWidget);
	ui.groupBox->setLayout(layout);
	thread = NULL;
	typeSelectedItem = NULL;
	
	isLoaded = false;
	
	// always visible items	
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setTimeIndex(int)));
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), this, SLOT(setGroupBoxLabel(int)));
	// items on the TRAILS tab
	connect(ui.spinBoxTrailLength, SIGNAL(valueChanged(int)), glWidget, SLOT(setTrailLength(int)));
	// items on the TRACKS tab
	connect(ui.sliderR, SIGNAL(valueChanged(int)), this, SLOT(trackColorR_Changed()));
	connect(ui.sliderG, SIGNAL(valueChanged(int)), this, SLOT(trackColorG_Changed()));
	connect(ui.sliderB, SIGNAL(valueChanged(int)), this, SLOT(trackColorB_Changed()));
	connect(ui.sliderO, SIGNAL(valueChanged(int)), this, SLOT(trackColorO_Changed()));
	connect(this, SIGNAL(trackColorChanged(double, double, double, double)), glWidget, SLOT(setTrackColor(double, double, double, double)));
	connect(ui.chkAgentPaths, SIGNAL(toggled(bool)), glWidget, SLOT(toggleShowPaths(bool)));
	connect(ui.chkSameColor, SIGNAL(toggled(bool)), glWidget, SLOT(showPathWithSameColor(bool)));
	// items on the TYPES tab
	connect(ui.chkOverrideAgentColor, SIGNAL(toggled(bool)), glWidget, SLOT(override_toggled(bool)));
	connect(this, SIGNAL(updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>)),
		glWidget, SLOT(updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>)));
	// items on the AGENTS tab
	connect(this, SIGNAL(selectedTrackAgentsChanged(QList<int>)), glWidget, SLOT(selectedTrackAgentsChanged(QList<int>)));
	connect(ui.tableWidgetTracks, SIGNAL(itemSelectionChanged()), this, SLOT(trackSelectionChanged()));
	// items on the MISC tab
	connect(ui.chkBoundingBox, SIGNAL(toggled(bool)), glWidget, SLOT(boundingBox_toggled(bool)));
	connect(ui.chkDepthChecking, SIGNAL(toggled(bool)), glWidget, SLOT(depthChecking_toggled(bool)));
	
	setupUI();
}

SwarmVis::~SwarmVis()
{

}

void SwarmVis::btnPlay_clicked()
{	
	if (isLoaded)
	{
		if (thread == NULL || thread->isFinished ())
		{
			createThread();
			thread->start();
		}
		else
		{
			startSignal();
		}
	}
}

void SwarmVis::setupUI()
{
	
}

void SwarmVis::loadNewData()
{	
	ui.tableWidgetTracks->clear();
	ui.tableWidgetTracks->insertColumn(0);
	QStringList tracksHeader;
    tracksHeader << "Agents";
	ui.tableWidgetTracks->setHorizontalHeaderLabels(tracksHeader);
	ui.tableWidgetTracks->setRowCount(0);
	
	ui.listWidgetTypes->clear();
	ui.tableWidget->clear();
	ui.tableWidget->insertColumn(0);
	ui.tableWidget->insertColumn(1);	
	ui.tableWidget->insertColumn(2);	
	ui.tableWidget->insertColumn(3);
	QStringList headers;
    headers << "Agents" << "Color" << "Size" << "Glyph";
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	ui.tableWidget->setRowCount(0);
	typeList.clear();	
	agentTypeIndex.clear();
	agentIndex.clear();
	agentR.clear();
	agentG.clear();
	agentB.clear();
	agentO.clear();
	isLoaded = true;
}

void SwarmVis::menubar_action_handler(QAction* action)
{
	if (action->text().compare("&Quit") == 0)
	{
		exit(0);
	}
	// Action handler for loading data
	if (action->text().compare("&Load Agent Data") == 0)
	{
		// Prompt user for an .info File		
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
							"/home/workspace5/3DSwarmData/",
							tr("Info (*.info);;Text file (*.txt);;XML files (*.xml);; All File (*.*)"));

		if (fileName.length() > 0)
		{
			loadNewData();
			std::string str =  fileName.toStdString();
			size_t found;		
			found = str.find_last_of("/");
			std::string directory = str.substr(0, found+1);
			std::string filename = str.substr(found + 1);
			
			// Initialize the Agents
			agents = new AgentArray(directory, filename);
			ui.timeSlider->setMaximum(agents->getTimeSteps() - 1);
			std::vector<Agent> * v = agents->getAgentsVector();
			glWidget->setInput(v, agents->getTimeSteps() - 1);
			
			// Initialize the GUI components
			populateListView();
		}
	}	
}

void SwarmVis::threadFinished()
{
	ui.timeSlider->setValue(0);
	if (ui.chkContinuousPlay->isChecked())
	{
		btnPlay_clicked();
	}
}

void SwarmVis::createThread()
{	
	double DELAY = ui.dSpinBoxDelay->value();
	thread = new MyThread(this, agents->getTimeSteps() - 1, DELAY);
	connect(thread, SIGNAL(playSignal()), ui.timeSlider, SLOT(addStep()));
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), thread, SLOT(setTimeIndex(int)));
	connect(ui.dSpinBoxDelay, SIGNAL(valueChanged(double)), thread, SLOT(setDelay(double)));
	connect(ui.btnStop, SIGNAL(clicked()), thread, SLOT(startStopThread()));
	connect(this, SIGNAL(startSignal()), thread, SLOT(startStopThread()));
	connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));	

}

void SwarmVis::btnSelectAll_clicked()
{
	/*QItemSelectionModel * selectionModel = ui.listWidgetAgents->selectionModel();
	for(int i = 0; i < agents->getNumAgents(); i++)
	{
		const QModelIndex *index = new QModelIndex;
		selectionModel->select(index, QItemSelectionModel::SelectCurrent);
	}*/
}

void SwarmVis::btnClearSelection_clicked()
{
	QItemSelectionModel * selectionModel = ui.tableWidgetTracks->selectionModel();
	selectionModel->clearSelection();
}


void SwarmVis::populateListView()
{
	for (int i = 0; i < agents->getNumAgents(); i++)
	{	
		std::string s;
		std::stringstream out;
		out << "Agent" << i;
		s = out.str();				

		//the path of agents view	
		//QListWidgetItem * item = new QListWidgetItem;		
		//item->setText(s.c_str());
		//item->setData(Qt::ToolTipRole, i);	
		//ui.listWidgetAgents->addItem(item);

		QTableWidgetItem *item = new QTableWidgetItem(s.c_str());
		int r = ui.tableWidgetTracks->rowCount();
		ui.tableWidgetTracks->insertRow(r);
		ui.tableWidgetTracks->setItem(r, 0, item);

		//the table view
		QTableWidgetItem *tItem0 = new QTableWidgetItem(s.c_str());
		QTableWidgetItem *tItem1 = new QTableWidgetItem("");
		QColor color = QColor::fromRgbF(0.0, 0.0, 1.0, 1.0);
		tItem1->setBackgroundColor(color);
		QTableWidgetItem *tItem2 = new QTableWidgetItem("3.0");
		QTableWidgetItem *tItem3 = new QTableWidgetItem("1");
		int row = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row);
		ui.tableWidget->setItem(row, 0, tItem0);
		ui.tableWidget->setItem(row, 1, tItem1);
		ui.tableWidget->setItem(row, 2, tItem2);
		ui.tableWidget->setItem(row, 3, tItem3);

		//the type view
		std::vector<Agent> * agents_array = agents->getAgentsVector();
		for (int j = 0; j < agents->getTimeSteps(); j++)
		{
			if (!typeList.contains(agents_array[j][i].getType()))
			{
				typeList.append(agents_array[j][i].getType());
			}
		}		
	}
	
	//Populate the list of Types with the different types
	for (int i = 0; i < typeList.size(); i++) 
	{ 
		QListWidgetItem * item = new QListWidgetItem;
		std::string s;
		std::stringstream out;		
		out << typeList.at(i);
		s = out.str();		
		item->setText(s.c_str());
		ui.listWidgetTypes->addItem(item);
	}
	ui.tableWidget->resizeColumnsToContents();
}

void SwarmVis::trackSelectionChanged()
{
	QList<QTableWidgetItem*> selectedAgents = ui.tableWidgetTracks->selectedItems();
	QList<int> list;
	for (int i = 0; i < selectedAgents.size(); i++)
	{
		QTableWidgetItem* item = selectedAgents.at(i);
		int index = item->row();

		if (!list.contains(index))
		{
			list.append(index);
			//std::cout<<index<<std::endl;
		}
		
	}
	selectedTrackAgentsChanged(list);
}

void SwarmVis::typeSelectionChanged()
{
	QList<QListWidgetItem*> selectedType = ui.listWidgetTypes->selectedItems();
	typeSelectedItem = selectedType.at(0);
	//QColor c = typeSelectedItem->backgroundColor();
	//bool * temp;
	//int index = item->data(Qt::ToolTipRole).toInt(temp);	
}

void SwarmVis::agentSelectionChanged()
{
	agentIndex.clear();
	QList<QTableWidgetItem*> selectedAgents = ui.tableWidget->selectedItems();
	for (int i = 0; i < selectedAgents.size(); i++)
	{
		QTableWidgetItem* item = selectedAgents.at(i);
		int index = item->row();
		if (!agentIndex.contains(index))
		{
			agentIndex.append(index);
			//std::cout<<index<<std::endl;
		}
	}
}

void SwarmVis::btnSetColor_clicked()
{
	if (typeSelectedItem!=NULL)
	{
		std::cout << "A" << std::endl;
	
		QColor color = QColorDialog::getColor(typeSelectedItem->backgroundColor(), this );	
		typeSelectedItem->setBackgroundColor(color);
		double r = color.redF();
		double g = color.greenF();
		double b = color.blueF();
		double o = color.alphaF();

		std::cout << "B" << std::endl;

		
		bool * temp;
		
		QVariant qv = typeSelectedItem->data(Qt::DisplayRole);
		
		std::cout << "Can we convert this to an int?" <<   qv.canConvert<int>() << std::endl;
		
		int val = qv.toInt(temp);		

		std::cout << "C" << std::endl;


		if (!agentTypeIndex.contains(val))
		{		
			agentTypeIndex.append(val);
			agentR.append(r);
			agentG.append(g);
			agentB.append(b);
			agentO.append(o);
		}
		else
		{		
			agentR.replace(agentTypeIndex.indexOf(val),r);
			agentG.replace(agentTypeIndex.indexOf(val),g);
			agentB.replace(agentTypeIndex.indexOf(val),b);
			agentO.replace(agentTypeIndex.indexOf(val),o);			
		}
		
		std::cout << "D" << std::endl;

		
		updateAgentTypesColor(agentTypeIndex, agentR, agentG, agentB, agentO);			
	}
	
	std::cout << "E" << std::endl;

}
void SwarmVis::btnSetColor_2_clicked()
{
//sets the color for each individual agent that is selected
	if (agentIndex.size() > 0)
	{
		QTableWidgetItem * first = ui.tableWidget->item(agentIndex.at(0), 1);
		QColor color = QColorDialog::getColor(first->backgroundColor(), this );
		
		std::vector<Agent> *agent_array = agents->getAgentsVector();
				
		for (int i = 0; i < agentIndex.size(); i++)
		{
			int aIndex = agentIndex.at(i);
			//set the items background color
			QTableWidgetItem * item = ui.tableWidget->item(aIndex, 1);
			item->setBackgroundColor(color);
			
			for (int j = 0; j < agents->getTimeSteps(); j++)
			{
				//set the agents color
				agent_array[j][aIndex].setColor(color);
			}
		}
		//glWidget->setInput(agent_array,agents->getTimeSteps());
	}
}
void SwarmVis::btnSetAgentSize_clicked()
{
//sets the size for each individual agent that is selected
	if (agentIndex.size() > 0)
	{	
		std::vector<Agent> *agent_array = agents->getAgentsVector();
				
		for (int i = 0; i < agentIndex.size(); i++)
		{
			int aIndex = agentIndex.at(i);
			//set the items size
			QTableWidgetItem * item = ui.tableWidget->item(aIndex, 2);
			
			item->setText(QString::number(ui.dSpinBoxAgentSize->value(),'g', 2));

			for (int j = 0; j < agents->getTimeSteps(); j++)
			{
				//set the agents size
				agent_array[j][aIndex].setSize(ui.dSpinBoxAgentSize->value());
			}
		}
		//glWidget->setInput(agent_array,agents->getTimeSteps());
	}
}

void SwarmVis::setGroupBoxLabel(int frame)
{
	QString s("OpenGL Window - Frame ");
	s.append(QString::number(frame, 10));
	ui.groupBox->setTitle(s);
}

void SwarmVis::trackColorR_Changed()
{	
	double r = (double)ui.sliderR->value() / (double)ui.sliderR->maximum();
	double g = (double)ui.sliderG->value() / (double)ui.sliderG->maximum();
	double b = (double)ui.sliderB->value() / (double)ui.sliderB->maximum();
	double o = (double)ui.sliderO->value() / (double)ui.sliderO->maximum();
	trackColorChanged(r, g, b, o);
}
void SwarmVis::trackColorG_Changed()
{
	double r = (double)ui.sliderR->value() / (double)ui.sliderR->maximum();
	double g = (double)ui.sliderG->value() / (double)ui.sliderG->maximum();
	double b = (double)ui.sliderB->value() / (double)ui.sliderB->maximum();
	double o = (double)ui.sliderO->value() / (double)ui.sliderO->maximum();
	trackColorChanged(r, g, b, o);
}
void SwarmVis::trackColorB_Changed()
{
	double r = (double)ui.sliderR->value() / (double)ui.sliderR->maximum();
	double g = (double)ui.sliderG->value() / (double)ui.sliderG->maximum();
	double b = (double)ui.sliderB->value() / (double)ui.sliderB->maximum();
	double o = (double)ui.sliderO->value() / (double)ui.sliderO->maximum();
	trackColorChanged(r, g, b, o);
}
void SwarmVis::trackColorO_Changed()
{
	double r = (double)ui.sliderR->value() / (double)ui.sliderR->maximum();
	double g = (double)ui.sliderG->value() / (double)ui.sliderG->maximum();
	double b = (double)ui.sliderB->value() / (double)ui.sliderB->maximum();
	double o = (double)ui.sliderO->value() / (double)ui.sliderO->maximum();
	trackColorChanged(r, g, b, o);
}





