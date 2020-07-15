//author  Renato Sousa, 2018
#include <QtNetwork>
#include <stdio.h>
#include "net/robocup_ssl_client.h"
#include "net/grSim_client.h"
#include "util/timer.h"

#include "pb/command.pb.h"
#include "pb/common.pb.h"
#include "pb/packet.pb.h"
#include "pb/replacement.pb.h"




void posProcess(fira_message::Frame detection){
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
    }
    else if (detection.ball().x() < -0.75 && fabs(detection.ball().y()) < 0.2)
    {
        side = false;
        goals_yellow++;
        is_goal = true;
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
    steps_fault++;
    int quadrant = 4;
    if (steps_fault * cfg->DeltaTime() * 1000 >= 10000)
    {
        if (fabs(ball_prev_pos.first - detection.ball().x()) < 0.0001 &&
            fabs(ball_prev_pos.second - detection.ball().y()) < 0.0001){
            if ((detection.ball().x() < -0.6) && abs(detection.ball().y() < 0.35)){
                penalty = true;
                side = true;
            }else if(detection.ball().x() > 0.6 && abs(detection.ball().y() < 0.35)){

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
        }
        ball_prev_pos.first = detection.ball().x();
        ball_prev_pos.second = detection.ball().y();
        steps_fault = 0;
    }
    else
    {
        if (fabs(ball_prev_pos.first - detection.ball().x()) > 0.0001 ||
            fabs(ball_prev_pos.second - detection.ball().y()) > 0.0001)
        {
            steps_fault = 0;
        }
    }
    ball_prev_pos.first = detection.ball().x();
    ball_prev_pos.second = detection.ball().y();

    // End Time Detection
    bool end_time;

    if((steps_super * cfg->DeltaTime() * 1000) > 300000){
        end_time = true;
    } else{
        end_time = false;
    }


    if ((((int)(steps_super * cfg->DeltaTime() * 1000) / 60000) - minute) > 0)
    {
        minute++;
        std::cout << "****************** " << minute << " Minutes ****************" << std::endl;
    }

    if (randomStart && (is_goal || penalty || fault || goal_shot || end_time))
    {


       steps_fault = 0;
        if(end_time){
            steps_super = 0;
            goals_blue = 0;
            goals_yellow = 0;
            minute = 0;
        }


    }else if(is_goal || end_time){


        if(side)
        {
            dReal posX[6] = {0.15,0.35,0.71,-0.08,-0.35,-0.71};
            dReal posY[6] = {0.02,0.13,-0.02,0.02,0.13,-0.02};



        }else
        {
            dReal posX[6] = {0.08,0.35,0.71,-0.15,-0.35,-0.71};
            dReal posY[6] = {0.02,0.13,-0.02,0.02,0.13,-0.02};

        }if(end_time){
            steps_fault = 0;
            steps_super = 0;
            goals_blue = 0;
            goals_yellow = 0;
            minute = 0;
        }



    }else if(fault){
        if(quadrant == 0){


            dReal posX[6] = {-0.575,-0.44,-0.71,-0.175,-0.3,0.71};
            dReal posY[6] = {-0.4,0.13,-0.02,-0.4,0.13,-0.02};



        }else if(quadrant == 1){


            dReal posX[6] = {-0.575,-0.44,-0.71,-0.175,-0.30,0.71};
            dReal posY[6] = {0.4,-0.13,-0.02,0.4,-0.13,-0.02};


        }else if(quadrant == 2){


            dReal posX[6] = {0.175,0.3,-0.71,0.575,0.44,0.71};
            dReal posY[6] = {-0.4,0.13,-0.02,-0.4,0.13,-0.02};


        }else if(quadrant == 3){


            dReal posX[6] = {0.175,0.3,-0.71,0.575,0.44,0.71};
            dReal posY[6] = {0.4,-0.13,-0.02,0.4,-0.13,-0.02};


        }
        steps_fault = 0;

    }else if(penalty){

        steps_fault = 0;

        if(side)
        {
            dReal posX[6] = {0.75, -0.06, -0.06, 0.35, -0.05,-0.74};
            dReal posY[6] = {-0.01, 0.23, -0.33, 0.02, 0.48, 0.01};


        }else
        {
            dReal posX[6] = {0.35, -0.05,-0.74,0.75, -0.06, -0.06};
            dReal posY[6] = {0.02, 0.48, 0.01,-0.01, 0.23, -0.33};



        }

    }else if(goal_shot){

        steps_fault = 0;

        dReal posX[6] = {0.65, 0.48, 0.49, 0.19, 0.18, -0.67};
        dReal posY[6] = {0.11, 0.37, -0.33, 0.13, -0.21, -0.01};
        if(side)
        {

        }else
        {

        }

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
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_frame()) {
                fira_message::Frame detection = packet.frame();




                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n =  detection.robots_yellow_size();
                double width,length;
                width = 1.3/2.0;
                length = 1.7/2.0;

                //Ball info:

                fira_message::Ball ball = detection.ball();
                ball.set_x((length+ball.x())*100);
                ball.set_y((width+ball.y())*100);
                printf("-Ball:  POS=<%9.2f,%9.2f> \n",ball.x(),ball.y());



                //Blue robot info:
                for (int i = 0; i < robots_blue_n; i++) {
                    fira_message::Robot robot = detection.robots_blue(i);
                    robot.set_x((length+robot.x())*100);//convertendo para centimetros
                    robot.set_y((width+robot.y())*100);
                    robot.set_orientation(to180range(robot.orientation()));
                    printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
                    printRobotInfo(robot);
                    ;

                }

                //Yellow robot info:
                for (int i = 0; i < robots_yellow_n; i++) {
                    fira_message::Robot robot = detection.robots_yellow(i);
                    printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
                    printRobotInfo(robot);
                }

            }

            //see if packet contains geometry data:
            if (packet.has_field()){
                printf("-[Geometry Data]-------\n");

                const fira_message::Field & field = packet.field();
                printf("Field Dimensions:\n");
                printf("  -field_length=%f (mm)\n",field.length());
                printf("  -field_width=%f (mm)\n",field.width());
                printf("  -goal_width=%f (mm)\n",field.goal_width());
                printf("  -goal_depth=%f (mm)\n",field.goal_depth());



            }
        }
    }

    return 0;
}


