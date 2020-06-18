TARGET = main
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += network

SOURCES += main.cpp \
    pb/command.pb.cc \
    pb/common.pb.cc \
    pb/packet.pb.cc \
    pb/replacement.pb.cc \
    net/robocup_ssl_client.cpp \
    net/netraw.cpp \
    net/grSim_client.cpp

HEADERS += pb/command.pb.h \
    pb/common.pb.h \
    pb/packet.pb.h \
    pb/replacement.pb.h \
    net/robocup_ssl_client.h \
    net/netraw.h \
    util/timer.h \
    util/util.h \
    net/grSim_client.h

INCLUDEPATH += include
LIBS += -lpthread \
    -L/usr/local/lib -lprotobuf -lz
