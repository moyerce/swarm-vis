#include "swarmvis.h"
#include <QLayout>

SwarmVis::SwarmVis(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidget = new GLWidget(this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(glWidget);
	ui.groupBox->setLayout(layout);
	
	thread = NULL;
	//typeSelectedItem = NULL;
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
	connect(ui.tableWidgetTypes, SIGNAL(itemSelectionChanged()), this, SLOT(typeSelectionChanged()));
	connect(this, SIGNAL(updateAgentTypesColor(QList<std::string>, QList<QColor>)), glWidget, SLOT(updateAgentTypesColor(QList<std::string>, QList<QColor>)));
	// items on the AGENTS tab
	connect(this, SIGNAL(selectedTrackAgentsChanged(QList<int>)), glWidget, SLOT(selectedTrackAgentsChanged(QList<int>)));
	connect(ui.tableWidgetTracks, SIGNAL(itemSelectionChanged()), this, SLOT(trackSelectionChanged()));
	// items on the MISC tab
	connect(ui.chkBoundingBox, SIGNAL(toggled(bool)), glWidget, SLOT(boundingBox_toggled(bool)));
	connect(ui.chkDepthChecking, SIGNAL(toggled(bool)), glWidget, SLOT(depthChecking_toggled(bool)));
	connect(ui.chkDumpImage, SIGNAL(toggled(bool)), glWidget, SLOT(dumpImage_toggled(bool)));
	connect(ui.txtDumpImageFolder, SIGNAL(textChanged(const QString&)), glWidget, SLOT(imageFolder_changed(const QString&)));
	
	
	
	
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
	ui.tableWidgetTracks->insertColumn(0);
	QStringList tracksHeader;
	tracksHeader << "Agents";
	ui.tableWidgetTracks->setHorizontalHeaderLabels(tracksHeader);

	ui.tableWidget->insertColumn(0);
	ui.tableWidget->insertColumn(1);	
	ui.tableWidget->insertColumn(2);	
	ui.tableWidget->insertColumn(3);
	QStringList headers;
	headers << "Agents" << "Color" << "Size" << "Glyph";
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	ui.tableWidgetTypes->insertColumn(0);
	ui.tableWidgetTypes->insertColumn(1);
	ui.tableWidgetTypes->insertColumn(2);
	ui.tableWidgetTypes->insertColumn(3);
	ui.tableWidgetTypes->insertColumn(4);
	QStringList typesHeaders;
	typesHeaders << "Agents" << "Color" << "# at t = 0" << "# at last t" << "Average";
	ui.tableWidgetTypes->setHorizontalHeaderLabels(typesHeaders);
}

void SwarmVis::loadNewData()
{
	ui.tableWidgetTracks->clear();
	QStringList tracksHeader;
	tracksHeader << "Agents";
	ui.tableWidgetTracks->setHorizontalHeaderLabels(tracksHeader);
	ui.tableWidgetTracks->setRowCount(0);
	
	ui.tableWidgetTypes->clear();
	QStringList typesHeaders;
	typesHeaders << "Types" << "Color" << "First" << "Last" << "Avg";
	ui.tableWidgetTypes->setHorizontalHeaderLabels(typesHeaders);
	ui.tableWidgetTypes->setRowCount(0);

	ui.tableWidget->clear();
	QStringList headers;
	headers << "Agents" << "Color" << "Size" << "Glyph";
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	ui.tableWidget->setRowCount(0);
	
	typeIndex.clear();
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
			std::string directory = str.substr(0, found + 1);
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
	QList<std::string> typeList;

	for (int i = 0; i < agents->getNumAgents(); i++)
	{
		std::string s;
		std::stringstream out;
		out << "Agent" << i;
		s = out.str();

		//the path of agents view
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
	//get the counts for the types of agents
	std::map<std::string, double> counts_average;
	std::map<std::string, double> counts_first;
	std::map<std::string, double> counts_last;
	std::vector<Agent> * agents_array = agents->getAgentsVector();
	for (int i = 0; i < agents->getNumAgents(); i++)
	{
		for (int j = 0; j < agents->getTimeSteps(); j++)
		{
			for (int k = 0; k < typeList.size(); k++)
			{
				if (typeList.at(k) == agents_array[j][i].getType())
				{
					++counts_average[typeList.at(k)];
					if (j == 0)
					{
						++counts_first[typeList.at(k)];
					}
					if (j == agents->getTimeSteps() - 1)
					{
						++counts_last[typeList.at(k)];
					}
				}
			}
		}
	}
	
	//Populate the list of Types with the different types
	for (int i = 0; i < typeList.size(); i++)
	{
		std::string s = typeList.at(i);
		QTableWidgetItem * item = new QTableWidgetItem(s.c_str());
		
		QTableWidgetItem *colorItem = new QTableWidgetItem("");
		QColor color = QColor::fromRgbF(0.0, 0.0, 1.0, 1.0);
		colorItem->setBackgroundColor(color);
		
		double average = counts_average[typeList.at(i)]/agents->getTimeSteps();
		QTableWidgetItem *total = new QTableWidgetItem(QString::number(average, 'f', 3));
		double first = counts_first[typeList.at(i)];
		QTableWidgetItem *total_first = new QTableWidgetItem(QString::number(first, 'f', 3));
		double last = counts_last[typeList.at(i)];
		QTableWidgetItem *total_last = new QTableWidgetItem(QString::number(last, 'f', 3));

		int row = ui.tableWidgetTypes->rowCount();
		ui.tableWidgetTypes->insertRow(row);
		ui.tableWidgetTypes->setItem(row, 0, item);
		ui.tableWidgetTypes->setItem(row, 1, colorItem);
		ui.tableWidgetTypes->setItem(row, 2, total_first);
		ui.tableWidgetTypes->setItem(row, 3, total_last);
		ui.tableWidgetTypes->setItem(row, 4, total);
	}
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidgetTypes->resizeColumnsToContents();
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
		}
		
	}
	selectedTrackAgentsChanged(list);
}

