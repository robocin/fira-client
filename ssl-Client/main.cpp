//author  Renato Sousa, 2018
#include <QtNetwork>
#include <stdio.h>
#include "net/robocup_ssl_client.h"
#include "net/grSim_client.h"

#include "net/pb/command.pb.h"
#include "net/pb/common.pb.h"
#include "net/pb/packet.pb.h"
#include "net/pb/replacement.pb.h"

#include "net/pb/vssref_command.pb.h"
#include "net/pb/vssref_common.pb.h"
#include "net/pb/vssref_placement.pb.h"

#include "net/refereeclient.h"
#include "net/replacerclient.h"

enum Fouls{
        FREE_KICK,
        PENALTY_KICK,
        GOAL_KICK,
        FREE_BALL,
        KICKOFF,
        STOP,
        GAME_ON,
};


void printRobotInfo(const fira_message::Robot & robot) {

    printf("ID=%3d \n",robot.robot_id());

    printf(" POS=<%9.2f,%9.2f> \n",robot.x(),robot.y());
    printf(" VEL=<%9.2f,%9.2f> \n",robot.vx(),robot.vy());

    printf("ANGLE=%6.3f \n",robot.orientation());
    printf("ANGLE VEL=%6.3f \n",robot.vorientation());
}

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    std::vector<double> posX, posY ;

    refereeClient client("224.5.23.2", 10003);
    client.open(false);

    replacerClient replacer("224.5.23.2", 10004);

    VSSRef::ref_to_team::VSSRef_Command packet;

    while(true){
        //printf("A esperar....\n");
       if(client.receive(packet)){
           printf("Foul : %d\n", packet.foul());
           posX = {-0.08,-0.35,-0.71};
           posY = {-0.4,0.13,-0.02};
           replacer.sendCommand(posX, posY);
       }
    }

    /*
    //define your team color here
    bool my_robots_are_yellow = false;
    
    // the ip address need to be in the range 224.0.0.0 through 239.255.255.255
    RoboCupSSLClient visionClient("224.5.23.2", 10020);
    visionClient.open(false);

    RoboCupSSLClient refereeClient("224.5.23.2", 10020);
    refereeClient.open(false);

    GrSim_Client commandClient("127.0.0.1", 20011);
    

    fira_message::sim_to_ref::Environment packet;

    while(true) {
        if (visionClient.receive(packet)) {
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_frame()) {
                fira_message::Frame detection = packet.frame();


                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n =  detection.robots_yellow_size();

                //Ball info:

                fira_message::Ball ball = detection.ball();
                printf("-Ball:  POS=<%9.2f,%9.2f> \n",ball.x(),ball.y());

                //Blue robot info:
                for (int i = 0; i < robots_blue_n; i++) {
                    fira_message::Robot robot = detection.robots_blue(i);
                    //printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
                    //printRobotInfo(robot);

                    if(!my_robots_are_yellow){
                        if(robot.x() <= 0){
                            commandClient.sendCommand(10, 10, my_robots_are_yellow, i);
                        }else{
                            commandClient.sendCommand(-10, -10, my_robots_are_yellow, i);
                        }
                    }
                }

                //Yellow robot info:
                for (int i = 0; i < robots_yellow_n; i++) {
                    fira_message::Robot robot = detection.robots_yellow(i);
                    //printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
                    //printRobotInfo(robot);

                    if(my_robots_are_yellow){
                        if(robot.x() <= 0){
                            commandClient.sendCommand(10, 10, my_robots_are_yellow, i);
                        }else{
                            commandClient.sendCommand(-10, -10, my_robots_are_yellow, i);
                        }
                    }
                }

            }

            //see if packet contains geometry data:
            //if (packet.has_field()){
            //    printf("-[Geometry Data]-------\n");
            //
            //    const fira_message::Field & field = packet.field();
            //    printf("Field Dimensions:\n");
            //    printf("  -field_length=%f (mm)\n",field.length());
            //    printf("  -field_width=%f (mm)\n",field.width());
            //    printf("  -goal_width=%f (mm)\n",field.goal_width());
            //    printf("  -goal_depth=%f (mm)\n",field.goal_depth());
            //}
        }
    }
*/
    return 0;
}
