#include "mythread.h"


MyThread::MyThread(QObject *parent, int maxTimeIndex, double delayToNextFrame)
: QThread(parent)
{
	maxTime = maxTimeIndex;
	delay = delayToNextFrame;
	currentTime = 0;
	stopped = false;	
}

void MyThread::custom_sleep(double delay)
{
	clock_t start_time;
	start_time = clock();
	while((clock() - start_time) < delay * CLOCKS_PER_SEC)
	{
		//do nothing
	}
}

void MyThread::run()
{	
	currentTime = 0;
	qDebug() << "Executing in new independant thread, GUI is NOT blocked";
	while (true)
	{
		custom_sleep(delay);
		if (!stopped)
		{
		if (!(currentTime >= maxTime))
		{
			playSignal(); //signal to update the timeIndex
		}
		else
		{
			break;			
		}
		}
	}		
	qDebug() << "Thread done";		
	//terminate();
	quit();
	//exec();
	
}
void MyThread::startStopThread()
{
	stopped = !stopped;
}

void MyThread::setTimeIndex(int timeIndex)
{
	currentTime = timeIndex;
}

void MyThread::setDelay(double ms)
{
	delay = ms;
}

