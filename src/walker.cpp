#include <sstream>
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>

class LookandDrive {
private:
ros::NodeHandle nh_;


public:
ros::Publisher cmd_vel_pub;
float rangetofront=0.0;
LookandDrive (ros::NodeHandle &nh) {
 nh_ = nh;
cmd_vel_pub = nh_.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 10);
}
void Rangecallback(const sensor_msgs::LaserScan& range,float &rangetofront);
};


void LookandDrive::Rangecallback(const sensor_msgs::LaserScan& range,float& rangetofront) {
std::vector<float> rangeVector=range.ranges;
rangetofront = *min_element(rangeVector.begin(), rangeVector.end());
//ROS_INFO("range is "<<rangetofront);
//return rangetofront;
}

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;
  LookandDrive driver(nh);
ros::Subscriber listener = nh.subscribe("scan", 1000, &LookandDrive::Rangecallback, &driver);
//ros::Subscriber listener = nh.subscribe("scan", 1000, &Rangecallback);
geometry_msgs::Twist base_cmd;
ros::Rate loop_rate(20);
while (ros::ok()) {

if (driver.rangetofront>1.0) {
base_cmd.linear.x = 0.25;
}
else {
base_cmd.angular.z = 0.75;
}
driver.cmd_vel_pub.publish(base_cmd);
ros::spinOnce();
loop_rate.sleep();
}
return 0;
}
