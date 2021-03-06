#ifndef POTENTIALFIELD
#define POTENTIALFIELD
#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <map_reader.h>
#include <angles/angles.h>
#include <math.h>
#include <set>

#include <planning_stack/ped.h>

#define pi 3.141592653589793

class PotentialField{
    public:
    // PotentialField(nav_msgs::Path gp, MapReader m);
    PotentialField(MapReader m);     
    void getNewPath(nav_msgs::Path new_path);  
    void init();
    void run();
    void run_test();

    private:    
    bool use_cam_info = false;
    bool manual_goal = false;
    bool DI = false;  
    bool wall = true;
    bool velo = true;
    
      
    MapReader map;   
    double map_offset_x = 115.0;
    double map_offset_y = 115.0;  
    double map_resolution = 0.1;
    double local_map_size = 1.5; // half of side length 

    // Parameters for tuning ----------------------------------------------------------
    double run_freq = 30;  

    double horizon_r = 5.0;     // robot vision horizon
    double bot_r = 0.2;     // robot radius
    double check_arrive_threshold = 3.0;    //waypoint tolerance
    

    // attraction field pararmeters ---
    double att_scale = 5.0;   //Fatt intensity
    double att_r = 2.0;     // parabolic to conic well boundary
    double att_const = att_scale * att_r;   //   

    // pedestrian repulsive field parameters ---
    double rep_scale_p = 2.0;     //Frep intneisty       // integrated demo parameters ---
    double ped_r = 0.35;     // ped radius
    double rep_r_p = 0.2;     // extra radius of the repulsive field    
    // double rep_scale_p = 500.0;     //Frep intneisty      // DI test parameters ---
    // double ped_r = 0.35;     // ped radius
    // double rep_r_p = 1.0;     // extra radius of the repulsive field
    
    // wall repulsive field parameters ---
    double rep_scale_w = 15.0;
    double wall_r = map_resolution * sqrt(2.0) / 2.0;
    double rep_r_w = 0.5;   // extra radius of the repulsize field 

    // danger index field paremeters ---
    double DI_scale = 20.0;
    double rhocmin = bot_r + ped_r + 0.2;   // closest distance of the centers of the robot and the pedestrian can get
    double rhocmax = 1.8;       // DI start distance
    double eta = (rhocmin * rhocmax) / (rhocmax - rhocmin); 
    double epsilon = 1.5;   // larger, robot more conservative; smaller, more aggressive

    // velocity repulsive ---
    double rep_scale_v = 1.1;
    double rep_r_v = 2.5;

    double dodge_min = 1.0;
    double dodge_max = 4.0;

    ros::NodeHandle n;

    nav_msgs::Path sparse_global_path;
    nav_msgs::Path dense_global_path;
    std::string path_topic = "global_path";
    ros::Publisher path_pub;  

    std::string odom_topic_name = "odom";
    ros::Subscriber odom_sub;
    
    std::string relative_location_topic_name = "rlocation";
    ros::Subscriber rlocation_sub;
    
    std::string force_cmd_name = "PFforce";
    ros::Publisher force_pub;     // com_vel publisher    
    geometry_msgs::Pose2D decision_signal;
    double far_avoid_flag = 0.0;
    // std::vector<geometry_msgs::Pose2D> pedRlocations;   //relative position of the peds to the robot
    // std::vector<geometry_msgs::Pose2D> pedRlocations_pre;   //relative position of the peds to the robot
    std::vector<planning_stack::ped> ped_info;
    std::vector<planning_stack::ped> ped_info_pre;
    std::vector<planning_stack::ped> ped_info_1;
    std::vector<planning_stack::ped> ped_info_2;
    std::vector<planning_stack::ped> ped_info_3;
    double cam_angle = 80.0;
    double sin_cam = sin(cam_angle / 180.0 * pi);
    double cos_cam = cos(cam_angle / 180.0 * pi);
    std::vector<int> peds_in_range;
    std::set<std::string> ped_set;
    std::set<std::string> ped_set_pre; 

    // bool fgp = false;
    geometry_msgs::Pose2D bot_pos;
    geometry_msgs::Pose2D bot_vel;
    geometry_msgs::Pose2D next_wp_pos;      // next waypoint position

    planning_stack::ped ped_trial;

    void step();        // each PF step
    std::vector<double> calculateForce();
    void calcFatt(std::vector<double> &F_tot);
    void calcFrep_p(std::vector<double> &F_tot);
    void calcFrep_w(std::vector<double> &F_tot);
    void calcDangerIndex(std::vector<double> &F_tot);
    void calcFrep_v(std::vector<double> &F_tot);
    bool check();     // check if reaches the next way point
    void getNextWaypoint(const int i);
    double distance(const geometry_msgs::Pose2D &a , const geometry_msgs::Pose2D &b);
    bool isInMap(const int &x, const int &y);
    // std::vector<int> pickRlocationWithinRange(); // returns a vectorof 1s and 0s. 1:in range, 0:not in range
    
    void Odom_call_back(const nav_msgs::Odometry::ConstPtr &msg);
    void rlocationProcessing(const gazebo_msgs::ModelStates::ConstPtr &msg);
    void pickRlocationFromCameras(const gazebo_msgs::ModelStates::ConstPtr &msg);      // pick the peds from the other objs in the simualtor
    void pickRlocationFromWorld(const gazebo_msgs::ModelStates::ConstPtr &msg);     // pick the peds from god view
    void pickRlocationWithinRange();   
    void printRlocation();
    void getPedVelocity();
    void getPedVelocity_DI();   // gets peds velosity gazebo frame
    void updatePedInfo();
    double clamp(double value);      // clamp the velocity of the pedestrians 
    void newPedMsgTest();
    void getSparseGlobalPath();     // get waypoints from dijkstra result
    
};

#endif // POTENTIALFIELD