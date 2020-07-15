//author  Renato Sousa, 2018
#include <QtNetwork>
#include <stdio.h>
#include <iostream>
#include "net/robocup_ssl_client.h"
#include "net/grSim_client.h"
#include "util/timer.h"

#include "pb/command.pb.h"
#include "pb/common.pb.h"
#include "pb/packet.pb.h"
#include "pb/replacement.pb.h"




void posProcess(fira_message::Frame detection, uint32_t step){
    static Timer t;
    bool side;
    bool is_goal = false;
    bool out_of_bands = false;
    static int goals_blue = 0;
    static int goals_yellow = 0;

    int robots_blue_n = detection.robots_blue_size();
    int robots_yellow_n = detection.robots_yellow_size();


    // Goal Detection
    if (detection.ball().x() > 0.75 && fabs(detection.ball().y()) < 0.2)
    {
        side = true;
        goals_blue++;
        is_goal = true;
        std::cout << "****************** Goal Blue ****************" << std::endl;

    }
    else if (detection.ball().x() < -0.75 && fabs(detection.ball().y()) < 0.2)
    {
        side = false;
        goals_yellow++;
        is_goal = true;
        std::cout << "****************** Goal Yellow ****************" << std::endl;

    }
    if (detection.ball().x() < -2 || detection.ball().x() > 2 || detection.ball().y() > 2 || detection.ball().y() < -2)
        out_of_bands = true;


    bool penalty = false;
    bool goal_shot = false;
    if (detection.ball().x() < -0.6 && abs(detection.ball().y()) < 0.35)
    {
        // Penalti Detection
        bool one_in_pen_area = false;
        for (int i = 0; i < robots_blue_n; i++)
        {
            fira_message::Robot robot = detection.robots_blue(i);


            if (robot.x() < -0.6 && fabs(robot.y()) < 0.35)
            {
                if (one_in_pen_area){
                    penalty = true;
                    side = true;
                }
                else
                    one_in_pen_area = true;
            }
        }

        // Atk Fault Detection

        bool one_in_enemy_area = false;
        for (int i = 0; i < robots_yellow_n; i++)
        {
            fira_message::Robot robot = detection.robots_yellow(i);

            if (robot.x() < -0.6 && fabs(robot.y()) < 0.35)
            {
                if (one_in_enemy_area){
                    goal_shot = true;
                    side = false;

                }
                else
                    one_in_enemy_area = true;
            }
        }

    }


    if (detection.ball().x() > 0.6 && fabs(detection.ball().y()) < 0.35)
    {
        // Penalti Detection
        bool one_in_pen_area = false;
        for (int i = 0; i < robots_yellow_n; i++)
        {
            fira_message::Robot robot = detection.robots_yellow(i);

            if (robot.x() > 0.6 && fabs(robot.y()) < 0.35)
            {
                if (one_in_pen_area){
                    penalty = true;
                    side =false;
                }
                else
                    one_in_pen_area = true;
            }
        }

        // Atk Fault Detection

        bool one_in_enemy_area = false;
        for (int i = 0; i < robots_blue_n; i++)
        {
            fira_message::Robot robot = detection.robots_blue(i);
            if (robot.x() > 0.6 && abs(robot.y()) < 0.35)
            {
                if (one_in_enemy_area){
                    goal_shot = true;
                    side = true;

                }
                else
                    one_in_enemy_area = true;
            }
        }

    }

    // Fault Detection
    bool fault = false;
    int quadrant = 4;
    static uint32_t oldStep = step;
    static double ball_prev_posX = -50,ball_prev_posY=-50;

    if (fabs(ball_prev_posX - detection.ball().x()) < 0.0001 &&
        fabs(ball_prev_posY - detection.ball().y()) < 0.0001){
        if(step - oldStep >= 10000){
            if ((detection.ball().x() < -0.6) && fabs(detection.ball().y()) < 0.35){
                penalty = true;
                side = true;
            }else if(detection.ball().x() > 0.6 && fabs(detection.ball().y()) < 0.35){

                penalty = true;
                side = false;
            }else{
                fault = true;
                if(detection.ball().x() < 0 && detection.ball().y() <= 0){
                   quadrant = 0;
                }else if(detection.ball().x() < 0 && detection.ball().y() >= 0){
                    quadrant = 1;
                }else if(detection.ball().x() > 0 && detection.ball().y() <= 0){
                    quadrant = 2;
                }else if(detection.ball().x() > 0 && detection.ball().y() >= 0){
                    quadrant = 3;
                }
            }
            oldStep = step;
        }
    }else{
        oldStep = step;
    }

    ball_prev_posX = detection.ball().x();
    ball_prev_posY = detection.ball().y();

    // End Time Detection
    bool end_time;

    if(step > 300000){
        end_time = true;
    } else{
        end_time = false;
    }

    static int minute = 0;
    if ((((step) / 60000) - minute) > 0)
    {
        minute++;
        std::cout << "****************** " << minute << " Minutes ****************" << std::endl;
    }
    if(fault){
        std::cout << "****************** Fault ****************" << std::endl;
    }
    if(penalty){
        std::cout << "****************** Penalty ****************" << std::endl;
    }
    if(goal_shot){
        std::cout << "****************** Goal Shot ****************" << std::endl;
    }


    if(end_time){
        goals_blue = 0;
        goals_yellow = 0;
        minute = 0;
    }



}


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
    RoboCupSSLClient client;
    client.open(false);
    fira_message::sim_to_ref::Environment packet;

    GrSim_Client grSim_client;

    while(true) {
        if (client.receive(packet)) {
            //printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_frame()) {
                fira_message::Frame detection = packet.frame();

                posProcess(detection,packet.step());

                //Ball info:
            }

            //see if packet contains geometry data:
            /*if (packet.has_field()){
                printf("-[Geometry Data]-------\n");

                const fira_message::Field & field = packet.field();
                printf("Field Dimensions:\n");
                printf("  -field_length=%f (mm)\n",field.length());
                printf("  -field_width=%f (mm)\n",field.width());
                printf("  -goal_width=%f (mm)\n",field.goal_width());
                printf("  -goal_depth=%f (mm)\n",field.goal_depth());



            }*/
        }
    }

    return 0;
}


