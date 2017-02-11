// diane_climber.cpp


#include <diane_climber/diane_climber.h>

using namespace std;


diane_climber::DianeClimber::DianeClimber()
{
    stop = false;

}

void diane_climber::DianeClimber::ClimberStair()
{
    usleep(1000);
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



//double velLin = 0;
//double velAng = 0;
//double velArmF = 0;
//double velArmB = 0;
//double posArmF = 0;
//double posArmB = 0;


diane_climber::DianeClimber::~DianeClimber()
{
    StopInternalCycle();
}


