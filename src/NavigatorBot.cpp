/*
*	Yosef Silberhaft
*	210028924
*	NavigatorBot.cpp
*/
#include "Navigator.h"
#include <stdlib.h>
#include <fstream>
#include <nav_msgs/GetMap.h>

NavigatorPath::NavigatorPath() {

}

/*int main(int argc, char **argv){
	double robot_size;

	std::vector<Location> starting_location;;
	ros::init(argc, argv, "navigation_planner");


	ros::NodeHandle nh;

	NavigatorPath navigatorPath;


	std::vector<double> location_start, locations_goal_location;
	string starting;
	nh.getParam("starting_location", starting);

	nh.getParam("starting_location", location_start);
	nh.getParam("goal_location", locations_goal_location);
	nh.getParam("robot_size", robot_size);
	
	//double start_x = atof(starting.at(0));
	//double start_y = atof(starting.at(2));

	//starting_location.push_back(Location(start_x, start_y));

}*/

bool NavigatorPath::requestMap(ros::NodeHandle &nh) {
	nav_msgs::GetMap::Request req;
	nav_msgs::GetMapResponse res;

	while(!ros::service::waitForService("static_map", ros::Duration(3.0))){
		ROS_INFO("Waiting for static_map to become available");
	}

	ROS_INFO("Requesting the map...");
	ros::ServiceClient mapClient = nh.serviceClient<nav_msgs::GetMap>("static_map");
	if(mapClient.call(req, res)){
		readMap(res.map);
		return true;
	}
	else{
		ROS_ERROR("Failed to call map service");
		return false;
	}
}

void NavigatorPath::readMap(const nav_msgs::OccupancyGrid& map) {
	ROS_INFO("Received a %d X %d map @ %.3f m/px\n", map.info.width, map.info.height, map.info.resolution);

	rows = map.info.height;
	cols = map.info.width;
	mapResolution = map.info.resolution;
	int currCell = 0;

	// Dynamically resize the grid
	grid.resize(rows);
	for (int i = 0; i < rows; i++) {
		grid[i].resize(cols);
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
            grid[i][j] = map.data[currCell] != 0; // occupied (100) or unknown cell (-1)
			currCell++;
		}
	}
}