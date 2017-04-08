// diane_climber_nodelet.cpp



#include <diane_climber/diane_climber_nodelet.h>


using namespace std;


void diane_climber::DianeClimberNodelet::TreatBoolCallBack(const std_msgs::Bool::ConstPtr &msg)
{
    Publishcontroll();
}


diane_climber::DianeClimberNodelet::DianeClimberNodelet()
{

}

unsigned char  diane_climber::DianeClimberNodelet::GetNewControlID()
{
    controller::RequestIDRequest req;
    controller::RequestIDResponse res;
//    if (srvOriginIDcli.call(req, res))
//        return res.id;
//    else
//        return 0;
}


void diane_climber::DianeClimberNodelet::onInit()
{
    nodeHandle = getNodeHandle();

    std::string controllerName;
//    privateNH.param("controler_name", controllerName, (std::string)"diane_controller");

    Pubcontroll = nodeHandle.advertise <std_msgs::Float64MultiArray>("teste",10);

    msgBoolSub = nodeHandle.subscribe <std_msgs::Bool> ("/bool_msg", 10, &DianeClimberNodelet::TreatBoolCallBack, this);


    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/Robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);

//    srvOriginIDcli = nodeHandle.serviceClient<controller::RequestID>(controllerName + "/request_id");

    //Iniciando o Ciclo do Thread

    StartInternalCycle();

}

void diane_climber::DianeClimberNodelet::TreatKinectAngleCallBack(const std_msgs::Float64ConstPtr &msg)
{
    kinectAngle = msg->data;
}

void diane_climber::DianeClimberNodelet::TreatFeedback(const std_msgs::Float64MultiArrayConstPtr &msg)
{
    kinectAngle =  msg->data[1];
    velLin = msg->data[2];
    posFrontArm =msg->data[3];
    posRearArm =msg->data[4];
    cout<<kinectAngle<<endl;
}

diane_climber::DianeClimberNodelet::~DianeClimberNodelet()
{
    StopInternalCycle();
}

void diane_climber::DianeClimberNodelet::Publishcontroll()
{
    std_msgs::Float64MultiArray msg;

    msg.data.push_back(0);




  Pubcontroll.publish(msg);
}
