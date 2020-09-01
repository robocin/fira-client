TARGET = main
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += network

SOURCES += main.cpp \
    net/pb/command.pb.cc \
    net/pb/common.pb.cc \
    net/pb/packet.pb.cc \
    net/pb/replacement.pb.cc \
    net/pb/vssref_command.pb.cc \
    net/pb/vssref_common.pb.cc \
    net/pb/vssref_placement.pb.cc \
    net/refereeclient.cpp \
    net/replacerclient.cpp \
    net/robocup_ssl_client.cpp \
    net/netraw.cpp \
    net/grSim_client.cpp

HEADERS += net/pb/command.pb.h \
    net/pb/common.pb.h \
    net/pb/packet.pb.h \
    net/pb/replacement.pb.h \
    net/pb/vssref_command.pb.h \
    net/pb/vssref_common.pb.h \
    net/pb/vssref_placement.pb.h \
    net/refereeclient.h \
    net/replacerclient.h \
    net/robocup_ssl_client.h \
    net/netraw.h \
    net/grSim_client.h

LIBS += -lpthread \
    -L/usr/local/lib -lprotobuf -lz
