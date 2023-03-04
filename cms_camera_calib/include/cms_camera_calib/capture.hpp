#pragma once
#include "cms_camera_calib/global_header.hpp"



class CAPTURE
{
private: 
    ros::NodeHandle nh;

    // params.yaml
    std::string img_left_topic;
    std::string img_right_topic;
    std::string save_directory;

    // message filter - camera & lidar
    message_filters::Subscriber<sensor_msgs::Image> img_left_sub; 
    message_filters::Subscriber<sensor_msgs::Image> img_right_sub; 
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;
    typedef message_filters::Synchronizer<MySyncPolicy> Sync;
    boost::shared_ptr<Sync> sync_;

    // save related
    std::string filename_left;
    std::string filename_right;
    bool SAVE_FLAG_ROS = false;

    int count          = 1;
    int image_sec      = 0;
    int image_nsec     = 0;



public : 

    CAPTURE()
    {
        nh.param<std::string>("/cms_camera_calib/img_left_topic", img_left_topic, "/camera_array/left/image_raw");
        nh.param<std::string>("/cms_camera_calib/img_right_topic", img_right_topic, "/camera_array/right/image_raw");
        nh.param<std::string>("/cms_camera_calib/save_directory", save_directory, "/home/cms-desktop/catkin_ws/src/cms_camera_calib/DATA/");
        nh.param<bool>("/cms_camera_calib/saveFLAG", SAVE_FLAG_ROS, false );

        img_left_sub.subscribe(nh, "/camera_array/left/image_raw", 1);
        img_right_sub.subscribe(nh, "/camera_array/right/image_raw", 1);
        sync_.reset(new Sync(MySyncPolicy(10), img_left_sub, img_right_sub));
        sync_->registerCallback(boost::bind(&CAPTURE::callback, this, _1, _2));
    }


    void callback(const sensor_msgs::ImageConstPtr& img_left, const sensor_msgs::ImageConstPtr& img_right)
    {
        // ROS_INFO("Synchronization successful");

        // Change format  ROS -> OpenCV & PCL
        cv::Mat left;
        left = cv_bridge::toCvShare(img_left,"bgr8")->image;
        cv::Mat right;
        right = cv_bridge::toCvShare(img_right,"bgr8")->image;

        // Load time stamp
        image_sec  = img_left->header.stamp.sec;
        image_nsec = img_left->header.stamp.nsec; 

        // get ROS parameter from terminal
        nh.getParam("/cms_camera_calib/saveFLAG", SAVE_FLAG_ROS);

        if(SAVE_FLAG_ROS)
        {
            capture_camera(left, right);
            nh.setParam("/cms_camera_calib/saveFLAG", false);
        }   
    }


    void capture_camera(cv::Mat left, cv::Mat right)
    {
        int NUM_ZEROS = 3;
        std::string count_str = std::to_string(count);
        std::string count_str_padded = std::string(NUM_ZEROS - count_str.length(),'0') + count_str;

        // Save image
        filename_left = save_directory + "LEFT/" + "image" + count_str_padded + ".png";
        cv::imwrite(filename_left, left);
        filename_right = save_directory + "RIGHT/" + "image" + count_str_padded + ".png";
        cv::imwrite(filename_right, right);

        ROS_INFO("Success to save IMG %04d. Captured time : [%09d,%09d]. ", count, image_sec, image_nsec);

        count++;
    }

};
