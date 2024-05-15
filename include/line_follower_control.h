#ifndef LINE_FOLLOWER_CONTROL_H_
#define LINE_FOLLOWER_CONTROL_H_

#include "order_interpreter.hpp"

#include "rclcpp/rclcpp.hpp"
#include "ai_msgs/msg/perception_targets.hpp"


class LineFollowerControlNode : public rclcpp::Node{
public:
    LineFollowerControlNode(const std::string &node_name, const rclcpp::NodeOptions &options = rclcpp::NodeOptions());
    ~LineFollowerControlNode();

private:
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr point_subscriber_;
    void point_subscription_callback(const ai_msgs::msg::PerceptionTargets::SharedPtr point_msg);
    std::shared_ptr<OrderInterpreter> order_interpreter_;
    std::string sub_topic_ = "line_center_detection";

};



#endif //LINE_FOLLOWER_CONTROL_H_