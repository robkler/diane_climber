// diane_climber_nodelet.cpp



#include <diane_climber/diane_climber_nodelet.h>


using namespace std;



diane_climber::DianeClimberNodelet::DianeClimberNodelet()
{

}


void diane_climber::DianeClimberNodelet::onInit()
{

    ///*********************************************************************************
    ///Creating the Publishers/Subscribers/Services/Clients of the Diane Octomap Nodelet
    ///*********************************************************************************

    nodeHandle = getNodeHandle();

    ///Initializing the Publishers
    msgInputControlPub = nodeHandle.advertise <controller::Control> (getName() + "/input", 1000, true);


	//Initializing the Subscribers
    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/Robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);


    ///Initializing the Services
    srvClimbStairSer = nodeHandle.advertiseService(getName() + "/Climb_Stair", &DianeClimberNodelet::ClimbStairCallback, this);


    ///Initializing the Clients
    srvOriginIDCli = nodeHandle.serviceClient<controller::RequestID>(getName() + "/request_id");



    ///Inicializing the Thread's Cycle
    StartInternalCycle();

}


void diane_climber::DianeClimberNodelet::TreatFeedback(const std_msgs::Float64MultiArrayConstPtr &msg)
{
    kinectAngle =  msg->data[1];
    velLin = msg->data[2];
    posFrontArm = msg->data[3];
    posRearArm = msg->data[4];
}



bool diane_climber::DianeClimberNodelet::ClimbStairCallback(diane_climber::ClimbStair::Request & req, diane_climber::ClimbStair::Response & res)
{
    //Initialize the climbing algorithm


    return true;
}


diane_climber::DianeClimberNodelet::~DianeClimberNodelet()
{
    StopInternalCycle();
}
