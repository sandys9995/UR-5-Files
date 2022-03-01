#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<iostream>

using namespace std;

int main(int argv,char **argc){
    ros::init(argv,argc,"send_initial_effort");

    ros::NodeHandle n;

    ros::Publisher joint1_publisher= n.advertise<std_msgs::Float64>("/shoulder_pan_joint_position_controller/command",100);
    ros::Publisher joint2_publisher= n.advertise<std_msgs::Float64>("/shoulder_lift_joint_position_controller/command",100);
    ros::Publisher joint3_publisher= n.advertise<std_msgs::Float64>("/elbow_joint_position_controller/command",100);
    ros::Publisher joint4_publisher= n.advertise<std_msgs::Float64>("/wrist_1_joint_position_controller/command",100);
    ros::Publisher joint5_publisher= n.advertise<std_msgs::Float64>("/wrist_2_joint_position_controller/command",100);
    ros::Publisher joint6_publisher= n.advertise<std_msgs::Float64>("/wrist_3_joint_position_controller/command",100);

    ros::Rate rate(100);

    
    while(ros::ok()){
    
        std_msgs::Float64 theta1,theta2,theta3,theta4,theta5,theta6;

        theta1.data = 135;
        theta2.data = -90;
        theta3.data = -6;
        theta4.data = -4;
        theta5.data = -0.3;
        theta6.data = 0.15;
        

        joint1_publisher.publish(theta1);
        joint2_publisher.publish(theta2);
        joint3_publisher.publish(theta3);
        joint4_publisher.publish(theta4);   
        joint5_publisher.publish(theta5);             
        joint6_publisher.publish(theta6);             

        ros::spinOnce();

        rate.sleep();

    }

    return 0;
}