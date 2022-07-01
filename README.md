# Assignment_3
## Introduction
For this assignment, I have developed a software architecture for the control of the robot in the environment. 
I have controlled 3 different behaviours of the robot, depending on user’s input, in particular, if the user enters the letter:
*	“a”, the robot has to autonomously reach a (x, y) coordinate inserted by the user,
*	“m”, the user can manually drive the robot using the keyboard,
*	“c”, the user can manually drive the robot using the keyboard but there are some controls in order to avoid collision. 

## How to run the code?
First of all you need the packages: 
*	final_assignment,
*	slam_gmapping.
Then, in order to run the code, you have to run:
*	roslaunch final_assignment simulation_gmapping.launch,
*	roslaunch final_assignment move_base.launch.
And finally:
*	rosrun final_assignment goal_node
