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

//    nh.setCallbackQueue(&my_queue);



    ///Initializing the Publishers
    msgInputControlPub = nodeHandle.advertise <controller::Control> ("/robot/r/diane_controller/input", 10, true);

    ///Initializing the Subscribers
    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);
//    msgFeedbackSub = nh.subscribe <std_msgs::Float64MultiArray> ("/robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);
    msgClimbStairSub = nodeHandle.subscribe <std_msgs::Bool> ("/robot/r/diane_climber/Start_Stair_Climb", 10, &DianeClimberNodelet::TreatStartStairClimb, this);

    ///Initializing the Services
    srvClimbStairSer = nodeHandle.advertiseService(getName() + "/Climb_Stair", &DianeClimberNodelet::ClimbStairCallback, this);


    ///Initializing the Clients
    //Modificar o caminho para o servico
    srvOriginIDCli = nodeHandle.serviceClient<controller::RequestID>("/robot/r/diane_controller/request_id");



    ///Inicializing the Thread's Cycle
    StartInternalCycle();

}


void diane_climber::DianeClimberNodelet::TreatFeedback(std_msgs::Float64MultiArray msg)
{
    mutParam.lock();

    kinectAngle =  msg.data[1];
    velLin = msg.data[2];
    posFrontArm = msg.data[4];
    cout<<"essa porra: essa porra: essa porra: essa porra: essa porra:"<< kinectAngle <<endl;
    posRearArm = msg.data[5];

    mutParam.unlock();
}


void diane_climber::DianeClimberNodelet::TreatStartStairClimb(std_msgs::Bool msg)
{
    if (msg.data)
    {
       diane_climber::DianeClimberNodelet::ClimbStair(25);

    }
}


bool diane_climber::DianeClimberNodelet::ClimbStairCallback(diane_climber::ClimbStair::Request &req, diane_climber::ClimbStair::Response &res)
{
    //Initialize the climbing algorithm
    //diane_climber::DianeClimberNodelet::ClimbStair(req.stair_angle);
    return true;
}


void diane_climber::DianeClimberNodelet::ClimbStair(const float StairAngle)
{

    controller::Control Msg;
    int Id = 0;

    //Parameters
    float linearSpeed = 0.5;

    float InitFrontArmAngle = 50;
    float InitRearArmAngle = 0;
    float FinalFrontArmAngle = 0;
    float FinalRearArmAngle = 0;


    for(int i=0; i< 3; i++)
    {
        Id = GetNewControlID();

        if(Id > 0)
        {
            break;
        }
        else
        {
            sleep(300);
        }

    }

    if (Id > 0)
    {
        mutParam.lock();

        float posFArm = posFrontArm;
        float posRArm = posRearArm;

        mutParam.unlock();

        while ((posFArm <= InitFrontArmAngle) || (posRArm != InitRearArmAngle))//Put the arms in position
        {
            cout << "posFrontArm: " << posFrontArm << endl;
            //cout << "posRearArm: " << posRearArm << endl;

            Msg = CreateMsgPos(Id ,0 , 0 , InitFrontArmAngle , InitRearArmAngle);
            msgInputControlPub.publish(Msg);

            //Obtendo os valores
            mutParam.lock();

            posFArm = posFrontArm;
            posRArm = posRearArm;

            mutParam.unlock();

        }


//        // Go ahead until dont climb stair
//        while((kinectAngle + 2) < StairAngle)
//        {
//            cout << "kinectAngle: " << kinectAngle << endl;

//            Msg = CreateMsgVel(Id, linearSpeed, 0, 0, 0);
//            msgInputControlPub.publish(Msg);

//        }

//        while((posFrontArm != FinalFrontArmAngle) || (posRearArm != FinalRearArmAngle))
//        {
//            Msg = CreateMsgPos(Id, 0.1, 0, FinalFrontArmAngle, FinalRearArmAngle);
//            msgInputControlPub.publish(Msg);
//        }


/*        Msg = CreateMsgPos(Id, 0, 0, FinalFrontArmAngle, FinalRearArmAngle);
        msgInputControlPub.publish(Msg)*/;

    }

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
