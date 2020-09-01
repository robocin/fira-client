#include "replacerclient.h"

replacerClient::replacerClient(QString address, int port, QObject *parent) : QObject(parent){

    // create a QUDP socket
    socket = new QUdpSocket(this);

    this->_addr.setAddress(address);
    this->_port = quint16(port);

    socket->bind(this->_addr, this->_port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void replacerClient::sendCommand(std::vector<double> posX, std::vector<double> posY){
    VSSRef::team_to_ref::VSSRef_Placement packet;

    packet.mutable_world()->set_teamcolor(VSSRef::Color::BLUE);

    for(int i=0;i<3;i++){
            VSSRef::Robot* robot_replace = packet.mutable_world()->add_robots();
            robot_replace->set_robot_id(i);
            robot_replace->set_x(posX[i]);
            robot_replace->set_y(posY[i]);
            robot_replace->set_orientation(0.0);
    }

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    if(socket->writeDatagram(dgram, this->_addr, this->_port) > -1){
        printf("send data\n");
    }
}

void replacerClient::readyRead(){
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;
}
