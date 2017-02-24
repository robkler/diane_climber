// diane_climber_nodelet.cpp



#include <diane_climber/diane_climber_nodelet.h>


using namespace std;


void diane_climber::DianeClimberNodelet::TreatBoolCallBack(const std_msgs::Bool::ConstPtr &msg)
{
    Publishcontroll();
}

void diane_climber::DianeClimberNodelet::TreatStairCallBack(const diane_octomap::StairInfoConstPtr &msg)
{

    cout << msg->Max_Z << endl;
}

void diane_climber::DianeClimberNodelet::TreatArrayStairCallBack(const diane_octomap::StairArrayInfoConstPtr &msg)
{
    cout<<"msg"<<msg<<endl;
    for (int i = 0; i < msg->Stairs.size(); i++)
    {
        cout<<msg->Stairs.at(i)<<endl;
    }
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

    msgStair = nodeHandle.subscribe <diane_octomap::StairArrayInfo> ("/diane_octomap/Modeled_Stairs_Info_All", 10, &DianeClimberNodelet::TreatArrayStairCallBack, this);

    subKinectAngle = nodeHandle.subscribe <std_msgs::Float64> ("/cur_tilt_angle", 10, &DianeClimberNodelet::TreatKinectAngleCallBack, this);
//    srvOriginIDcli = nodeHandle.serviceClient<controller::RequestID>(controllerName + "/request_id");

    //Iniciando o Ciclo do Thread

    StartInternalCycle();

}

void diane_climber::DianeClimberNodelet::TreatKinectAngleCallBack(const std_msgs::Float64ConstPtr &msg)
{
    kinectAngle = msg->data;
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
