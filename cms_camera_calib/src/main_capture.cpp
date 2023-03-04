#include "cms_camera_calib/capture.hpp"

int main (int argc, char** argv)
{
    ros::init (argc, argv, "capture");
    std::cout << "###############################################################################" << std::endl;
    std::cout << "###  Please type below command to capture lidar and camera (other terminal) ###" << std::endl;
    std::cout << "###        >>  rosparam set /cms_camera_calib/saveFLAG true  <<             ###" << std::endl;
    std::cout << "###############################################################################" << std::endl;
    CAPTURE capture;
    ros::spin();
}