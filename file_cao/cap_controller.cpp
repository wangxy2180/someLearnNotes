#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/ApplyJointEffort.h>
#include <gazebo_msgs/GetJointProperties.h>
#include <gazebo_msgs/GetLinkState.h>
#include <sensor_msgs/JointState.h>
#include <string.h>
#include <stdio.h>
#include <std_msgs/Float64.h>
#include "capturerobot_gazebo/ForceMsg.h"
#include "capturerobot_gazebo/VelMsg.h"
#include "capturerobot_gazebo/PosMsg.h"
#include "capturerobot_gazebo/PosError.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "std_msgs/Float64MultiArray.h"
#include "tf/transform_datatypes.h"



// 饱和度函数,返回值在-pi到pi之间
double min_periodicity(double theta_val)
{
    double periodic_val = theta_val;
    while (periodic_val > M_PI)
    {
        periodic_val -= 2 * M_PI;
    }
    while (periodic_val < -M_PI)
    {
        periodic_val += 2 * M_PI;
    }
    return periodic_val;
}

std::vector<double> pos_plan(13,0.0); // 轨迹规划输入值

// 接收轨迹信息的回调函数
void posCmdCB(const std_msgs::Float64MultiArrayConstPtr& pos_cmd_sub)
{
    pos_plan = pos_cmd_sub->data;
}

// 检测/gazebo/get_joint_properties服务是否存在
bool test_service()
{
    bool service_ready = false;
    // exists函数第二个参数为是否打印检测服务不可用的原因
    if (!ros::service::exists("/gazebo/get_joint_properties", true))
    {
        ROS_WARN("waiting for /gazebo/get_joint_properties service");
        return false;
    }
    ROS_INFO("services are ready");
    return true;
}

