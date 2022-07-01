// Assignment 3: software architecture for the control of the robot in the environment.
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

using namespace std;

// global variable definition:

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

geometry_msgs::Twist base_cmd;
sensor_msgs::LaserScan scan;
ros::Publisher scan_pub;
ros::Publisher pub;


float x,y;
char cmd;
char input;	
float lin_vel, ang_vel;
float value_front, value_right, value_left;
int timer; 

void distCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	// compute the distances fromt the obstacles on robot's front, robot's right, robot's left
	value_front = scan->ranges[360];
	value_right = scan->ranges[72];
	value_left = scan->ranges[650];
	
	if(cmd!='+' && cmd!='l' && cmd!='r' &&  cmd!='s' && cmd!='.')
	{
		cout<<"Unknown command: "<<cmd<<endl;
	}
			
	base_cmd.linear.x=base_cmd.linear.y=base_cmd.angular.z=0;
	
	// if the user enters "+" 	
	if(cmd=='+')
	{
		ROS_INFO("control the distance from the obstacles in front of the robot");
		
		// if the obstacle in front of the robot is near
		if(value_front< 1.5)
		{
			// stop the robot
			ROS_INFO("the robot has an obstacle in front of him: it can't move");
			base_cmd.linear.x =0;
			pub.publish(base_cmd);
		}
		
		// if there are no obstacles nearby in front of the robot
		else
		{
			// the robot moves on a straight line
			ROS_INFO("no obstacles in the front, proceed");
			base_cmd.linear.x=0.5;
			pub.publish(base_cmd);
		}
		
		cin>>cmd;
	}
	
	// if the user enters "l"
	else if(cmd=='l')
	{
		ROS_INFO("control the distance from the obstacles on the robot's left");
		
		// if there is an obstacle nearby on the robot's left
		if(value_left<1.5)
		{
			// the robot turns right
			ROS_INFO("The robot has an obtascle on the left, so it turns right");
			base_cmd.angular.z=-0.75;
			base_cmd.linear.x=0.1;
			pub.publish(base_cmd);
		}
		// if there are no obstacles nearby on the robot's left
		else
		{
			// the robot turns right
			ROS_INFO("no obstacles on the left, proceed");
			base_cmd.angular.z=0.75;
			base_cmd.linear.x=0.1;
			pub.publish(base_cmd);
		}
		
		cin>>cmd;
	}
	
	// if the user enters "r"
	else if(cmd=='r')
	{
		ROS_INFO("control the distance from the obstacles on the robot's right");
		
		// if there is an obstacle on the robot's right
		if(value_right <1.5)
		{
			// the robot turns left	
			ROS_INFO("The robot has an obtascle on the right, so it turns left");
			base_cmd.angular.z=0.75;
			base_cmd.linear.x=0.1;
			pub.publish(base_cmd);
		}
		
		// if there are no obstacles nearby on the robot's right
		else
		{
			// the robot turns right
			ROS_INFO("no obstacles on the right, proceed");
			base_cmd.angular.z = -0.75;
			base_cmd.linear.x = 0.1;
			pub.publish(base_cmd);
		}
		
		cin>>cmd;
	}
		
	// if the user enters "s"	
	else if(cmd=='s')
	{
		// stop the robot
		ROS_INFO("stop the robot");
		base_cmd.linear.x = 0;
		base_cmd.angular.z = 0;
		pub.publish(base_cmd);
		
		cin>>cmd;
			
	}
	
	
	
					
}
		


