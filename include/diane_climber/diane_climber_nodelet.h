/*!
 * \file diane_climber_nodelet.h
 */



#ifndef DIANE_DIANE_CLIMBER_NODELET_H
#define DIANE_DIANE_CLIMBER_NODELET_H

#include <diane_climber/diane_climber.h>
#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <string>

#include <time.h>

#include <std_msgs/Float64.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Bool.h>

#include <diane_climber/ClimbStair.h>

#include <controller/Control.h>
#include <controller/RequestID.h>


namespace diane_climber {



/*!
 * \class DianeClimberNodelet
 * \brief The DianeClimberNodelet class
 */
class DianeClimberNodelet : public DianeClimber, public nodelet::Nodelet
{
    boost::mutex mutParam;

    boost::mutex mutClimbParam;

    boost::thread* climbThread;
    bool startStairClimb;
    bool climbingStair;
    bool emergencyStop;


    /// ROS node handle.
    ros::NodeHandle nodeHandle;


    //Declarando os Publishers das Mensagens
    ros::Publisher msgInputControlPub;

	//Declarando os Subscribers das Mensagens
    ros::Subscriber msgFeedbackSub;
    ros::Subscriber msgStartClimbStairPositioningSub;
    ros::Subscriber msgEmergenyStopClimbPositioningSub;

    //ros::Subscriber teste2Sub;

    //Declarando os Clientes
    ros::ServiceClient srvOriginIDCli;

protected:

	//Métodos de Tratamento de Subscribes
    void TreatFeedback(std_msgs::Float64MultiArray msg);
    void TreatStartStairClimbPositioning(std_msgs::Bool msg);
    void TreatEmergencyStopClimbPositioning(std_msgs::Bool msg);


    //Método da Thread de background que realizará iniciará o posicionamento para a subida da escada
    void StairClimbPositioningThreadTask();



public:


    void StairClimbPositioning(const float StairAngle);

    //Create MSG
    controller::Control CreateMsgPos(int Id, float velLin, float velAng , float posArmF, float posArmB);
    controller::Control CreateMsgVel(int Id, float velLin, float velAng , float velArmF, float velArmB);


    //Método para obter o ID gerado para o Controller
    unsigned char GetNewControlID();


    DianeClimberNodelet();
    void onInit();
    virtual ~DianeClimberNodelet();
};


}
#endif
