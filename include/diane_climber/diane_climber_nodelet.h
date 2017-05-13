/*!
 * \file diane_climber_nodelet.h
 */



#ifndef DIANE_DIANE_CLIMBER_NODELET_H
#define DIANE_DIANE_CLIMBER_NODELET_H

#include <diane_climber/diane_climber.h>
#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <string>


#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Bool.h>

#include <diane_climber/ClimbStair.h>

#include <controller/Control.h>
#include <controller/RequestID.h>

#include <future>

namespace diane_climber {



/*!
 * \class DianeClimberNodelet
 * \brief The DianeClimberNodelet class
 */
class DianeClimberNodelet : public DianeClimber, public nodelet::Nodelet
{
    boost::mutex mutParam;

    /// ROS node handle.
    ros::NodeHandle nodeHandle;
//    ros::NodeHandle nh;

//    ros::CallbackQueue my_queue;

    //Declarando os Publishers das Mensagens
    ros::Publisher msgInputControlPub;

	//Declarando os Subscribers das Mensagens
    ros::Subscriber msgFeedbackSub;
    ros::Subscriber msgClimbStairSub;

    //Declarando os Services
    ros::ServiceServer srvClimbStairSer;

    //Declarando os Clientes
    ros::ServiceClient srvOriginIDCli;

protected:

	//Métodos de Tratamento de Subscribes
    void TreatFeedback(std_msgs::Float64MultiArray msg);

    void TreatStartStairClimb(std_msgs::Bool msg);


    //Métodos de Callback do servico de subida de escada
    bool ClimbStairCallback(diane_climber::ClimbStair::Request & req, diane_climber::ClimbStair::Response & res);

public:


    void ClimbStair(const float StairAngle);

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
