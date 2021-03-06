#include "map_reader.h"
#include "global_path_finder.h"
#include <ros/ros.h>
#include <iostream>
#include <vector>
#include "PotentialField.h"


int main(int argc, char** argv){
    ros::init(argc, argv, "nav_node");
    ros::NodeHandle nh;
    MapReader map(&nh);

    // //============================Map Test Section===============================

    // // Test get_occ_val for global occupancy grid 
    // ROS_INFO("Testing get_occ_val...");
    // for(int i=500; i<=600; i++){
    //     std::vector<double> i_double = {i*0.3, 250*0.3};
    //     ROS_INFO("Current occ value is: %d", map.get_occ_val(i_double));
    // }

    // // Test local occupancy grid
    // ROS_INFO("Testing get_local_occ_grid...");
    // std::vector<double> xy_double = {150.29, 75.96};
    // std::vector<std::vector<bool>> local_grid = map.get_local_occ_grid(xy_double, 0.5);
    // ROS_INFO("local map grid value:");
    // std::vector<std::vector<bool>>::iterator row;
    // std::vector<bool>::iterator col;
    // for(row = local_grid.begin(); row != local_grid.end(); row++){
    //     for(col = row->begin(); col != row->end(); col++){
    //         std::cout << "Current local grid value is: " << *col << std::endl;
    //     }
    // }

    // // Test get_curr_grid_pos
    // ROS_INFO("Testing get_curr_grid_pos");
    // std::vector<int> curr_grid_pos = map.get_curr_grid_pos(xy_double);
    // ROS_INFO("Current grid position at: (%d, %d)", curr_grid_pos[0], curr_grid_pos[1]);

    // // Test get_coord_from_idx and get_idx_from_coord
    // ROS_INFO("Testing get_coord_from_idx");
    // std::vector<int> coord = map.get_coord_from_idx(1230);
    // ROS_INFO("Current idx is %d and cell coord is (%d , %d)", 1230, coord[0], coord[1]);

    // ROS_INFO("Testing get_idx_from_coord");
    // int curr_idx = map.get_idx_from_coord(coord);
    // ROS_INFO("Current coord is (%d, %d), and current idx is: %d", coord[0], coord[1], curr_idx);

    //============================Dijkstra Test Section===========================
    GlobalPathFinder dijkstra_finder(&nh, "dijkstra", &map);

    // // Test path finding function
    // ROS_INFO("Testing find_path function");

    // std::vector<double> start = {275 * 0.3, 240 * 0.3};
    // std::vector<double> goal = {255 * 0.3, 545 * 0.3};
    // double map_offset_x = 115.0;
    // double map_offset_y = 115.0; 
    
    // std::vector<double> start = {77.0, 240 * 0.3};
    // std::vector<double> goal = {173.0, 550 * 0.3};    
    // std::vector<double> start = {50.45 + map_offset_x, -51.25 + map_offset_y};
    //std::vector<double> goal = {140.0, -51.59 + map_offset_y};
    // std::vector<double> goal = {-50.45 + map_offset_x, 51.25 + map_offset_y}; 
    
    // std::vector<int> start_discre = map.get_curr_grid_pos(start);
    // std::vector<int> goal_discre = map.get_curr_grid_pos(goal);

    //dijkstra_finder.find_path(start, goal);
    //nav_msgs::Path path = dijkstra_finder.get_path();
    // ROS_INFO("Start at (%.2f, %.2f). Goal at (%.2f, %.2f)", start[0], start[1], goal[0], goal[1]);
    // ROS_INFO("Start at (%d, %d). Goal at (%d, %d)", start_discre[0], start_discre[1], goal_discre[0], goal_discre[1]);
    // ROS_INFO("Found Path: ");
    // for(int i = 0; i < path.poses.size(); i++){
    //     ROS_INFO("Path Cell Coordinate: (%.2f, %.2f)", path.poses[i].pose.position.x, path.poses[i].pose.position.y);
    // }

    // dijkstra_finder.publish_path();

    // PotentialField PF(path, map);
    // PF.init();
    // PF.run();
    // bool flag = true;


    
    PotentialField PF(map);
    PF.init();
    //===========================Test Publish Topic and Visualization===============
    // ROS_INFO("Start Publishing...............");
    ros::Rate loop_rate(5);
    while(ros::ok()){
        nav_msgs::Path path;
        ROS_INFO("Please specify a goal in Rviz ...");
        while(1){
            path = dijkstra_finder.fireup();
            ros::spinOnce();
            loop_rate.sleep();
            if(path.poses.size() > 1) break;
        }
        dijkstra_finder.publish_path();
    
        // ROS_INFO("Found Path: ");
        // for(int i = 0; i < path.poses.size(); i++){
        //     ROS_INFO("Path Cell Coordinate: (%.2f, %.2f)", path.poses[i].pose.position.x - 115.0, path.poses[i].pose.position.y - 115.0);
        // }

        PF.getNewPath(path);
        PF.run();
        ros::spinOnce();
        loop_rate.sleep();
    }

}
