/*!
 * \file diane_climber_nodelet.h
 */



#ifndef DIANE_DIANE_CLIMBER_NODELET_H
#define DIANE_DIANE_CLIMBER_NODELET_H

#include <diane_climber/diane_climber.h>
#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <string>

//#include <diane_octomap/StairInfo.h>
//#include <diane_octomap/StairArrayInfo.h>

#include <std_msgs/Float64.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Bool.h>

#include <controller/Control.h>
#include <controller/RequestID.h>

namespace diane_climber {



/*!
 * \class DianeClimberNodelet
 * \brief The DianeClimberNodelet class
 */
class DianeClimberNodelet : public DianeClimber, public nodelet::Nodelet
{
    /// ROS node handle.
    ros::NodeHandle nodeHandle;
    ros::Publisher Pubcontroll;

    ros::Subscriber msgBoolSub;
    ros::Subscriber msgStair;
    ros::Subscriber srvOriginIDcli;
    ros::Subscriber subKinectAngle;
    ros::Subscriber msgFeedbackSub;

protected:


public:
    unsigned char GetNewControlID();

    void TreatBoolCallBack(const std_msgs::Bool::ConstPtr &msg);
   // void TreatStairCallBack(const diane_octomap::StairInfoConstPtr &msg);
    //void TreatArrayStairCallBack(const diane_octomap::StairArrayInfoConstPtr &msg);
    void TreatKinectAngleCallBack (const std_msgs::Float64ConstPtr &msg);
    void TreatFeedback(const std_msgs::Float64MultiArrayConstPtr &msg);
    DianeClimberNodelet();
    void onInit();
    virtual ~DianeClimberNodelet();
    void Publishcontroll();
};


}
#endif
