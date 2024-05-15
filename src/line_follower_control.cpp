#include "line_follower_control.h"

#include <unistd.h>

LineFollowerControlNode::LineFollowerControlNode(const std::string& node_name, const rclcpp::NodeOptions& options) : rclcpp::Node(node_name, options) {
    
    this->declare_parameter<std::string>("sub_topic", sub_topic_);

    this->get_parameter<std::string>("sub_topic", sub_topic_);
    
    point_subscriber_ =  this->create_subscription<ai_msgs::msg::PerceptionTargets>(
      sub_topic_,
      1,
      std::bind(&LineFollowerControlNode::point_subscription_callback,
      this,
      std::placeholders::_1)); 
    order_interpreter_ = std::make_shared<OrderInterpreter>();
    order_interpreter_->control_serial_servo("stand");
    order_interpreter_->control_PWM_servo(1, 1000, 200);
    order_interpreter_->control_PWM_servo(2, 1500, 200);
}
LineFollowerControlNode::~LineFollowerControlNode(){

}

void LineFollowerControlNode::point_subscription_callback(const ai_msgs::msg::PerceptionTargets::SharedPtr point_msg){
    int x = int(point_msg->targets[0].points[0].point[0].x);
    int y = int(point_msg->targets[0].points[0].point[0].y);
    float temp =  x- 320.0;
    if (( temp >= -40 ) && ( temp <= 40 )) {
        order_interpreter_->control_serial_servo("go_forward");
    } 
    else if ((temp > 40) && (temp <= 320)) {
        order_interpreter_->control_serial_servo("turn_right_fast");
    } else if((temp < -40) && (temp >= -320)) {
        order_interpreter_->control_serial_servo("turn_left_fast");
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("LineFollowerControlNode"), "x value error");
    }
    sleep(0.1);
}

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<LineFollowerControlNode>("LineFollowerControlNode"));

    rclcpp::shutdown();

    RCLCPP_WARN(rclcpp::get_logger("LineFollowerControlNode"), "Pkg exit");

    return 0;
}