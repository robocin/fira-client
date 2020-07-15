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

void GrSim_Client::sendCommand(double *posX, double *posY, double posBallX, double posBallY){
    fira_message::sim_to_ref::Packet packet;

    fira_message::sim_to_ref::Replacement* replacement = packet.mutable_replace();

    //Ball
    fira_message::sim_to_ref::BallReplacement* ball_replace = replacement->mutable_ball();
    ball_replace->set_x(posBallX);
    ball_replace->set_y(posBallY);
    ball_replace->set_vx(0.0);
    ball_replace->set_vy(0.0);

    //Blue Robots
    for(int i=0;i<NUMBER_OF_ROBOTS;i++){
        fira_message::sim_to_ref::RobotReplacement* robot_replace = replacement->add_robots();
        robot_replace->mutable_position()->set_robot_id(i);
        robot_replace->mutable_position()->set_x(posX[i]);
        robot_replace->mutable_position()->set_y(posY[i]);
        robot_replace->mutable_position()->set_orientation(0.0);
        robot_replace->mutable_position()->set_vx(0.0);
        robot_replace->mutable_position()->set_vy(0.0);
        robot_replace->mutable_position()->set_vorientation(0.0);

        robot_replace->set_yellowteam(false);
        robot_replace->set_turnon(true);

    }

    //Yellow Robots
    for(int i=0;i<NUMBER_OF_ROBOTS;i++){
        fira_message::sim_to_ref::RobotReplacement* robot_replace = replacement->add_robots();
        robot_replace->mutable_position()->set_robot_id(i);
        robot_replace->mutable_position()->set_x(posX[i+NUMBER_OF_ROBOTS]);
        robot_replace->mutable_position()->set_y(posY[i+NUMBER_OF_ROBOTS]);
        robot_replace->mutable_position()->set_orientation(0.0);
        robot_replace->mutable_position()->set_vx(0.0);
        robot_replace->mutable_position()->set_vy(0.0);
        robot_replace->mutable_position()->set_vorientation(0.0);

        robot_replace->set_yellowteam(true);
        robot_replace->set_turnon(true);
    }


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
