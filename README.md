# Assignment_3
## Introduction
For this assignment, I have developed a software architecture for the control of the robot in the environment. 
I have controlled 3 different behaviours of the robot, depending on user’s input, in particular, if the user enters the letter:
*	“a”, the robot has to autonomously reach a (x, y) coordinate inserted by the user,
*	“m”, the user can manually drive the robot using the keyboard,
*	“c”, the user can manually drive the robot using the keyboard but there are some controls in order to avoid collision. 

## How to run the code?
First of all you need the packages: 
*	final_assignment
*	slam_gmapping

Then, in order to run the code, you have to run:
*	roslaunch final_assignment simulation_gmapping.launch
*	roslaunch final_assignment move_base.launch

And finally:
*	rosrun final_assignment goal_node

## Description of the code
In order to implement the “automatic guide” I have used the move_base package, which requires goal to be sent to the topic move_base/goal; in particular I have used a MoveBaseAction interface, where the move_base action accepts goals from clients and attempts to move the robot to the specified position in the world. 

Since the user can enter a not reachable target, I have inserted a control which checks the reachability of the position, and in the case in which the position has not been reached it cancells the goal. Moreover, I have inserted a timeout in order to cancel the goal if the request can’t be accomplished. 

For the “manual guide”, instead, I have build some commands in order to move manually the robot using “geometry_msgs/Twist.h”; so, if the user enters:
*	“+”, the robot proceeds in a straight line,
*	“l”, the robot turns left,
*	“r”, the robot turns right,
*	“s”, the robot stops,
*	“.”, the manual guide mode is exited.

For the “controlled guide”, instead, I have kept the commands of the “manual guide”, but I have inserted some controls releted to the distance from the obstacles. To do that I have a Publisher and a Subscriber to the topic /scan of the robot, so that if the user enters:
*	“+”, but the distance from the obstacles in front of the robot is less than a threshold (I have choose 1.5), then the robot doesn’t proceed,
*	“l”, but the distance from the obstancles on the left of the robot is less than the threshold, it doesn’t turn left, but right in order to avoid collision,
*	“r”, but the distance from the obstancles on the right of the robot is less than the threshold, it doesn’t turn right, but left in order to avoid collision. 

## Flowchart
