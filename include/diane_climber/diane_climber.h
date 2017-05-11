/*!
 * \file diane_octomap.h
 */



#ifndef DIANE_DIANE_CLIMBER_H
#define DIANE_DIANE_CLIMBER_H


#include <ros/ros.h>
#include <atomic>

#include <controller/Control.h>
#include <controller/RequestID.h>

#include <boost/thread.hpp>

using namespace std;


namespace diane_climber {

class DianeClimberMsg
{
protected:

    public:
    DianeClimberMsg();

    double velLin;
    double velAng;
    double velArmF;
    double velArmB;
    double posArmF;
    double posArmB;


    virtual ~DianeClimberMsg();
};






/*!
 * \class DianeClimber
 * \brief The DianeClimber class
 */
class DianeClimber
{
    /// Pointer used to control the thread responsible for the internal
    ///     cycle.
    boost::thread* internalThread;

    /// Variable used to tell the controller to stop or not.
    bool stop;


    /*!
     * \brief InternalThreadFunction calls the function
     *      DianeClimber::InternalCycleProcedure().
     */
    static void InternalThreadFunction(DianeClimber* diane_climber);

    /*!
     * \brief InternalCycleProcedure calls the function
     *      DianeClimber:InternalCycle() from in each period of time
     *      specified.
     */
    virtual void InternalCycleProcedure();


protected:
    /// Mutex used to control the internal cycle thread.
    boost::mutex mutStartStop;

    std::atomic<double> kinectAngle;
    std::atomic<double> velLin;
    std::atomic<double> posFrontArm;
    std::atomic<double> posRearArm;
    unsigned int controlId = 0;


public:
    DianeClimber();

    void onInit();




    /*!
     * \brief StartInternalCycle starts the internal cycle of the
     *      controller.
     */
    void StartInternalCycle();


    /*!
     * \brief StopInternalCyle waits for the internal cycle to end then
     *      stops the controller.
     */
    void StopInternalCycle();



    virtual ~DianeClimber();
};


}

#endif
