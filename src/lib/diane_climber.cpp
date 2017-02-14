// diane_climber.cpp


#include <diane_climber/diane_climber.h>

using namespace std;


diane_climber::DianeClimber::DianeClimber()
{
    stop = false;

}




void diane_climber::DianeClimber::onInit()
{
    StartInternalCycle();
}




void diane_climber::DianeClimber::StartInternalCycle()
{
    mutStartStop.lock();

    stop = false;

    internalThread = new boost::thread(DianeClimber::InternalThreadFunction, this);

    mutStartStop.unlock();

}


void diane_climber::DianeClimber::StopInternalCycle()
{
    mutStartStop.lock();

    stop = true;
    internalThread->join();
    delete internalThread;
    internalThread = NULL;

    mutStartStop.unlock();

}


void diane_climber::DianeClimber::InternalThreadFunction(DianeClimber* diane_climber)
{
    diane_climber->InternalCycleProcedure();
}


void diane_climber::DianeClimber::InternalCycleProcedure()
{
    while (!stop)
    {

    }
}


void diane_climber::DianeClimber::ClimberStair()
{
    // subir armF
    CreateMsgPos(0,0,90,0);
    //Andar em direçao a escada e ir verificando o acelerometro
    CreateMsgVel(0.2,0,0.2,0);
    // Quando estiver em cima da escada subir controladamente e verificar o acelerometro para quando ele começar a abaixar
    CreateMsgVel(0.2,0,0,0);
    //abaixar o armF
    CreateMsgPos(0,0,0,0);
    //andar lentamente para sair da escada
    CreateMsgVel(0.1,0,0,0);
    usleep(1000);
}


diane_climber::DianeClimberMsg* diane_climber::DianeClimber::CreateMsgPos(double velLin, double velAng , double posArmF, double posArmB )
{
    DianeClimberMsg* Msg = new DianeClimberMsg();
    Msg->posArmB = posArmB;
    Msg->posArmF = posArmF;
    Msg->velAng = velAng;
    Msg->velLin = velLin;

    return Msg;
}

diane_climber::DianeClimberMsg *diane_climber::DianeClimber::CreateMsgVel(double velLin, double velAng, double velArmF, double velArmB)
{
    DianeClimberMsg* Msg = new DianeClimberMsg();
    Msg->velArmB = velArmB;
    Msg->velArmF = velArmF;
    Msg->velAng = velAng;
    Msg->velLin = velLin;

    return Msg;
}



diane_climber::DianeClimber::~DianeClimber()
{
    StopInternalCycle();
}

//Diane Climber MSG
//----------------------------------------------------------------------------------------------------------------------------------

diane_climber::DianeClimberMsg::DianeClimberMsg()
{

}

diane_climber::DianeClimberMsg::~DianeClimberMsg()
{

}