int main(int argc, char** argv){

	ros::init(argc, argv, "goal_node");
	
	ros::NodeHandle n;
	ros::spinOnce();
	
	
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
	
	// create a publisher and a subscriber to the topic /scan
	
	scan_pub = n.advertise<sensor_msgs::LaserScan>("/scan",1);
	
	ros::Subscriber scan_sub = n.subscribe("/scan", 10, distCallback);
		
	
	while(n.ok())
	{
	// the user has to choose from keyboard how to move the robot
	cout<<"Choose how to move: a for automatic guide, m for manual guide, c if you want an assisted guide"<<endl;
	cin>>input;
    
    	// if the user chooses the automatic guide:
    	if (input=='a')
	{
     		// construct an action client used to communicate with the action named "move_base"
       	MoveBaseClient ac("move_base", true);
   
     		// wait for the action server to come up
    		while(!ac.waitForServer(ros::Duration(5.0))){
      			 ROS_INFO("Waiting for the move_base action server to come up");
     		}
   
   		// create a goal to send to the move_base
     		move_base_msgs::MoveBaseGoal goal;
   
		// set the coordinate frame w.r.t the robot moves
     		goal.target_pose.header.frame_id = "map";
     		goal.target_pose.header.stamp = ros::Time::now();
     		goal.target_pose.pose.orientation.w = 1.0;
     		
     		// the user enters the desired target:
     		cout<<"where do you want the robot to go?"<<endl;
		cout<<"x= "<<endl;
		cin>>x;
		goal.target_pose.pose.position.x=x;
		cout<<"y= "<<endl;
		cin>>y;
		goal.target_pose.pose.position.y=y;
   		
   		// control on the target entered
   		// if the target entered is not reachable, it will be deleted
   		if ((x<-5.0) || (x>5.0) || (y>8.2) || (y<-8.0) || (x<1 && y<-6) || (x==1 && y>0.0) || (x>2.5 && y>2.5) || ((x>=0 && x<=3) && y>-0.1) || (x<=0 && y<=-5.3) || ((x>=2 && x<=3) && (y>=-4 && y<=-1.3)) || (x>=2 && (y>=-2.2 && y<=-1.3)) || (x>=1.5 && (y>-6.2 && y<-5.3)) || (x<=-2 && (y>0 && y<0.8)) || (x<=-2 &&(y>2 && y<3.5)) || ((x>=-2 && x<=-1.5) && (y>=2 && y<=3.3)))
   		{
   			ROS_INFO("goal non reachable: the entered target will be deleted");
   			ac.cancelAllGoals();
   			ros::Duration(1).sleep(); // wait 1 sec
   			
   		}
   		
   		// otherwise, if the entered target is reachable
   		else
   		{
   			// send the goal to the move_base
     			ROS_INFO("Sending goal");
     			ac.sendGoal(goal);
   
   			// wait for the goal to finish: wait for up to 30.0
     			bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));
     				
   			// check if the goal succeeded or failed
     			if(finished_before_timeout)
     			{
     				actionlib::SimpleClientGoalState state = ac.getState();
       			ROS_INFO("Hooray, the robot has reached the target");
       		}
     			else
       			ROS_INFO("The robot has not succeeded in reaching the target");
       			ac.cancelAllGoals();
       	}
   	}
   	
   	// if the user chooses the manual guide:
   	else if (input=='m')
   	{
   		// the user has to move manually the robot using the keyboard
   		cout<<"Type a command and then press enter: use '+' to more forward, 'l' to turn left, 'r' to turn right, 's' to stop the robot, '.' to exit"<<endl;
   		cin>>cmd;
   	
   		while(n.ok())
		{
			if(cmd!='+' && cmd!='l' && cmd!='r' && cmd!='s' && cmd!='.')
			{
				cout<<"Unknown command: "<<cmd<<endl;
			}
			
			base_cmd.linear.x=base_cmd.linear.y=base_cmd.angular.z=0;
			
			// if the user enters "+"
			if(cmd=='+')
			{
				// move the robot on a straight line
				base_cmd.linear.x=0.5;
				pub.publish(base_cmd);
				cin>>cmd;
			}
			
			// if the user enters "l"
			else if(cmd=='l')
			{
				// turn left
				base_cmd.angular.z=0.75;
				base_cmd.linear.x=0.5;
				pub.publish(base_cmd);
				cin>>cmd;
			}
			
			// if the user enters "r"
			else if(cmd=='r')
			{
				// turn right
				base_cmd.angular.z = -0.75;
				base_cmd.linear.x = 0.5;
				pub.publish(base_cmd);
				cin>>cmd;
			}
			
			// if the user enters "s"
			else if(cmd=='s')
			{
				// stop the robot
				base_cmd.linear.x = 0;
				base_cmd.angular.z = 0;
				pub.publish(base_cmd);
				cin>>cmd;
			
			}
			
			// if the user enters "."
			else if(cmd=='.')
			{
				// change the guide mode
				break;
			}
			
		}
	}
	
	// if the user chooses the controlled guide:
   	else if (input =='c')
   	{
   		
   		// the user has to move manually the robot using the keyboard, but there are some controls in order to avoid obstacles
   		cout<<"Type a command and then press enter: use '+' to more forward, 'l' to turn left, 'r' to turn right, 's' to stop the robot, '.' to exit"<<endl;
   		cin>>cmd;
   	
   	
   		while(n.ok())
		{
			// execute the callback
			ros::spinOnce();
			
			// if the user enters "."
			if(cmd=='.')
			{
				// exit from this modality
				base_cmd.linear.x = 0;
				base_cmd.angular.z = 0;
				pub.publish(base_cmd);
				break;
			}
   		}
   	}
   	
	}

	return 0;

}

