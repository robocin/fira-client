#include "grSim_client.h"
#include<iostream>


GrSim_Client::GrSim_Client(){

    // create a QUDP socket
    // now with UDP datagram sockets:
    this->dgram_socket = socket(PF_INET, SOCK_DGRAM, 0);
    // build destination
    this->dest.sin_family = AF_INET;
    inet_aton("127.0.0.1", &this->dest.sin_addr);
    this->dest.sin_port = htons(20011);
}
    

void GrSim_Client::sendCommand(double leftWheelVel, double rightWheelVel, bool my_robots_are_yellow, int id){
   
    
    
    fira_message::sim_to_ref::Packet packet;
    char Sentence [1000];

    fira_message::sim_to_ref::Command* command = packet.mutable_cmd()->add_robot_commands();
    command->set_id(id);
    command->set_yellowteam(my_robots_are_yellow);
    command->set_wheel_left(leftWheelVel);
    command->set_wheel_right(rightWheelVel);
    std::string env;
    packet.SerializeToString(&env);
    

    sendto(this->dgram_socket, env.data(), env.size(), 0, 
           (struct sockaddr*)&this->dest, sizeof(this->dest));
    
    
}


