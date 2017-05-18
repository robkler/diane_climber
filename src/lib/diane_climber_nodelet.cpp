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
    emergencyStop = false;

    climbThread = new boost::thread(boost::bind(&DianeClimberNodelet::StairClimbPositioningThreadTask, this));


    nodeHandle = getNodeHandle();


    ///Initializing the Publishers
    msgInputControlPub = nodeHandle.advertise <controller::Control> ("/robot/r/diane_controller/input", 10, true);

    ///Initializing the Subscribers
    msgFeedbackSub = nodeHandle.subscribe <std_msgs::Float64MultiArray> ("/robot/r/diane_controller/Motion/Feedback", 10, &DianeClimberNodelet::TreatFeedback, this);
    msgStartClimbStairPositioningSub = nodeHandle.subscribe <std_msgs::Bool> ("/robot/r/diane_climber/Start_Stair_Climb_Positioning", 10, &DianeClimberNodelet::TreatStartStairClimbPositioning, this);
    msgEmergenyStopClimbPositioningSub = nodeHandle.subscribe <std_msgs::Bool> ("/robot/r/diane_climber/Emergency_Stop_Climb_Positioning", 10, &DianeClimberNodelet::TreatEmergencyStopClimbPositioning, this);


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

    mutParam.unlock();

    cout << "Posição do braço frontal: " << posFrontArm << endl;
    cout << "Posição do braço traseiro: " << posRearArm << endl;

    cout << "TreatFeedback Finalizada!" << endl << endl;

}


void diane_climber::DianeClimberNodelet::TreatStartStairClimbPositioning(std_msgs::Bool msg)
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


void diane_climber::DianeClimberNodelet::TreatEmergencyStopClimbPositioning(std_msgs::Bool msg)
{
    if(msg.data)
    {
        mutClimbParam.lock();

        if(climbingStair == true)
        {
            emergencyStop = true;
        }

        mutClimbParam.unlock();
    }

}


void diane_climber::DianeClimberNodelet::StairClimbPositioningThreadTask()
{
    cout << "StairClimbPositioningThreadTask iniciada!" << endl << endl;

    while(this->nodeHandle.ok())
    {
        cout << "StairClimbPositioningThreadTask está ativa!" << endl << endl;

        mutClimbParam.lock();

        if(startStairClimb)
        {
            cout << "Subida de escada iniciada!" << endl << endl;

            climbingStair = true;

            mutClimbParam.unlock();

            //Start Climb Positioning
            StairClimbPositioning(37);


            //Enable varibles to start climb positioning
            mutClimbParam.lock();

            startStairClimb = false;

            climbingStair = false;

            cout << "Subida de escada finalizada!" << endl << endl;

        }

        mutClimbParam.unlock();

        sleep(1);

    }

    cout << "StairClimbPositioningThreadTask finalizada!" << endl << endl;

}


void diane_climber::DianeClimberNodelet::StairClimbPositioning(const float StairAngle)
{

    controller::Control Msg;
    int Id = 0;

    //Parameters
    float linearSpeed = 0.1;

    float InitFrontArmAngle = 45;
    float InitRearArmAngle = 45;
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


        while (((posFArm <= (InitFrontArmAngle - 1)) || (posFArm >= (InitFrontArmAngle + 1)) || (posRArm <= (InitRearArmAngle - 1)) || (posRArm >= (InitRearArmAngle + 1))) && (emergencyStop == false))//Put the arms in position
        {
            cout << "posFrontArm: " << posFrontArm << endl;
            cout << "posRearArm: " << posRearArm << endl;

            Msg = CreateMsgPos(Id, 0, 0, InitFrontArmAngle, InitRearArmAngle);
            msgInputControlPub.publish(Msg);


            //Obtendo os valores
            mutParam.lock();

            posFArm = posFrontArm;
            posRArm = posRearArm;

           mutParam.unlock();

        }



        //Commands robot to go forward until its pitch angle stabilizes in the stair angle

        bool forwardBreak = false;

        clock_t begin_time;
        clock_t end_time;

        while (!forwardBreak && emergencyStop == false)
        {

            mutParam.lock();

            if(((kinectAngle + 2) < StairAngle) || ((kinectAngle - 2) > StairAngle))
            {
                cout << "kinectAngle: " << kinectAngle << endl;

                Msg = CreateMsgVel(Id, linearSpeed, 0, 0, 0);
                msgInputControlPub.publish(Msg);

                begin_time = 0;
                end_time = 0;

            }
            else
            {
                if (begin_time == 0)
                {
                    begin_time = clock();
                }

                end_time = clock();

                cout << "Diferença de tempo: " << (end_time - begin_time)/CLOCKS_PER_SEC << endl;

                if (((end_time - begin_time)/CLOCKS_PER_SEC) > 3)
                {
                    forwardBreak = true;

                    cout << "Entrou!" << endl;

                }
                else
                {
                    cout << "kinectAngle Estabilizado: " << kinectAngle << endl;

                    Msg = CreateMsgVel(Id, linearSpeed, 0, 0, 0);
                    msgInputControlPub.publish(Msg);

                }

            }

            mutParam.unlock();

        }


        //Posição final
        while((((posFArm <= (FinalFrontArmAngle - 1)) || (posFArm >= (FinalFrontArmAngle + 1)) || (posRArm <= (FinalRearArmAngle - 1)) || (posRArm >= (FinalRearArmAngle + 1))) && (((kinectAngle + 2) > StairAngle) && ((kinectAngle - 2) < StairAngle))) && (emergencyStop == false))
        {

            Msg = CreateMsgPos(Id, 0, 0, FinalFrontArmAngle, FinalRearArmAngle);
            msgInputControlPub.publish(Msg);

        }

        emergencyStop = false;

//        //Estado Zero
//        while((posFArm <= (-1)) || (posFArm >= (1)) || (posRArm <= (-1)) || (posRArm >= (1)))
//        {
//            Msg = CreateMsgPos(Id, 0, 0, 0, 0);
//            msgInputControlPub.publish(Msg);
//        }


    }

}

//------------------------CREATE MESSAGE----------------------------------------------------------------------------------------
controller::Control diane_climber::DianeClimberNodelet::CreateMsgPos(int Id, float velLin, float velAng , float posArmF, float posArmB )
{
    controller::Control Msg;
    Msg.originId = Id;
    Msg.modes.push_back(4);
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
    Msg.modes.push_back(3);
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