// 主函数
int main(int argc, char **argv)
{
    ros::init(argc, argv, "cap_controller");
    ros::NodeHandle nh;
    ros::Duration half_sec(0.5);

    // 从参数服务器读取参数值 
    double Kp, Kv, dt; 
    ros::param::get("/cap_controller/Kp", Kp);
    ros::param::get("/cap_controller/Kv", Kv);
    ros::param::get("dt", dt);

    // 确保服务可用，否则节点将崩溃
    while (!test_service())
    {
        ros::spinOnce();
        half_sec.sleep();
    }
    

    // 实例化接收baselink信息的客户端
    ros::ServiceClient get_link_state_client =
        nh.serviceClient<gazebo_msgs::GetLinkState>("/gazebo/get_link_state");
    
    gazebo_msgs::GetLinkState baselink_state_srv;

    // 实例化接收gazebo关节信息的客户端
    ros::ServiceClient get_jnt_state_client =
        nh.serviceClient<gazebo_msgs::GetJointProperties>("/gazebo/get_joint_properties");

    gazebo_msgs::GetJointProperties joint1_state_srv;
    gazebo_msgs::GetJointProperties joint11_state_srv;
    gazebo_msgs::GetJointProperties joint21_state_srv;
    gazebo_msgs::GetJointProperties joint31_state_srv;
    gazebo_msgs::GetJointProperties jointc12_state_srv;
    gazebo_msgs::GetJointProperties jointc23_state_srv;
    gazebo_msgs::GetJointProperties jointc31_state_srv;

    // 创建发布力&力矩的发布者(为闭环传输数据)
    ros::Publisher force_publisher =
        nh.advertise<std_msgs::Float64MultiArray>("/capturerobot/force_cmd", 10);
    
    // 创建发布力&位置(误差)&速度发布者(为画图提供topic)
    ros::Publisher ForceMsg_publisher =
        nh.advertise<capturerobot_gazebo::ForceMsg>("/cap_force", 10);
    ros::Publisher PosMsg_publisher = nh.advertise<capturerobot_gazebo::PosMsg>("/cap_position", 10);
    ros::Publisher VelMsg_publisher = nh.advertise<capturerobot_gazebo::VelMsg>("/cap_velocity", 10);
    ros::Publisher PosError_publisher = nh.advertise<capturerobot_gazebo::PosError>("/cap_pos_error", 10);
    

    
    
    // 创建接收轨迹规划路径的Subscriber
    ros::Subscriber pos_cmd_subscriber = nh.subscribe("/capturerobot/pos_cmd", 10, posCmdCB);

    // 定义变量
    capturerobot_gazebo::ForceMsg force_msg_pub;
    capturerobot_gazebo::VelMsg vel_msg_pub;
    capturerobot_gazebo::PosMsg pos_msg_pub;
    std_msgs::Float64MultiArray force_msg;
    capturerobot_gazebo::PosError pos_error_msg;
    std::vector<double> for_cmd(13,0.0), pos_temp(13,0.0), vel_temp(13,0.0); 
    ros::Duration duration(dt);
    ros::Rate rate_timer(1 / dt);
    baselink_state_srv.request.link_name = "capturerobot::basebody_and_platform::link_base_b";
    joint1_state_srv.request.joint_name = "capturerobot::basebody_and_platform::joint_1";
    joint11_state_srv.request.joint_name = "capturerobot::joint_11";
    joint21_state_srv.request.joint_name = "capturerobot::joint_21";
    joint31_state_srv.request.joint_name = "capturerobot::joint_31";
    jointc12_state_srv.request.joint_name = "capturerobot::joint_c12";
    jointc23_state_srv.request.joint_name = "capturerobot::joint_c23";
    jointc31_state_srv.request.joint_name = "capturerobot::joint_c31";

    tf::Quaternion quat;
    double roll, pitch, yaw;


    // 控制器闭环
    while (ros::ok())
    {
        // 向gazebo服务端请求link&joint信息
        get_link_state_client.call(baselink_state_srv);
        get_jnt_state_client.call(joint1_state_srv);
        get_jnt_state_client.call(joint11_state_srv);
        get_jnt_state_client.call(joint21_state_srv);
        get_jnt_state_client.call(joint31_state_srv);
        get_jnt_state_client.call(jointc12_state_srv);
        get_jnt_state_client.call(jointc23_state_srv);
        get_jnt_state_client.call(jointc31_state_srv);

        //星体姿态四元数转欧拉角
        tf::quaternionMsgToTF(baselink_state_srv.response.link_state.pose.orientation,quat);
        tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);

        // 将位置和速度再重新发布为话题
        pos_temp[0] = baselink_state_srv.response.link_state.pose.position.x;
        pos_temp[1] = baselink_state_srv.response.link_state.pose.position.y;
        pos_temp[2] = baselink_state_srv.response.link_state.pose.position.z;
        pos_temp[3] = roll;
        pos_temp[4] = pitch;
        pos_temp[5] = yaw;
        pos_temp[6] = joint1_state_srv.response.position[0];
        pos_temp[7] = joint11_state_srv.response.position[0];
        pos_temp[8] = joint21_state_srv.response.position[0];
        pos_temp[9] = joint31_state_srv.response.position[0];
        pos_temp[10] = jointc12_state_srv.response.position[0];
        pos_temp[11] = jointc23_state_srv.response.position[0];
        pos_temp[12] = jointc31_state_srv.response.position[0];

        pos_msg_pub.basebody_x = pos_temp[0];
        pos_msg_pub.basebody_y = pos_temp[1];
        pos_msg_pub.basebody_z = pos_temp[2];
        pos_msg_pub.basebody_phi = pos_temp[3];
        pos_msg_pub.basebody_theta = pos_temp[4];
        pos_msg_pub.basebody_psi = pos_temp[5];
        pos_msg_pub.joint1_angle = pos_temp[6];
        pos_msg_pub.joint11_angle = pos_temp[7];
        pos_msg_pub.joint21_angle = pos_temp[8];
        pos_msg_pub.joint31_angle = pos_temp[9];
        pos_msg_pub.jointc12_angle = pos_temp[10];
        pos_msg_pub.jointc23_angle = pos_temp[11];
        pos_msg_pub.jointc31_angle = pos_temp[12];
        PosMsg_publisher.publish(pos_msg_pub);

        vel_temp[0] = baselink_state_srv.response.link_state.twist.linear.x;
        vel_temp[1] = baselink_state_srv.response.link_state.twist.linear.y;
        vel_temp[2] = baselink_state_srv.response.link_state.twist.linear.z;
        vel_temp[3] = baselink_state_srv.response.link_state.twist.angular.x;
        vel_temp[4] = baselink_state_srv.response.link_state.twist.angular.y;
        vel_temp[5] = baselink_state_srv.response.link_state.twist.angular.z;
        vel_temp[6] = joint1_state_srv.response.rate[0];
        vel_temp[7] = joint11_state_srv.response.rate[0];
        vel_temp[8] = joint21_state_srv.response.rate[0];
        vel_temp[9] = joint31_state_srv.response.rate[0];
        vel_temp[10] = jointc12_state_srv.response.rate[0];
        vel_temp[11] = jointc23_state_srv.response.rate[0];
        vel_temp[12] = jointc31_state_srv.response.rate[0];

        vel_msg_pub.basebody_Vx = vel_temp[0];
        vel_msg_pub.basebody_Vy = vel_temp[1];
        vel_msg_pub.basebody_Vz = vel_temp[2];
        vel_msg_pub.basebody_Wx = vel_temp[3];
        vel_msg_pub.basebody_Wy = vel_temp[4];
        vel_msg_pub.basebody_Wz = vel_temp[5];
        vel_msg_pub.joint1_v = vel_temp[6];
        vel_msg_pub.joint11_v = vel_temp[7];
        vel_msg_pub.joint21_v = vel_temp[8];
        vel_msg_pub.joint31_v = vel_temp[9];
        vel_msg_pub.jointc12_v = vel_temp[10];
        vel_msg_pub.jointc23_v = vel_temp[11];
        vel_msg_pub.jointc31_v = vel_temp[12];
        VelMsg_publisher.publish(vel_msg_pub);

        //将位置误差信息发布为话题
        pos_error_msg.basebody_x_error = pos_plan.at(0) - pos_temp[0];
        pos_error_msg.basebody_y_error = pos_plan.at(1) - pos_temp[1];
        pos_error_msg.basebody_z_error = pos_plan.at(2) - pos_temp[2];
        pos_error_msg.basebody_phi_error = pos_plan.at(3) - pos_temp[3];
        pos_error_msg.basebody_theta_error = pos_plan.at(4) - pos_temp[4];
        pos_error_msg.basebody_psi_error = pos_plan.at(5) - pos_temp[5];
        pos_error_msg.joint1_angle_error = pos_plan.at(6) - pos_temp[6];
        pos_error_msg.joint11_angle_error = pos_plan.at(7) - pos_temp[7];
        pos_error_msg.joint21_angle_error = pos_plan.at(8) - pos_temp[8];
        pos_error_msg.joint31_angle_error = pos_plan.at(9) - pos_temp[9];
        pos_error_msg.jointc12_angle_error = pos_plan.at(10) - pos_temp[10];
        pos_error_msg.jointc23_angle_error = pos_plan.at(11) - pos_temp[11];
        pos_error_msg.jointc31_angle_error = pos_plan.at(12) - pos_temp[12];
        PosError_publisher.publish(pos_error_msg);


        //向控制指令话题发布消息
        for_cmd[0] = Kp * (pos_plan.at(0) - pos_temp[0]) - Kv * vel_temp[0];
        for_cmd[1] = Kp * (pos_plan.at(1) - pos_temp[1]) - Kv * vel_temp[1];
        for_cmd[2] = Kp * (pos_plan.at(2) - pos_temp[2]) - Kv * vel_temp[2];
        // for_cmd[0] = 0;
        // for_cmd[1] = 0;
        // for_cmd[2] = 0;
        for_cmd[3] = Kp * (min_periodicity(pos_plan.at(3) - pos_temp[3])) - Kv * vel_temp[3];
        for_cmd[4] = Kp * (min_periodicity(pos_plan.at(4) - pos_temp[4])) - Kv * vel_temp[4];
        for_cmd[5] = Kp * (min_periodicity(pos_plan.at(5) - pos_temp[5])) - Kv * vel_temp[5];
        for_cmd[6] = Kp * (min_periodicity(pos_plan.at(6) - pos_temp[6])) - Kv * vel_temp[6];
        for_cmd[7] = Kp * (min_periodicity(pos_plan.at(7) - pos_temp[7])) - Kv * vel_temp[7];
        for_cmd[8] = Kp * (min_periodicity(pos_plan.at(8) - pos_temp[8])) - Kv * vel_temp[8];
        for_cmd[9] = Kp * (min_periodicity(pos_plan.at(9) - pos_temp[9])) - Kv * vel_temp[9];
        for_cmd[10] = Kp * (min_periodicity(pos_plan.at(10) - pos_temp[10])) - Kv * vel_temp[10];
        for_cmd[11] = Kp * (min_periodicity(pos_plan.at(11) - pos_temp[11])) - Kv * vel_temp[11];
        for_cmd[12] = Kp * (min_periodicity(pos_plan.at(12) - pos_temp[12])) - Kv * vel_temp[12];
        
        // ROS_INFO("fz: %f; pos_x: %f; pos_y: %f; pos_z: %f.",
        //                 for_cmd[2],pos_temp[0],pos_temp[1],pos_temp[2]);
        force_msg_pub.basebody_Fx = for_cmd[0];
        force_msg_pub.basebody_Fy = for_cmd[1];
        force_msg_pub.basebody_Fz = for_cmd[2];
        force_msg_pub.basebody_Tx = for_cmd[3];
        force_msg_pub.basebody_Ty = for_cmd[4];
        force_msg_pub.basebody_Tz = for_cmd[5];
        force_msg_pub.joint1_T = for_cmd[6];
        force_msg_pub.joint11_T = for_cmd[7];
        force_msg_pub.joint21_T = for_cmd[8];
        force_msg_pub.joint31_T = for_cmd[9];
        force_msg_pub.jointc12_T = for_cmd[10];
        force_msg_pub.jointc23_T = for_cmd[11];
        force_msg_pub.jointc31_T = for_cmd[12];

        ForceMsg_publisher.publish(force_msg_pub);
        
        force_msg.data = for_cmd;
        force_publisher.publish(force_msg);

        ros::spinOnce();
        rate_timer.sleep();

    }
    return 0;
    

}