void SwarmVis::typeSelectionChanged()
{
	typeIndex.clear();
	QList<QTableWidgetItem*> selectedAgents = ui.tableWidgetTypes->selectedItems();
	
	for (int i = 0; i < selectedAgents.size(); i++)
	{
		QTableWidgetItem* rowItem = selectedAgents.at(i);
		QTableWidgetItem* item = ui.tableWidgetTypes->item(rowItem->row(), 0);
		std::string type_name = item->text().toStdString();

		if (!typeIndex.contains(type_name))
		{
			typeIndex.append(type_name);
		}
		
	}
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
		}
	}
}

void SwarmVis::btnSetColor_clicked()
{
	//typeIndex contains the names of the types (must NOT be an integer)
	if (typeIndex.size() > 0)
	{
		QColor color = QColorDialog::getColor(QColor(0,0,0), this );

		//std::vector<Agent> *agent_array = agents->getAgentsVector();
		for (int i = 0; i < typeIndex.size(); i++)
		{
			for (int j = 0; j < ui.tableWidgetTypes->rowCount(); j++)
			{
				QTableWidgetItem * colorItem = ui.tableWidgetTypes->item(j, 1);
				QTableWidgetItem * nameItem = ui.tableWidgetTypes->item(j, 0);
				if (nameItem->text().toStdString() == typeIndex.at(i))
				{
					colorItem->setBackgroundColor(color);
				}
			}
		}
		QList<std::string> types;
		QList<QColor> colors;
		for (int i = 0; i < ui.tableWidgetTypes->rowCount(); i++)
		{
			QTableWidgetItem * colorItem = ui.tableWidgetTypes->item(i, 1);
			QColor c = colorItem->backgroundColor();
			colors.append(c);
			QTableWidgetItem * nameItem = ui.tableWidgetTypes->item(i, 0);
			std::string s = nameItem->text().toStdString();
			types.append(s);
		}
		updateAgentTypesColor(types, colors);
	}
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

void SwarmVis::btnSetGlyph_clicked()
{
	//sets the glyph for each individual agent that is selected
	if (agentIndex.size() > 0)
	{	
		std::vector<Agent> *agent_array = agents->getAgentsVector();
				
		for (int i = 0; i < agentIndex.size(); i++)
		{
			int aIndex = agentIndex.at(i);
			//set the items size
			QTableWidgetItem * item = ui.tableWidget->item(aIndex, 3);
			
			item->setText("2");

			for (int j = 0; j < agents->getTimeSteps(); j++)
			{
				//set the agents size
				agent_array[j][aIndex].setGlyph(2);
			}
		}
		//glWidget->setInput(agent_array,agents->getTimeSteps());
	}
}


