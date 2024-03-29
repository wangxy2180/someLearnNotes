#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
void chatterCallback(const sensor_msgs::ImageConstPtr& msg)
 {
  // ROS_INFO("I heard: [%s]", "come on");
try
  {

    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {
  //  ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
 }
 int main(int argc, char **argv)
 {



 ros::init(argc, argv, "listener");
ros::NodeHandle nh;
 cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("/kinect2/qhd/image_color_rect", 1, chatterCallback);
//ros::Subscriber sub = n.subscribe("/kinect2/qhd/image_color_rect", 1, chatterCallback);
ros::spin();
 return 0;
   }
