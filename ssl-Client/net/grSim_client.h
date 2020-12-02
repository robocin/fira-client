#ifndef GRSIMCLIENT_H
#define GRSIMCLIENT_H

//#include <QObject>
//#include <QUdpSocket>
//#include <QString>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "pb/common.pb.h"

class GrSim_Client 
{
public:
    GrSim_Client();
    void sendCommand(double leftWheelVel, double rightWheelVel, bool my_robots_are_yellow, int id);
private:
    int dgram_socket;
    struct sockaddr_in dest;


};

#endif // GRSIMCLIENT_H
