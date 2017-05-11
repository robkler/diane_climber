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

    startStairClimb = false;
    climbingStair = false;

    climbThread = new boost::thread(boost::bind(&DianeClimberNodelet::ClimbStairThreadTask, this));


    nodeHandle = getNodeHandle();


    ///Initializing the Publishers
    msgInputControlPub = nodeHandle.advertise <controller::Control> ("/robot/r/diane_controller/input", 10, true);

    ///Initializing the Subscribers
    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);
    msgClimbStairSub = nodeHandle.subscribe <std_msgs::Bool> ("/robot/r/diane_climber/Start_Stair_Climb", 10, &DianeClimberNodelet::TreatStartStairClimb, this);


    //teste2Sub = nodeHandle.subscribe <std_msgs::Bool> ("/robot/r/diane_climber/teste2", 10, &DianeClimberNodelet::Teste2, this);


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
    cout << "TreatFeedback Iniciada!" << endl << endl;

    mutParam.lock();

    kinectAngle =  msg.data[1];
    velLin = msg.data[2];
    posFrontArm = msg.data[4];
    posRearArm = msg.data[5];

    cout << "Ângulo do Kinect: " << kinectAngle << endl;

    mutParam.unlock();

    cout << "TreatFeedback Finalizada!" << endl << endl;

}


void diane_climber::DianeClimberNodelet::TreatStartStairClimb(std_msgs::Bool msg)
{
    if (msg.data)
    {
        mutClimbParam.lock();

        if(startStairClimb == false && climbingStair == false)
        {
            startStairClimb = true;
        }

        mutClimbParam.unlock();

    }

}


void diane_climber::DianeClimberNodelet::ClimbStairThreadTask()
{
    cout << "ClimbStairThreadTask iniciada!" << endl << endl;

    while(this->nodeHandle.ok())
    {
        cout << "ClimbStairThreadTask está ativa!" << endl << endl;

        mutClimbParam.lock();

        if(startStairClimb)
        {
            cout << "Subida de escada iniciada!" << endl << endl;

            climbingStair = true;

            ClimbStair(25);


            startStairClimb = false;

            climbingStair = false;

            cout << "Subida de escada finalizada!" << endl << endl;

        }

        mutClimbParam.unlock();

        sleep(1);

    }

    cout << "ClimbStairThreadTask finalizada!" << endl << endl;

}


//void diane_climber::DianeClimberNodelet::Teste2(std_msgs::Bool msg)
//{

//    cout << "Início do Teste2" << endl;

//    if(msg.data)
//    {
//        cout << "Teste2: True." << endl;
//    }
//    else
//    {
//        cout << "Teste2: False." << endl;
//    }

//    cout << "Fim do Teste2" << endl << endl;

//}


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


    for(int i=0; i<10; i++)
    {
        Id = GetNewControlID();

        if(Id > 0)
        {
            break;
        }
        else
        {
            sleep(1);
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

            sleep(1);

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
