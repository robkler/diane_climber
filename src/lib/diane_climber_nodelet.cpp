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


void diane_climber::DianeClimberNodelet::onInit()
{
    nodeHandle = getNodeHandle();

    Pubcontroll = nodeHandle.advertise <std_msgs::Float64MultiArray>("teste",10);

    msgBoolSub = nodeHandle.subscribe <std_msgs::Bool> ("/bool_msg", 10, &DianeClimberNodelet::TreatBoolCallBack, this);

    msgStair = nodeHandle.subscribe <diane_octomap::StairArrayInfo> ("/diane_octomap/Modeled_Stairs_Info_All", 10, &DianeClimberNodelet::TreatArrayStairCallBack, this);

    //Iniciando o Ciclo do Thread

    StartInternalCycle();

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
