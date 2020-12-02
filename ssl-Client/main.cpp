//author  Renato Sousa, 2018
//#include <QtNetwork>
#include <stdio.h>
#include <iostream>
#include "net/robocup_ssl_client.h"
#include "net/grSim_client.h"
#include "util/timer.h"
#include "util/util.h"


class Objective{
    double m_x;
    double m_y;
    double m_angle;

public:
    Objective(double t_x, double t_y, double t_angle): m_x(t_x),m_y(t_y),m_angle(t_angle){};


    void setY(double value){
        m_y = value;
    }

    void setAngle(double value){
        m_angle = value;
    }

    void setX(double value){
        m_x = value;
    }
    double x(){
        return m_x;
    }
    double y(){
        return m_y;
    }
    double angle(){
        return m_angle;
    }
};


void printRobotInfo(const fira_message::sim_to_ref::Robot & robot) {

    printf("ID=%3d \n",robot.robot_id());

    printf(" POS=<%9.2f,%9.2f> \n",robot.x(),robot.y());
    printf(" VEL=<%9.2f,%9.2f> \n",robot.vx(),robot.vy());

    printf("ANGLE=%6.3f \n",robot.orientation());
    printf("ANGLE VEL=%6.3f \n",robot.vorientation());
}

double to180range(double angle) {
  angle = fmod(angle, 2 * M_PI);
  if (angle < -M_PI) {
    angle = angle + 2 * M_PI;
  } else if (angle > M_PI) {
    angle = angle - 2 * M_PI;
  }
  return angle;
}

double smallestAngleDiff(double target, double source) {
  double a;
  a = fmod(target + 2*M_PI, 2 * M_PI) - fmod(source + 2*M_PI, 2 * M_PI);

  if (a > M_PI) {
    a = a - 2 * M_PI;
  } else if (a < -M_PI) {
    a = a + 2 * M_PI;
  }
  return a;
}


void PID(fira_message::sim_to_ref::Robot robot, Objective objective, int index,GrSim_Client* grSim_client)
{
    double Kp = 20;
    double Kd = 2.5;
    static double lastError = 0;

    double rightMotorSpeed;
    double leftMotorSpeed;

    bool reversed = false;

    double angle_rob = robot.orientation();


    double angle_obj = atan2( objective.y() - robot.y(),  objective.x() - robot.x()) ;


    double error = smallestAngleDiff(angle_rob, angle_obj);

    if(fabs(error) > M_PI/2.0 + M_PI/20.0) {
          reversed = true;
          angle_rob = to180range(angle_rob+M_PI);
          // Calculates the error and reverses the front of the robot
          error = smallestAngleDiff(angle_rob,angle_obj);
    }

    double motorSpeed = (Kp*error) + (Kd * (error - lastError));// + 0.2 * sumErr;
    lastError = error;



    double baseSpeed = 30;

    // Normalize
    motorSpeed = motorSpeed > 30 ? 30 : motorSpeed;
    motorSpeed = motorSpeed < -30 ? -30 : motorSpeed;

    if (motorSpeed > 0) {
      leftMotorSpeed = baseSpeed ;
      rightMotorSpeed = baseSpeed - motorSpeed;
    } else {
      leftMotorSpeed = baseSpeed + motorSpeed;
      rightMotorSpeed = baseSpeed;
    }


    if (reversed) {
      if (motorSpeed > 0) {
        leftMotorSpeed = -baseSpeed + motorSpeed;
        rightMotorSpeed = -baseSpeed ;
      } else {
        leftMotorSpeed = -baseSpeed ;
        rightMotorSpeed = -baseSpeed - motorSpeed;
      }
    }
    grSim_client->sendCommand(leftMotorSpeed,rightMotorSpeed, false, index);
}




double x,y,ang;


Objective defineObjective(fira_message::sim_to_ref::Robot robot, fira_message::sim_to_ref::Ball ball)
{   
    //Implementar a estratégia aqui
    //Deve retornar o objetivo do robô
    return Objective(x, y, ang);
}

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;
    //define your team color here
    bool my_robots_are_yellow = false;
    
    // the ip address need to be in the range 224.0.0.0 through 239.255.255.255
    RoboCupSSLClient *visionClient = new RoboCupSSLClient("224.5.23.2", 10002);
    visionClient->open(false);


    GrSim_Client *commandClient = new GrSim_Client();
    

    fira_message::sim_to_ref::Environment packet;

    while(true) {
        if (visionClient->receive(packet)) {
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_frame()) {
                fira_message::sim_to_ref::Frame detection = packet.frame();



                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n =  detection.robots_yellow_size();
                double width,length;
                width = 1.3/2.0;
                length = 1.7/2.0;

                //Ball info:

                fira_message::sim_to_ref::Ball ball = detection.ball();
                ball.set_x((length+ball.x())*100);
                ball.set_y((width+ball.y())*100);
                printf("-Ball:  POS=<%9.2f,%9.2f> \n",ball.x(),ball.y());



                //Blue robot info:
                for (int i = 0; i < robots_blue_n; i++) {
                    fira_message::sim_to_ref::Robot robot = detection.robots_blue(i);
                    robot.set_x((length+robot.x())*100);//convertendo para centimetros
                    robot.set_y((width+robot.y())*100);
                    robot.set_orientation(to180range(robot.orientation()));
                    printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
                    printRobotInfo(robot);
                    if(i==0){
                        Objective o = defineObjective(robot,ball);
                        PID(robot,o,i,commandClient);
                    }
                }

                //Yellow robot info:
                /*for (int i = 0; i < robots_yellow_n; i++) {
                    fira_message::sim_to_ref::Robot robot = detection.robots_yellow(i);
                    printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
                    printRobotInfo(robot);
                }*/

            }

            //see if packet contains geometry data:
            /*if (packet.has_field()){
                printf("-[Geometry Data]-------\n");

                const fira_message::sim_to_ref::Field & field = packet.field();
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


