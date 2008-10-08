TEMPLATE = app
TARGET = SwarmVis
QT += core \
    gui \
    opengl
HEADERS += Agent/Agent.h \
    Agent/AgentArray.h \
    OpenGL/glwidget.h \
    Thread/mythread.h \
    swarmvis.h \
    Agent/AgentArray.h \
    Agent/Agent.h \
    OpenGL/glwidget.h \
    Thread/mythread.h \
    swarmvis.h
SOURCES += Agent/AgentArray.cpp \
    Agent/Agent.cpp \
    OpenGL/glwidget.cpp \
    Thread/mythread.cpp \
    main.cpp \
    swarmvis.cpp
FORMS += swarmvis.ui \
    swarmvis.ui
RESOURCES += 
