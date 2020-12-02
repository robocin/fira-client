#ifndef REPLACERCLIENT_H
#define REPLACERCLIENT_H


#include <QObject>
#include <QUdpSocket>
#include <QString>

#include "pb/vssref_command.pb.h"
#include "pb/vssref_common.pb.h"
#include "pb/vssref_placement.pb.h"

class replacerClient
{
public:
    replacerClient(QString address, int port);
    void sendCommand(std::vector<double> posX, std::vector<double> posY);

    QHostAddress _addr;
    quint16 _port;

private:
    QUdpSocket *socket;

};
#endif // REPLACERCLIENT_H
