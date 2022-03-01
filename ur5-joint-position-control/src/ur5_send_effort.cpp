#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<iostream>
#include<sensor_msgs/JointState.h>

#include<std_msgs/Float64MultiArray.h>
using namespace std;

int main(int argv,char **argc){
    ros::init(argv,argc,"send_commands");
    ros::NodeHandle n;

    
    ros::Publisher effort_publisher= n.advertise<std_msgs::Float64MultiArray>("/send_effort",100);
    
    ros::Rate rate(0.5);

    while(ros::ok()){
        
        float fx,fy,fz;

        cout<<"Enter Force value"<<endl;
        cin>>fx>>fy>>fz;
        
        std_msgs::Float64MultiArray force;
        force.data.resize(3);

        force.data[0] = fx;
        force.data[1] = fy;
        force.data[2] = fz;

        effort_publisher.publish(force);

        ros::spinOnce();

        rate.sleep();
    }

    return 0;
}