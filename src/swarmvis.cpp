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
	
			
	connect(ui.timeSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setTimeIndex(int)));
	connect(ui.chkOverrideAgentColor, SIGNAL(toggled(bool)), glWidget, SLOT(override_toggled(bool)));
	connect(ui.spinBoxTrailLength, SIGNAL(valueChanged(int)), glWidget, SLOT(setTrailLength(int)));
	connect(ui.chkAgentPaths, SIGNAL(toggled(bool)), glWidget, SLOT(toggleShowPaths(bool)));
	connect(ui.chkSameColor, SIGNAL(toggled(bool)), glWidget, SLOT(showPathWithSameColor(bool)));

	connect(ui.sliderR, SIGNAL(valueChanged(int)), glWidget, SLOT(individualAgentsColorValueR(int)));
	connect(ui.sliderG, SIGNAL(valueChanged(int)), glWidget, SLOT(individualAgentsColorValueG(int)));
	connect(ui.sliderB, SIGNAL(valueChanged(int)), glWidget, SLOT(individualAgentsColorValueB(int)));
	connect(ui.sliderO, SIGNAL(valueChanged(int)), glWidget, SLOT(individualAgentsColorValueO(int)));
	connect(this, SIGNAL(selectedAgentsChanged(QList<QListWidgetItem*>)), glWidget, SLOT(selectedAgentsChanged(QList<QListWidgetItem*>)));

	connect(ui.sliderR_2, SIGNAL(valueChanged(int)), glWidget, SLOT(agentsColorValueR(int)));
	connect(ui.sliderG_2, SIGNAL(valueChanged(int)), glWidget, SLOT(agentsColorValueG(int)));
	connect(ui.sliderB_2, SIGNAL(valueChanged(int)), glWidget, SLOT(agentsColorValueB(int)));
	connect(ui.sliderO_2, SIGNAL(valueChanged(int)), glWidget, SLOT(agentsColorValueO(int)));

	connect(ui.chkBoundingBox, SIGNAL(toggled(bool)), glWidget, SLOT(boundingBox_toggled(bool)));
	connect(this, SIGNAL(updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>)),
		glWidget, SLOT(updateAgentTypesColor(QList<int>, QList<double>, QList<double>, QList<double>, QList<double>)));
}

SwarmVis::~SwarmVis()
{

}

void SwarmVis::listSelectionChanged()
{
	QList<QListWidgetItem*> selectedAgents = ui.listWidgetAgents->selectedItems();
	selectedAgentsChanged(selectedAgents);
}



void SwarmVis::btnPlay_clicked()
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

void SwarmVis::menubar_action_handler(QAction* action)
{
	if (action->text().compare("&Quit") == 0)
	{
		exit(0);
	}
	if (action->text().compare("&Load Agent Data") == 0)
	{			
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
							"/home/workspace5/3DSwarmData/",
							tr("Info (*.info);;Text file (*.txt);;XML files (*.xml);; All File (*.*)"));

		if (fileName.length() > 0)
		{
		std::string str =  fileName.toStdString();
		size_t found;		
		found = str.find_last_of("/");
		std::string directory = str.substr(0, found+1);
		std::string filename = str.substr(found + 1);
		//std::cout << directory << std::endl;
		//std::cout << filename << std::endl;
		
		agents = new AgentArray(directory, filename);
		ui.timeSlider->setMaximum(agents->getTimeSteps() - 1);
		std::vector<Agent> * v = agents->getAgentsVector();
		glWidget->setInput(v, agents->getTimeSteps() - 1);
		//createThread();
		populateListView();
		}
	}	
}


void SwarmVis::threadFinished()
{
	ui.timeSlider->setValue(0);
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
	QItemSelectionModel * selectionModel = ui.listWidgetAgents->selectionModel();
	selectionModel->clearSelection();
}


void SwarmVis::populateListView()
{
	ui.listWidgetAgents->clear();
	ui.listWidgetTypes->clear();
	//setup the table
	ui.tableWidget->clear();	
	ui.tableWidget->insertColumn(0);	
	ui.tableWidget->insertColumn(1);	
	ui.tableWidget->insertColumn(2);	
	ui.tableWidget->insertColumn(3);	
	QStringList headers;
    headers << "Agents" << "Color" << "Size" << "Glyph";
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	//end setup the table
	
	typeList.clear();

	for (int i = 0; i < agents->getNumAgents(); i++)
	{	
		//the agents view	
		QListWidgetItem * item = new QListWidgetItem;
		std::string s;
		std::stringstream out;
		out << "Agent" << i;
		s = out.str();				
		item->setText(s.c_str());
		item->setData(Qt::ToolTipRole, i);	
		ui.listWidgetAgents->addItem(item);

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

	connect(ui.listWidgetAgents, SIGNAL(itemSelectionChanged()), this, SLOT(listSelectionChanged()));
	//QList<QListWidgetItem *> QListWidget::selectedItems () const
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

void SwarmVis::btnSetColor_2_clicked()
{
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

void SwarmVis::btnSetColor_clicked()
{
	if (typeSelectedItem!=NULL)
	{			
		QColor color = QColorDialog::getColor(typeSelectedItem->backgroundColor(), this );	
		//double r = (double)ui.sliderR_3->value ()/(double)10000;
		//double g = (double)ui.sliderG_3->value ()/(double)10000;
		//double b = (double)ui.sliderB_3->value ()/(double)10000;
		//double o = (double)ui.sliderO_3->value ()/(double)10000;
		//QColor color; // =	QColor( 100, 100, 100, 255 );
		//color.setRgbF(r, g, b, o);		
		typeSelectedItem->setBackgroundColor(color);
		double r = color.redF();
		double g = color.greenF();
		double b = color.blueF();
		double o = color.alphaF();
		
		bool * temp;
		int val = typeSelectedItem->data(Qt::DisplayRole).toInt(temp);		
		if (!agentTypeIndex.contains(val))
		{
			//std::cout<<"DOES NOT CONTAIN"<<std::endl;
			agentTypeIndex.append(val);
			agentR.append(r);
			agentG.append(g);
			agentB.append(b);
			agentO.append(o);
		}
		else
		{
			//std::cout<<"DOES CONTAIN"<<std::endl;
			agentR.replace(agentTypeIndex.indexOf(val),r);
			agentG.replace(agentTypeIndex.indexOf(val),g);
			agentB.replace(agentTypeIndex.indexOf(val),b);
			agentO.replace(agentTypeIndex.indexOf(val),o);			
		}
		/*for (int i = 0; i < agentR.size(); i++)
		{
			double c[4] = {agentR.at(i),agentG.at(i),agentB.at(i),agentO.at(i)};
			double nc[4] = {c[0],c[1],c[2],c[3]};
			std::cout << nc[0] <<"::"<<nc[1] <<"::" <<nc[2]<<"::"<<nc[3] << std::endl<<std::endl;		
		}*/		
		updateAgentTypesColor(agentTypeIndex, agentR, agentG, agentB, agentO);			
	}
}





