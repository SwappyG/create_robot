#include "create/create.h"

#include <ros/node_handle.h>

#include <iostream>

int main(int argc, char** argv) {
  ros::init(argc, argv, "create_test");
  auto n = ros::NodeHandle();

  auto model = create::RobotModel::CREATE_2;
  auto port = std::string{"/dev/ttyUSB0"};
  auto baud = 115200;
  std::cout << "Running driver for Create 2" << std::endl;

  // Construct robot object
  auto robot = create::Create(model);

  // Connect to robot
  if (robot.connect(port, baud)) {
    std::cout << "Connected to robot" << std::endl;
  } else {
    std::cout << "Failed to connect to robot on port " << port.c_str() << "\n";
    return EXIT_FAILURE;
  }

  // Switch to Full mode
  robot.setMode(create::MODE_FULL);

  // Get battery capacity (max charge)
  const float battery_capacity = robot.getBatteryCapacity();

  float battery_charge = 0.0f;
  auto r = ros::Rate(10);
  while (ros::ok()) {
    // Get battery charge
    battery_charge = robot.getBatteryCharge();

    // Print battery percentage
    std::cout << "\rBattery level: "
              << (battery_charge / battery_capacity) * 100.0 << "%";

    r.sleep();
  }

  return 0;
}
