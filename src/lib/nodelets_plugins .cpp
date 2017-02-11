// nodelets_plugins.cpp

#include <pluginlib/class_list_macros.h>

#include <diane_climber/diane_climber_nodelet.h>



//#include <diane_controller/diane_controller_nodelet.h>
//#include <diane_controller/diane_controller_remap.h>

PLUGINLIB_DECLARE_CLASS(diane_climber_lib, DianeClimberNodelet, diane_climber::DianeClimberNodelet, nodelet::Nodelet)


//PLUGINLIB_DECLARE_CLASS(diane_controller_lib, DianeControllerNodelet, diane_controller::DianeControllerNodelet, nodelet::Nodelet)
//PLUGINLIB_DECLARE_CLASS(diane_controller_lib, DianeControllerRemap, diane_controller::DianeControllerRemap, nodelet::Nodelet)

