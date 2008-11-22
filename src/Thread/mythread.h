#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include <QtGui>

class MyThread : public QThread
{
	Q_OBJECT

	public:
		MyThread(QObject *parent, int maxTimeIndex, double delayToNextFrame);
		void run(); // this is virtual method, we must implement it in our subclass of QThread
		
	
	private:
		int maxTime;
		double delay;
		int currentTime;
		void custom_sleep (double delay);
		bool stopped;
		bool continuousPlay;
		
	signals:
		void playSignal();

	private slots:
		void setTimeIndex(int timeIndex);
		void setDelay(double ms);
		void startStopThread();
};


#endif /*MYTHREAD_H_*/
