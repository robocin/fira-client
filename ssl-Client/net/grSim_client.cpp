#include "net/grSim_client.h"


GrSim_Client::GrSim_Client(QObject *parent) :
    QObject(parent)
{
    // create a QUDP socket
    socket = new QUdpSocket(this);

    this->_addr.setAddress("127.0.0.1");
    this->_port = quint16(20011);

    socket->bind(this->_addr, this->_port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void GrSim_Client::sendCommand(double velX, int id){
    double zero = 0.0;
    fira_message::sim_to_ref::Packet packet;
    bool yellow = false;

    fira_message::sim_to_ref::Command* command = packet.mutable_cmd()->add_robot_commands();
    command->set_id(id);

    command->set_wheel_left(velX);
    command->set_wheel_right(velX);


    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    if(socket->writeDatagram(dgram, this->_addr, this->_port) > -1){
        printf("send data\n");
    }
}

void GrSim_Client::readyRead(){
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
