#include <ros/ros.h>
#include <thread>
#include <sensor_msgs/JointState.h>
#include <vector>
#include <algorithm>

#include <tinyxml.h>

#ifndef BR_ROBOT_H
#define BR_ROBOT_H

// B&R writes on axis every 2ms
#define MACHINE_TICK 0.002
#define PI 3.1415926535897

class BRrobot
{
private:
    ros::NodeHandle nh; // node handle
    ros::Subscriber joint_sub; // Desired jonint position
    ros::Publisher robot_state_pub;

    std::thread readingThread_; // thread to read data
    std::thread writingThread_; // thread to write socket data
    std::thread statePublisherThread_; // thread to publish data
    std::thread stateMachineThread_; // thread to publish data

    void readData();
    void writeData();  
    void statePublisher();
    void stateMachine(int config);

    void JointDeviationCallback(const sensor_msgs::JointState::ConstPtr& msg);

    bool jointStateReceived;
    bool keepalive_;
    bool debug_;
    int status; // Robot status
    int number_of_cables_; // number of cables


    // Parsing functions
    void unpack_message(std::string buffer); // unpack message
    void extract_robot_state(TiXmlElement* root,std::string State);
    std::string pack_joint_message(); // pack the message

    void check_attribute(int attribute);

    bool kill_signal;
    int incoming_sockfd_;
    int new_sockfd_;
    const unsigned int REVERSE_PORT_;

    sensor_msgs::JointState desired_joint_position; // Declaration of message
    sensor_msgs::JointState robot_state;

    std::vector<bool> Motor_Control;
    bool All_Motor_Control;
    void SetJointFlag(bool Flag); // Set jointrecieved flag
    bool GetJointFlag(); // Get the joint recieved flag
    int GetStatus(); // Set the status flag
    void SetStatus(int s); // Get the status flag

    // Commuinication functions
    bool startCommuinication();
    bool halt(int restart);

    std::vector<double> q_pack_,tau_pack_,qdot_pack_;


public:
    BRrobot(ros::NodeHandle nh_,
            int reverse_port,
            int number_of_cables=8);

   int StartInterface(int config);
   enum robot_status {DISCONNECTED,CONNECTED,CONNECTING,DISCONNECTING,PENDING_CONNECTION};
};

#endif
