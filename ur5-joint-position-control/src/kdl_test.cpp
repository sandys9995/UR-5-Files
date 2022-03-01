#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainidsolver_recursive_newton_euler.hpp>
#include <kdl/tree.hpp>
#include <kdl_parser/kdl_parser.hpp>
#include<iostream>
#include "math.h"
#include <ros/package.h>
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include "ros/ros.h"
#include<std_msgs/Float64MultiArray.h>
#include<sensor_msgs/JointState.h>

class GetEffort{
    public:
        float effort[3];
        
        void get_effort(const std_msgs::Float64MultiArray::ConstPtr& msg){
            for(int i=0;i<3;i++){
                effort[i] = msg->data[i];
            }
        }
};

class CurrentAngle{
    public:
        float current_angle[6];
        
        void get_angle(const sensor_msgs::JointState::ConstPtr& msg){
            for(int i=0;i<6;i++){
                current_angle[i] = msg->position[i];
            }
        }
};

const int Joints = 6;
KDL::JntArray jnt_pos_start(Joints);

const int loop_rate_val = 100;

int main(int argv, char **argc)
{

    ros::init(argv,argc, "tcp_control");

	ros::NodeHandle n;

	ros::Rate loop_rate(loop_rate_val);

    GetEffort geteffort;

	ros::Subscriber effort_sub = n.subscribe("/send_effort", 1000, &GetEffort::get_effort, &geteffort);

    CurrentAngle current_pos;

    ros::Subscriber current_angle_sub = n.subscribe("/joint_states",1000, &CurrentAngle::get_angle, &current_pos);

	//Create publishers to send position commands to all joints
	ros::Publisher joint_com_pub[6]; 
	joint_com_pub[0] = n.advertise<std_msgs::Float64>("/shoulder_pan_joint_position_controller/command", 1000);
	joint_com_pub[1] = n.advertise<std_msgs::Float64>("/shoulder_lift_joint_position_controller/command", 1000);
	joint_com_pub[2] = n.advertise<std_msgs::Float64>("/elbow_joint_position_controller/command", 1000);
	joint_com_pub[3] = n.advertise<std_msgs::Float64>("/wrist_1_joint_position_controller/command", 1000);
	joint_com_pub[4] = n.advertise<std_msgs::Float64>("/wrist_2_joint_position_controller/command", 1000);
	joint_com_pub[5] = n.advertise<std_msgs::Float64>("/wrist_3_joint_position_controller/command", 1000);


    int mNumJnts = 6;
    KDL::Chain chain;
    KDL::Tree my_tree;
    KDL::JntArray jnt_q(mNumJnts);
    KDL::JntArray jnt_qd(mNumJnts);
    KDL::JntArray jnt_qdd(mNumJnts);
    KDL::JntArray jnt_taugc(mNumJnts);

    int ret;

    // std::string full_path = "ur5-joint-position-control/urdf/ur5_jnt_pos_ctrl.urdf";
	std::string full_path = ros::package::getPath("ur5-joint-position-control");
	full_path += "/urdf/ur5_jnt_pos_ctrl.urdf";

    if (!kdl_parser::treeFromFile(full_path, my_tree))
    {
        std::cout<<" treeFromFile Failed"<<std::endl;
        return -1;
    }
    my_tree.getChain("base_link", "wrist_3_link", chain);
    std::cout <<"Kinematic chain expects "<< chain.getNrOfJoints() <<" joints"<<std::endl;
    std::cout <<"Kinematic chain expects "<< chain.getNrOfSegments() <<" segments"<<std::endl;

    for(int i=0; i< 2; i++) {
            ros::spinOnce();
            loop_rate.sleep();
        }
    while (ros::ok()){
        
        KDL::Wrenches jnt_wrenches;
        for (int i = 0; i < mNumJnts; i++) {
            KDL::Wrench w;
            if(i==5){
                w(0) = geteffort.effort[0];
                w(1) = geteffort.effort[1];
                w(2) = geteffort.effort[2];
            }
            
            jnt_wrenches.push_back(w);
        }

        
        jnt_q(0) = current_pos.current_angle[0];
        jnt_q(1) = current_pos.current_angle[1];
        jnt_q(2) = current_pos.current_angle[2];
        jnt_q(3) = current_pos.current_angle[3];
        jnt_q(4) = current_pos.current_angle[4];
        jnt_q(5) = current_pos.current_angle[5];
        

        // Compute Dynamics 
        // KDL::Vector gravity(0.0, 0.0, -9.81);
        KDL::Vector gravity(-9.81, 0.0, 0.0);
        KDL::ChainIdSolver_RNE gcSolver = KDL::ChainIdSolver_RNE(chain, gravity);
        ret = gcSolver.CartToJnt(jnt_q, jnt_qd, jnt_qdd, jnt_wrenches,jnt_taugc);
        if (ret < 0){
            std::cout<<"CartToJnt Failed"<<std::endl;
            return -1;
        }else{
            for (int i = 0; i < mNumJnts; i++) {
            std::cout <<"torque " << i <<" "<<jnt_taugc(i)<<std::endl;
                }
        }

        std_msgs::Float64 theta1,theta2,theta3,theta4,theta5,theta6;

        theta1.data = jnt_taugc(0);
        theta2.data = jnt_taugc(1);
        theta3.data = jnt_taugc(2);
        theta4.data = jnt_taugc(3);
        theta5.data = jnt_taugc(4);
        theta6.data = jnt_taugc(5);
        
        joint_com_pub[0].publish(theta3);  //check on which joint torque is given
        joint_com_pub[1].publish(theta2);
        joint_com_pub[2].publish(theta1);  
        joint_com_pub[3].publish(theta4);   
        joint_com_pub[4].publish(theta5);             
        joint_com_pub[5].publish(theta6);

        ros::spinOnce();
        loop_rate.sleep();
        
    }
    return 0;
}