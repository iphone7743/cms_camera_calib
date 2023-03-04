cmake_minimum_required(VERSION 3.0.2)
project(cms_camera_calib)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
)
find_package(OpenCV REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES opencv
#  LIBRARIES cms_camera_calib
#  CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(capture src/main_capture.cpp)
target_link_libraries(capture ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})