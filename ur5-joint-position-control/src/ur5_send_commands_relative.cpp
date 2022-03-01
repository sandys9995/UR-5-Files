#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<iostream>
#include<sensor_msgs/JointState.h>


using namespace std;

class CurrentAngle{
    public:
        float current_angle[6];
        
        void change_current_angle(const sensor_msgs::JointState::ConstPtr& msg){
            for(int i=0;i<6;i++){
                current_angle[i] = msg->position[i];
            }
        }
};


int main(int argv,char **argc){
    ros::init(argv,argc,"send_commands");
    ros::NodeHandle n;

    CurrentAngle current_pos;

    ros::Subscriber current_angle_sub = n.subscribe("/joint_states",1000, &CurrentAngle::change_current_angle, &current_pos);

    ros::Publisher joint1_publisher= n.advertise<std_msgs::Float64>("/shoulder_pan_joint_position_controller/command",100);
    ros::Publisher joint2_publisher= n.advertise<std_msgs::Float64>("/shoulder_lift_joint_position_controller/command",100);
    ros::Publisher joint3_publisher= n.advertise<std_msgs::Float64>("/elbow_joint_position_controller/command",100);
    ros::Publisher joint4_publisher= n.advertise<std_msgs::Float64>("/wrist_1_joint_position_controller/command",100);
    ros::Publisher joint5_publisher= n.advertise<std_msgs::Float64>("/wrist_2_joint_position_controller/command",100);
    ros::Publisher joint6_publisher= n.advertise<std_msgs::Float64>("/wrist_3_joint_position_controller/command",100);

    ros::Rate rate(100);
    char repeat = 'Y';

   while(ros::ok()){
        while(repeat=='Y' or repeat=='y'){
            std_msgs::Float64 theta1,theta2,theta3,theta4,theta5,theta6;
          
            int t=0,a,b,c,d,e,f,n=0;

            cout<<"Enter values for time"<<endl;
            cin>>n;
            cout<<"Enter values for base joint between  -100 to +100"<<endl;
            cin>>a;
            cout<<"Enter values for shoulder_lift_joint_position between  -100 to +100"<<endl;
            cin>>b;
            cout<<"Enter values for elbow_joint between  -100 to +100"<<endl;
            cin>>c;
            cout<<"Enter values for wrist_1_joint between  -100 to +100"<<endl;
            cin>>d;
            cout<<"Enter values for wrist_2_joint between  -100 to +100"<<endl;
            cin>>e;
            cout<<"Enter values for wrist_3_joint between  -100 to +100"<<endl;
            cin>>f;


                
              while(t!=n)
              {
                theta1.data= sin(a/100)*(2*3.145);
                theta1.data -= current_pos.current_angle[0]; 

                theta2.data= sin(b/100)*(3.145/2)-2;
                theta2.data -= current_pos.current_angle[1]; 

                theta3.data= sin(c/100)*(3.145/2);
                theta3.data -= current_pos.current_angle[2]; 

                theta4.data= sin(d/100)*(2*3.145);
                theta4.data -= current_pos.current_angle[3]; 

                theta5.data= sin(e/100)*(2*3.145);
                theta5.data -= current_pos.current_angle[4]; 

                theta6.data= sin(f/100)*(2*3.145);
                theta6.data -= current_pos.current_angle[5]; 

                joint1_publisher.publish(theta1);
                joint2_publisher.publish(theta2);
                joint3_publisher.publish(theta3);
                joint4_publisher.publish(theta4);   
                joint5_publisher.publish(theta5);             
                joint6_publisher.publish(theta6); 
                cout<<"hello"<<endl;
                

                    t+=1;
            
             }            

            cout<<"Want to play more, enter Y/N"<<endl;
            cin>>repeat;
            if(repeat == 'N' or repeat=='n')
            {
                ros::shutdown();
            }

        }

        ros::spinOnce();

        rate.sleep();
    
    }
    ros::spin();
    
    return 0;
}