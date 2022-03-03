# UR-5-Files
-------
First try to install the following packages.

Using apt (Ubuntu, Debian) On supported Linux distributions (except noetic):
1) sudo apt-get install ros-$ROS_DISTRO-universal-robot 

note: If you are a noetic user, try with any of the distribution except noetic while intstalling the Universal robot package in the beginning.


After this:
1) sudo apt-get install ros-noetic-ros-control ros-noetic-ros-controllers

NOW,
Run the following commands for positioning,
1) roslaunch ur5-joint-position-control ur5_joint_position_control.launch 
2) roslaunch ur5-joint-position-control send_commands_relative.launch ( to move the joint in joint frame)
3) roslaunch ur5-joint-position-control send_commands_absolute.launch ( to move the joint in fixed frame)

To control our robot we used PID controller and callibrate it with the help of rqt.
1) rosrun rqt_gui rqt_gui
2) The computation for our PID controller output τ which is the effort that is applied on the robot joints looks something like this:
   τ=Kp∗perror+Ki∗∫t0perror(t)–Kd∗v
 
Secondly,
To move the end-effector by applied effort.
1) roslaunch ur5-joint-position-control ur5_joint_effort_control.launch
2) rosrun ur5-joint-position-control send_effort_node

