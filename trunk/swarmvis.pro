TEMPLATE = app
TARGET = bin/SwarmVis
QT += 	core \
	gui \
	opengl
HEADERS += src/Agent/Agent.h \
	src/Agent/AgentArray.h \
	src/OpenGL/glwidget.h \
	src/Thread/mythread.h \
	src/swarmvis.h
SOURCES += src/Agent/AgentArray.cpp \
	src/Agent/Agent.cpp \
	src/OpenGL/glwidget.cpp \
	src/Thread/mythread.cpp \
	src/main.cpp \
	src/swarmvis.cpp
FORMS += src/swarmvis.ui
