#include "replacerclient.h"
#include<QUdpSocket>

replacerClient::replacerClient(QString address, int port){

    // create a QUDP socket
    socket = new QUdpSocket();

    this->_addr.setAddress(address);
    this->_port = quint16(port);

    socket->bind(this->_addr, this->_port);
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


