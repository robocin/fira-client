#ifndef GRSIMCLIENT_H
#define GRSIMCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QString>

#include "pb/command.pb.h"
#include "pb/common.pb.h"
#include "pb/packet.pb.h"
#include "pb/replacement.pb.h"

class GrSim_Client : public QObject
{
    Q_OBJECT
public:
    GrSim_Client(QString address, int port, QObject *parent = 0);
    void sendCommand(double leftWheelVel, double rightWheelVel, bool my_robots_are_yellow, int id);

    QHostAddress _addr;
    quint16 _port;

signals:

public slots:
    void readyRead();

private:
    QUdpSocket *socket;

};

#endif // GRSIMCLIENT_H
