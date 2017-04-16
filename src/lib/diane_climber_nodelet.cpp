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
    msgInputControlPub = nodeHandle.advertise <controller::Control> ("/Robot/r/diane_controller/input", 10, true);


	//Initializing the Subscribers
    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/Robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);


    ///Initializing the Services
    srvClimbStairSer = nodeHandle.advertiseService(getName() + "/Climb_Stair", &DianeClimberNodelet::ClimbStairCallback, this);


    ///Initializing the Clients
    //Modificar o caminho para o servico
    srvOriginIDCli = nodeHandle.serviceClient<controller::RequestID>("/diane_robot/r/diane_controller/request_id");



    ///Inicializing the Thread's Cycle
    StartInternalCycle();
    teste1();



}

void diane_climber::DianeClimberNodelet::teste1()//se puder colocar no serviço seria o ideal
{
    diane_climber::DianeClimberNodelet::ClimberStair(50 , 0.5);
}



bool diane_climber::DianeClimberNodelet::ClimbStairCallback(diane_climber::ClimbStair::Request &req, diane_climber::ClimbStair::Response &res)
{
    //Initialize the climbing algorithm
    //diane_climber::DianeClimberNodelet::ClimberStair(req.stair_angle , 0.5);
    return true;
}


void diane_climber::DianeClimberNodelet::ClimberStair(const float angle, const float dist)
{

    controller::Control Msg;
    int Id;
    //Parameters
    float linearSpeed = 0.5;
    float ArmClimb = 50;
    float posArmClimb = 0;
    Id =GetNewControlID();


    while (posFrontArm != ArmClimb)//Put the arms in position
    {
        Msg = CreateMsgPos(Id ,0 , 0 , ArmClimb , ArmClimb);
        msgInputControlPub.publish(Msg);

    }


    // Go ahead until dont climb stair
    while(kinectAngle+2 < angle)
    {
        Msg = CreateMsgVel(Id, linearSpeed,0,0,0);
        msgInputControlPub.publish(Msg);

    }

    while(posFrontArm != posArmClimb && posRearArm != posArmClimb)
    {

        Msg = CreateMsgPos(Id,0.1,0,0,0);
        msgInputControlPub.publish(Msg);

    }

    Msg = CreateMsgPos(Id,0,0,0,0);
    msgInputControlPub.publish(Msg);



}

//------------------------CREATE MESSAGE----------------------------------------------------------------------------------------
controller::Control diane_climber::DianeClimberNodelet::CreateMsgPos(int Id, float velLin, float velAng , float posArmF, float posArmB )
{
    controller::Control Msg;
    Msg.originId = Id;
    Msg.modes.push_back(3);
    Msg.data.push_back(velLin);
    Msg.data.push_back(velAng);
    Msg.data.push_back(posArmF);
    Msg.data.push_back(posArmB);
    return Msg;
}

controller::Control diane_climber::DianeClimberNodelet::CreateMsgVel(int Id, float velLin, float velAng, float velArmF, float velArmB)
{
    controller::Control Msg;
    Msg.originId = Id;
    Msg.modes.push_back(4);
    Msg.data.push_back(velLin);
    Msg.data.push_back(velAng);
    Msg.data.push_back(velArmF);
    Msg.data.push_back(velArmB);
    return Msg;
}

void diane_climber::DianeClimberNodelet::TreatFeedback(std_msgs::Float64MultiArray msg)
{
    kinectAngle =  msg.data[1];
    velLin = msg.data[2];
    posFrontArm = msg.data[3];
    posRearArm = msg.data[4];
}
unsigned char diane_climber::DianeClimberNodelet::GetNewControlID()
{
    controller::RequestIDRequest req;
    controller::RequestIDResponse res;

    //Requisitando o servico que retorna um ID para o controlador. Se a requisicao não funcionar, utiliza 0.
    if(srvOriginIDCli.call(req, res))
    {
        return res.id;
    }
    else
    {
        return 0;
    }

}
//--------------------------------------------------------------------------------------------------------------------------------
diane_climber::DianeClimberNodelet::~DianeClimberNodelet()
{
    StopInternalCycle();
}
