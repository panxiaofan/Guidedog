#pragma once

#include <string>
#include <list>
#include <set>
#include <utility>
#include "map_reader.h"
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>

 

class GlobalPathFinder{
    private:
    std::string method;
    std::list<std::pair<int, int>> parents;  // first int stores parent index, second stores cell index.
    std::set<std::pair<int, int>> cost_min_heap;  // Stores the unvisited cells. first int stores weight, second stores cell index.
    std::list<std::pair<int, int>> curr_adjacent_list;  // first int stores distance, second stores cell index.
    std::vector<int> cost;  // cost or weight of each cell w.r.t start point
    //std::vector<std::vector<int>> path;  // shortest path
    MapReader* map;
    nav_msgs::Path path;
    ros::Publisher path_pub;
    std::string path_topic = "/global_path";
    ros::NodeHandle *nh;

    public:
    GlobalPathFinder(ros::NodeHandle *nh, const std::string &method, MapReader* input_map);
    void add_adjacent_cell(const int &cell_idx);
    // std::vector<std::vector<int>> find_path(const std::vector<int> start_cell_coord, const std::vector<int> goal_cell_coord);
    nav_msgs::Path find_path(const std::vector<double> &start_cell, const std::vector<double> &goal_cell);
    void publish_path();
};